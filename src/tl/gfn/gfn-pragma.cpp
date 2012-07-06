/*--------------------------------------------------------------------
  (C) Copyright 2006-2011 Barcelona Supercomputing Center 
                          Centro Nacional de Supercomputacion
  
  This file is part of Mercurium C/C++ source-to-source compiler.
  
  See AUTHORS file in the top level directory for information 
  regarding developers and contributors.
  
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 3 of the License, or (at your option) any later version.
  
  Mercurium C/C++ source-to-source compiler is distributed in the hope
  that it will be useful, but WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the GNU Lesser General Public License for more
  details.
  
  You should have received a copy of the GNU Lesser General Public
  License along with Mercurium C/C++ source-to-source compiler; if
  not, write to the Free Software Foundation, Inc., 675 Mass Ave,
  Cambridge, MA 02139, USA.
--------------------------------------------------------------------*/



#include "gfn-pragma.hpp"
#include "gfn-parallel_for.hpp"
#include "gfn-atomic.hpp"
#include "gfn-exception.hpp"


#include <algorithm>

using namespace TL::GFN;


GFNPragmaPhase::GFNPragmaPhase()
    : PragmaCustomCompilerPhase("gfn")
{
    set_phase_name("Griffon Transformations");
    set_phase_description("This phase implements griffon transformations "
            "available through the usage of #pragma gfn");
    
    register_construct("parallel_for");
    on_directive_post["parallel_for"].connect(functor(&GFNPragmaPhase::parallel_for, *this));
    // XXX: on_directive_pre ???
    
    register_construct("overlapcompute");
    on_directive_post["overlapcompute"].connect(functor(&GFNPragmaPhase::overlapcompute, *this));
    
    register_construct("barrier");
    on_directive_post["barrier"].connect(functor(&GFNPragmaPhase::barrier, *this));
    
    register_construct("atomic");
    on_directive_post["atomic"].connect(functor(&GFNPragmaPhase::atomic, *this));
}
#if 0
HLTPragmaPhase::HLTPragmaPhase()
    : PragmaCustomCompilerPhase("hlt")
{
    set_phase_name("High Level Transformations");
    set_phase_description("This phase implements several high level "
            "transformations available through the usage of #pragma hlt");

    register_construct("unroll");
    on_directive_post["unroll"].connect(functor(&HLTPragmaPhase::unroll_loop, *this));

    register_construct("fusion");
    on_directive_pre["fusion"].connect(functor(&HLTPragmaPhase::pre_fuse_loops, *this));
    on_directive_post["fusion"].connect(functor(&HLTPragmaPhase::fuse_loops, *this));

    register_construct("task_aggregate");
    on_directive_post["task_aggregate"].connect(functor(&HLTPragmaPhase::task_aggregate, *this));

    register_construct("simd");
    on_directive_post["simd"].connect(functor(&HLTPragmaPhase::simdize, *this));

    _allow_identity_str = "1";

    register_parameter("allow_identity", 
            "Use this to disable identity, this is for testing only",
            _allow_identity_str,
            "true").connect(functor( update_identity_flag ));

    register_parameter("instrument",
            "Enables mintaka instrumentation if set to '1'",
            _enable_hlt_instr_str,
            "0").connect(functor( &HLTPragmaPhase::set_instrument_hlt, *this ));
}
#endif

void GFNPragmaPhase::run(TL::DTO& dto)
{
    try
    {
        _scope_link = dto["scope_link"];
        _translation_unit = dto["translation_unit"];
        
        PragmaCustomCompilerPhase::run(dto);
    }
    catch (GFNException e)
    {
        std::cerr << e << std::endl;
        set_phase_status(PHASE_STATUS_ERROR);
    }
    catch (...)
    {
        std::cerr << "(gfn-phase): error: unknown exception" << std::endl;
        set_phase_status(PHASE_STATUS_ERROR);
    }
}

static void parallel_for_fun(TL::PragmaCustomConstruct construct, 
                             TL::ForStatement for_stmt, 
                             KernelInfo *kernel_info)
{
    TL::Source kernel_call_src = TL::GFN::parallel_for(construct, for_stmt, kernel_info);
    
    TL::AST_t kernel_call_tree = kernel_call_src.parse_statement(for_stmt.get_ast(),
            for_stmt.get_scope_link());

    for_stmt.get_ast().replace(kernel_call_tree);
}

