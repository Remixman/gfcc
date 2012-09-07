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
#include <fstream>

using namespace TL::GFN;


GFNPragmaPhase::GFNPragmaPhase()
    : PragmaCustomCompilerPhase("gfn")
{
    set_phase_name("Griffon Transformations");
    set_phase_description("This phase implements griffon transformations "
            "available through the usage of #pragma gfn");

    register_construct("init");
    on_directive_post["init"].connect(functor(&GFNPragmaPhase::init, *this));

    register_construct("finalize");
    on_directive_post["finalize"].connect(functor(&GFNPragmaPhase::finalize, *this));

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
    /* Read config file consist of
     *  - GPU & Cluster specify & topology TODO:
     *  - compile config (cluster, gpu, gpucluster) */
    // TODO: change config directory
    /*std::ifstream config_file("/home/remixman/Desktop/gfn_testsuite/config.gfn");
    std::string s;
    config_file >> s;
    BaseTransform::is_cluster_trans = false;
    BaseTransform::is_gpu_trans = false;
    if (s == "cluster")
        BaseTransform::is_cluster_trans = true;
    else if (s == "gpu")
        BaseTransform::is_gpu_trans = true;
    else if (s == "gpucluster") {
        BaseTransform::is_cluster_trans = true;
        BaseTransform::is_gpu_trans = true;
    }*/

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

void GFNPragmaPhase::init(PragmaCustomConstruct construct)
{
    Source result;

    result
        << comment("MPI_Init")
        << "_gfn_mpi_init();"
        // XXX: hacking follow construct statement disappear
        << construct.get_statement().prettyprint();

    AST_t mpi_init_tree = result.parse_statement(construct.get_ast(),
            construct.get_scope_link());

    construct.get_ast().replace(mpi_init_tree);
}

void GFNPragmaPhase::finalize(PragmaCustomConstruct construct)
{
    Source result;

    result
        << comment("MPI finalize")
        << "_gfn_mpi_finalize();"
        // XXX: hacking follow construct statement disappear
        << construct.get_statement().prettyprint();

    AST_t mpi_final_tree = result.parse_statement(construct.get_ast(),
            construct.get_scope_link());

    construct.get_ast().replace(mpi_final_tree);
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

    // XXX: find use and def variable
    Statement loop_body = for_statement.get_loop_body();
    kernel_info->loop_index_var_name = for_statement.get_induction_variable()
            .get_symbol().get_name();
    find_use_and_def_list(loop_body, kernel_info);

    // get data from clauses
    get_kernelname_clause(construct, kernel_info);
    get_waitfor_clause(construct, kernel_info);
    get_private_clause(construct, kernel_info, symbol_list);
    get_accurate_clause(construct, kernel_info);
    get_reduction_clause(construct, kernel_info);
    get_size_clause(construct, kernel_info);

    // DEBUG: print use and def list
    std::cout << "\n====================================================\n";
    std::cout << "USE LIST for kernel\n";
    for (ObjectList<DataReference>::iterator it = kernel_info->_use_list.begin();
         it != kernel_info->_use_list.end();
         ++it)
    {
        std::string var = it->prettyprint();
        std::cout << " - " << var /*<< " , size = "
                  << kernel_info->_dim_size_list[var].prettyprint()*/ << "\n";
    }
    std::cout << "====================================================\n";
    std::cout << "DEF LIST for kernel\n";
    for (ObjectList<DataReference>::iterator it = kernel_info->_def_list.begin();
         it != kernel_info->_def_list.end();
         ++it)
    {
        std::string var = it->prettyprint();
        std::cout << " - " << var /*<< " , size = "
                  << kernel_info->_dim_size_list[var].prettyprint()*/ << "\n";
    }
    std::cout << "====================================================\n\n";
    
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

    result
        << comment("barrier")
        << "__threadfence();"
        // XXX: hacking follow construct statement disappear
        << construct.get_statement().prettyprint();
    
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

void GFNPragmaPhase::get_kernelname_clause(PragmaCustomConstruct construct,
                                           KernelInfo *kernel_info)
{
    TL::PragmaCustomClause kernelname_clause = construct.get_clause("kernelname");
    if (kernelname_clause.is_defined())
    {
        ObjectList<std::string> kernel_name_list = kernelname_clause.get_arguments();

        if (kernel_name_list.size() != 1)
        {
            throw GFNException(construct, "kernelname only accepts one name");
        }
        kernel_info->set_kernel_name(kernel_name_list[0]);
    }
}

void GFNPragmaPhase::get_waitfor_clause(PragmaCustomConstruct construct,
                                        KernelInfo *kernel_info)
{
    TL::PragmaCustomClause waitfor_clause = construct.get_clause("waitfor");
    if (waitfor_clause.is_defined())
    {
        ObjectList<std::string> kernel_name_list = waitfor_clause.get_arguments();
        kernel_info->set_wait_for(kernel_name_list);
    }
}

void GFNPragmaPhase::get_private_clause(PragmaCustomConstruct construct,
                                        KernelInfo *kernel_info,
                                        ObjectList<IdExpression> &symbol_list)
{
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
}

void GFNPragmaPhase::get_accurate_clause(PragmaCustomConstruct construct,
                                         KernelInfo *kernel_info)
{
    TL::PragmaCustomClause accurate_clause = construct.get_clause("accurate");
    if (accurate_clause.is_defined())
    {
        ObjectList<std::string> accurate_arg = accurate_clause.get_arguments();

        if (accurate_arg.size() != 1)
        {
            std::cerr << "" << std::endl;
        }

        accurate_arg[0];
    }
}

void GFNPragmaPhase::get_reduction_clause(PragmaCustomConstruct construct,
                                          KernelInfo *kernel_info)
{
    TL::PragmaCustomClause reduction_clause = construct.get_clause("reduction");
    if (reduction_clause.is_defined())
    {
        // DO SOMETHING
        //ObjectList<OpenMP::ReductionSymbol> reduction_references;

        //ObjectList<ObjectList<std::string> > clause_arguments = reduction_clause.get_arguments_unflattened();
    }
}

void GFNPragmaPhase::get_size_clause(PragmaCustomConstruct construct,
                                     KernelInfo *kernel_info)
{
    TL::PragmaCustomClause size_clause = construct.get_clause("size");
    if (size_clause.is_defined())
    {
        ObjectList<std::string> list_arg = size_clause.get_arguments();
        for (ObjectList<std::string>::iterator it = list_arg.begin();
             it != list_arg.end();
             ++it)
        {
            // var,var2,var3,... : dim_size1, dim_size2, dim_size3
            size_t colon_pos = it->find(":");
            if (colon_pos == std::string::npos)
            {
                std::cerr << "size clause must have \':\' before size\n";
                // TODO: throw
            }

            std::string var_list_str = it->substr(0, colon_pos);
            std::string size_list_str = it->substr(colon_pos+1);

            if (var_list_str.size() == 0 || size_list_str.size() == 0)
            {
                std::cerr << "size clause syntax error";
            }

            ObjectList<std::string> var_list, size_list;
            size_t start_pos = 0, comma_pos = 0;
            while ((comma_pos = var_list_str.find(",", start_pos)) != std::string::npos) {
                std::string var = var_list_str.substr(start_pos, comma_pos - start_pos);
                if (var.size() != 0)
                {
                    var_list.push_back(var);
                }
                start_pos = comma_pos + 1;
            }
            if (start_pos != var_list_str.size())
            {
                std::string var = var_list_str.substr(start_pos, var_list_str.size() - start_pos);
                var_list.push_back(var);
            }

            // parse size list
            start_pos = 0, comma_pos = 0;
            while ((comma_pos = size_list_str.find(",", start_pos)) != std::string::npos) {
                std::string size = size_list_str.substr(start_pos, comma_pos - start_pos);
                if (size.size() != 0)
                {
                    size_list.push_back(size);
                }
                start_pos = comma_pos + 1;
            }
            if (start_pos != size_list_str.size())
            {
                std::string size = size_list_str.substr(start_pos, size_list_str.size() - start_pos);
                size_list.push_back(size);
            }
            while (size_list.size() < 3)
            {
                size_list.push_back("1");
            }

            // set size data to kernel
            for (ObjectList<std::string>::iterator it = var_list.begin();
                 it != var_list.end();
                 ++it)
            {
                if (kernel_info->get_use_list_index(*it) == -1 &&
                    kernel_info->get_def_list_index(*it) == -1)
                {
                    std::cerr << "cannot find " << *it << " in kernel\n";
                    continue;
                }

                DimSize &dim_size = kernel_info->_dim_size_list[*it];
                dim_size._dim1_size = size_list[0];
                dim_size._dim2_size = size_list[1];
                dim_size._dim3_size = size_list[2];
            }
        }
    }
}

void GFNPragmaPhase::find_use_and_def_list(TL::Statement compound_stmt,
                                           KernelInfo *kernel_info)
{

    if (!compound_stmt.is_compound_statement())
    {
        std::cerr
            << "Error at gfn-parallel_for.cpp:find_use_and_def_list\n";
        return;
    }

    ObjectList<Statement> statements = compound_stmt.get_inner_statements();

    /* FIXME: if def before use eg.
     * A[i] = 6;
     * B[i] = A[i];
     * A is def but also use;
     *
     * A[i] = 6
     * B[i] = A[i-1]??
     *
     * A[i] += 9; ?
     */
    for (ObjectList<Statement>::iterator it = statements.begin();
         it != statements.end();
         ++it)
    {
        Expression expression = it->get_expression();
        ObjectList<IdExpression> stmt_var_list;

        /* XXX: if pass to function we don't know use or def
         * add to both list.
         */

        if (it->is_compound_statement())
        {
            find_use_and_def_list(*it, kernel_info);
        }
        else if (expression.is_assignment() || expression.is_operation_assignment())
        {
            DataReference def_var_ref(expression.get_first_operand());

            kernel_info->push_to_def_list(def_var_ref);

            stmt_var_list = expression.get_second_operand().all_symbol_occurrences(Statement::ONLY_VARIABLES);
        }
        else
        {
            // XXX: get all variable in statement
            stmt_var_list = it->all_symbol_occurrences(Statement::ONLY_VARIABLES);
        }

        //
        for (ObjectList<IdExpression>::iterator iit = stmt_var_list.begin();
             iit != stmt_var_list.end();
             iit++)
        {
            DataReference data_ref(iit->get_expression());
            kernel_info->push_to_use_list(data_ref);
        }
    }
}

EXPORT_PHASE(TL::GFN::GFNPragmaPhase)