void GFNPragmaPhase::parallel_for(PragmaCustomConstruct construct)
{
    Statement statement = construct.get_statement();
    KernelInfo *kernel_info = new KernelInfo();
    
    ForStatement for_statement(construct.get_statement().get_ast(), construct.get_scope_link());
    Statement for_body = for_statement.get_loop_body();
    
    /*if (for_statement.empty())
    {
        throw GFNException(construct, "not found for statement followed #pragma gfn parallel_for");
    }*/
    
    ObjectList<IdExpression> symbol_list = for_body.all_symbol_occurrences(TL::Statement::ONLY_VARIABLES);
    
    TL::PragmaCustomClause kernelname_clause = construct.get_clause("kernelname");
    if (kernelname_clause.is_defined())
    {
        //std::cout << "In kernel name clause\n";
        ObjectList<std::string> kernel_name_list = kernelname_clause.get_arguments();
        
        if (kernel_name_list.size() != 1)
        {
            throw GFNException(construct, "kernelname only accepts one name");
        }
        kernel_info->set_kernel_name(kernel_name_list[0]);
    }
    
    TL::PragmaCustomClause waitfor_clause = construct.get_clause("waitfor");
    if (waitfor_clause.is_defined())
    {
        ObjectList<std::string> kernel_name_list = waitfor_clause.get_arguments();
        kernel_info->set_wait_for(kernel_name_list);
    }
    
    TL::PragmaCustomClause private_clause = construct.get_clause("private");
    if (private_clause.is_defined())
    {
        //ObjectList<std::string> identifier_list = private_clause.get_arguments();
        ObjectList<Expression> identifier_list = private_clause.get_expression_list();
        ObjectList<DataReference> private_name_list;

        for (ObjectList<Expression>::iterator it = identifier_list.begin();
             it != identifier_list.end();
             it++)
        {
            // FIXME: scope of variable. it is able to have two or more variables 
            // that has the same name
            
            Type var_type = it->get_type();
	    
            bool found = false;
            for (ObjectList<IdExpression>::iterator sit = symbol_list.begin();
                 sit != symbol_list.end();
                 sit++)
            {
                if (sit->get_symbol().get_name() == it->prettyprint())
                {   
                    found = true;
                    DataReference data_ref(*it);
                    
                    if (0 /*!data_ref.is_valid(warning)*/)
                    {
                        /*std::cerr << warning;
                        std::cerr << data_ref.get_ast().get_locus() << ": warning: '" << data_ref 
                            << "' is not a valid name for private variables" << std::endl;*/
                    }
                    else
                    {
                        private_name_list.push_back(data_ref);
                    }
                    break;
                }
            }
            if (!found)
            {
                // Remove from list
                std::cerr << it->prettyprint() << " is not variable.\n";
            }
        }

        kernel_info->set_private_list(private_name_list);
    }
    
    TL::PragmaCustomClause accurate_clause = construct.get_clause("accurate");
    if (accurate_clause.is_defined())
    {
        ObjectList<std::string> accurate_arg = private_clause.get_arguments();
        
        if (accurate_arg.size() != 1)
        {
            std::cerr << "" << std::endl;
        }
        
        accurate_arg[0];
    }
    
    TL::PragmaCustomClause reduction_clause = construct.get_clause("reduction");
    if (reduction_clause.is_defined())
    {
        // DO SOMETHING
        //ObjectList<OpenMP::ReductionSymbol> reduction_references;

        //ObjectList<ObjectList<std::string> > clause_arguments = reduction_clause.get_arguments_unflattened();
    }
    
    
    
    parallel_for_fun(construct, for_statement, kernel_info);

    construct.get_ast().replace(statement.get_ast());
}

void GFNPragmaPhase::overlapcompute(PragmaCustomConstruct construct)
{
    TL::PragmaCustomClause kernelname_clause = construct.get_clause("kernelname");
    if (kernelname_clause.is_defined())
    {
        ObjectList<std::string> kernel_name_list = kernelname_clause.get_arguments();
        
        if (kernel_name_list.size() != 1)
        {
            throw GFNException(construct, "kernelname only accepts one name");
        }
        kernel_name_list[0];
    }
    else
    {
        // get last kernelname in overlapcompute scope
    }
}

void GFNPragmaPhase::barrier(PragmaCustomConstruct construct)
{
    Source result;

    result << "__threadfence();";
    
    AST_t threadfence_tree = result.parse_statement(construct.get_ast(), 
            construct.get_scope_link());

    construct.get_ast().replace(threadfence_tree);
}

static void atomic_fun(TL::Statement assign_stmt)
{
    TL::Source atomic_call_src = TL::GFN::atomic(assign_stmt.get_expression());
    
    TL::AST_t atomic_call_tree = atomic_call_src.parse_statement(assign_stmt.get_ast(),
            assign_stmt.get_scope_link());

    assign_stmt.get_ast().replace(atomic_call_tree);
}

void GFNPragmaPhase::atomic(PragmaCustomConstruct construct)
{
    Statement statement = construct.get_statement();
    Expression expression = statement.get_expression();
    
    if (!expression.is_operation_assignment())
    {
        // XXX: one of +=, -=, *=, /=, ^=, |=, &&=, ||= 
        throw GFNException(construct, "after #pragma gfn atomic, statement must be assignment statement");
    }
    
    atomic_fun(statement);
    
    construct.get_ast().replace(statement.get_ast());
}

EXPORT_PHASE(TL::GFN::GFNPragmaPhase)
