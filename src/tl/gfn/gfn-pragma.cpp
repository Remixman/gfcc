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
#include "gfn-config.hpp"


#include <algorithm>
#include <fstream>

using namespace TL::GFN;

////////////////////////// HELPER FUNCTION //////////////////////////////
static void parse_var_list_and_append(std::string var_list_str,
                                      TL::ObjectList<std::string> &var_list)
{
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
}

static void parse_pattern_list_and_append(std::string var_list_str,
                                          TL::ObjectList<std::string> &var_names,
                                          TL::ObjectList<int> &pattern_types,
                                          TL::ObjectList< TL::ObjectList<std::string> > &pattern_arrays)
{
    // Compare , [ { what first
    size_t start_pos = 0;
    size_t comma_pos;
    size_t l_sq_bracket_pos, r_sq_bracket_pos;
    size_t l_brace_pos, r_brace_pos;
    size_t colon_pos;
    TL::ObjectList<std::string> pattern_array;
    
    std::remove(var_list_str.begin(), var_list_str.end(), ' '); // remove empty
    
    /* RANGE TYPE & SPECIFIC TYPE
     * in_pattern(A:[-1,1], B:{-2,0,2}) */
    while (true)
    {
        comma_pos = var_list_str.find(",", start_pos);
        l_sq_bracket_pos = var_list_str.find("[", start_pos);
        l_brace_pos = var_list_str.find("{", start_pos);
        colon_pos = var_list_str.find(":", start_pos);
            
        if (comma_pos == std::string::npos &&
            l_sq_bracket_pos == std::string::npos &&
            l_brace_pos == std::string::npos)
        {
            return;
        }
        
        std::string var_name;
        var_name = var_list_str.substr(start_pos, colon_pos - start_pos);
        var_names.append(var_name);
        
        // Range type
        if (l_sq_bracket_pos < comma_pos &&
            l_sq_bracket_pos < l_brace_pos)
        {
            start_pos = l_sq_bracket_pos + 1;
            pattern_types.append(GFN_PATTERN_RANGE);
            
            while (true)
            {
                comma_pos = var_list_str.find(",", start_pos);
                r_sq_bracket_pos = var_list_str.find("]", start_pos);
                
                size_t min_pos = std::min(comma_pos, r_sq_bracket_pos);
                std::string val = var_list_str.substr(start_pos, min_pos - start_pos);
                pattern_array.append(val);
                
                start_pos = min_pos + 1;
                
                if (r_sq_bracket_pos < comma_pos) break;
            }
            
            pattern_arrays.append(pattern_array);
        }
        
        // Spec type
        else if (l_brace_pos < comma_pos &&
                 l_brace_pos < l_sq_bracket_pos)
        {
            start_pos = l_brace_pos + 1;
            pattern_types.append(GFN_PATTERN_SPEC);
            
            while (true)
            {
                comma_pos = var_list_str.find(",", start_pos);
                r_brace_pos = var_list_str.find("}", start_pos);
                
                size_t min_pos = std::min(comma_pos, r_brace_pos);
                std::string val = var_list_str.substr(start_pos, min_pos - start_pos);
                pattern_array.append(val);
                
                start_pos = min_pos + 1;
                
                if (r_brace_pos < comma_pos) break;
            }
            
            pattern_arrays.append(pattern_array);
        }
        
        start_pos += 1; // consume ,
    }
}
//////////////////////////////////////////////////////////////////////////

GFNPragmaPhase::GFNPragmaPhase()
    : PragmaCustomCompilerPhase("gfn")
{
    set_phase_name("Griffon Transformations");
    set_phase_description("This phase implements griffon transformations "
            "available through the usage of #pragma gfn");

    register_construct("start");
    on_directive_pre["start"].connect(functor(&GFNPragmaPhase::start, *this));

    register_construct("finish");
    on_directive_pre["finish"].connect(functor(&GFNPragmaPhase::finish, *this));

    register_construct("parallel_for");
    on_directive_post["parallel_for"].connect(functor(&GFNPragmaPhase::parallel_for, *this));
    // XXX: on_directive_pre ???
    
    register_construct("use_in_parallel");
    on_directive_post["use_in_parallel"].connect(functor(&GFNPragmaPhase::use_in_parallel, *this));
    
    register_construct("overlapcompute");
    on_directive_post["overlapcompute"].connect(functor(&GFNPragmaPhase::overlapcompute, *this));

    register_construct("atomic");
    on_directive_post["atomic"].connect(functor(&GFNPragmaPhase::atomic, *this));
}

void GFNPragmaPhase::run(TL::DTO& dto)
{
    /* Read config file consist of
     *  - GPU & Cluster specify & topology TODO:
     *  - compile config (cluster, gpu, gpucluster) */
    // TODO: change config directory
    std::ifstream config_file("/home/remixman/Desktop/gfn_testsuite/config.gfn");
    std::string s;
    config_file >> s;
    if (s == "cluster")
        Conf_Trans_flags |= GFN_TRANS_MPI;
    else if (s == "gpu")
        Conf_Trans_flags |= GFN_TRANS_CUDA;
    else if (s == "gpucluster")
        Conf_Trans_flags |= GFN_TRANS_MPI | GFN_TRANS_CUDA;

    try
    {
        _scope_link = dto["scope_link"];
        _translation_unit = dto["translation_unit"];
        
        PragmaCustomCompilerPhase::run(dto);

        //std::cout << "TRANSLATION UNIT : \n";
        //std::cout << _translation_unit << "\n\n";
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

/*
 * if program is
 * #pragma gfn A
 * #pragma gfn B
 * startment block
 *
 * PragmaCustomConstruct of A is
 * #pragma gfn A
 * startment block
 *
 * so we cannot replace all of construct
 */
void GFNPragmaPhase::start(PragmaCustomConstruct construct)
{
    Source result;
    result
        << comment("Initialize IPC to worker")
        << "_OpenMasterMsgQueue();"
        << construct.get_statement(); // XXX: get startment of construct before replace

    AST_t master_start_tree = result.parse_statement(construct.get_ast(),
            construct.get_scope_link());

    construct.get_ast().replace(master_start_tree);
}

void GFNPragmaPhase::finish(PragmaCustomConstruct construct)
{
    Source result;

    result
        << comment("Close IPC to worker")
        << "_SendCallFuncMsg(0);"
        << "_CloseMasterMsgQueue();"
        << construct.get_statement(); // XXX: get startment of construct before replace

    AST_t master_finish_tree = result.parse_statement(construct.get_ast(),
            construct.get_scope_link());

    construct.get_ast().replace(master_finish_tree);
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
    
    /* Update kernel count */
    kernel_info->kernel_id = ++KernelInfo::kernel_count;

    /* Get for_statement and for loop body */
    ForStatement for_statement(construct.get_statement().get_ast(), construct.get_scope_link());
    Statement for_body = for_statement.get_loop_body();
    
    /*if (for_statement.empty())
    {
        throw GFNException(construct, "not found for statement followed #pragma gfn parallel_for");
    }*/

    ObjectList<IdExpression> symbol_list = for_statement.all_symbol_occurrences(TL::Statement::ONLY_VARIABLES);

    /* Symbol list is not unique create new unique variable list */
    //std::cout << "\n=====================================================\n";
    //std::cout << "VARIABLE IN KERNEL\n";
    ObjectList<std::string> sym_name_list;
    for (ObjectList<IdExpression>::iterator sit = symbol_list.begin();
         sit != symbol_list.end();
         sit++)
    {
        // TODO: don't add local declare variable
        DataReference data_ref(sit->get_expression());
        std::string name = sit->get_symbol().get_name();

        if (sym_name_list.find(name).size() == 0)
        {
            TL::Type var_type = data_ref.get_type();
            VariableInfo var_info(name);
            if (var_type.is_array() || var_type.is_pointer())
                var_info._is_array_or_pointer = true;
            kernel_info->_var_info.append(var_info);
            kernel_info->_var_ref.append(data_ref);
            sym_name_list.append(name);
        }
    }

    // XXX: find use and def variable
    Statement loop_body = for_statement.get_loop_body();
    kernel_info->loop_index_var_name = for_statement.get_induction_variable()
            .get_symbol().get_name();
    collect_loop_info(for_statement, kernel_info);
    find_use_and_def_list(loop_body, kernel_info);
    collect_variable_info(loop_body, kernel_info);

    // get data from clauses
    get_kernelname_clause(construct, kernel_info);
    get_waitfor_clause(construct, kernel_info);
    get_private_clause(construct, kernel_info, symbol_list);
    get_reduction_clause(construct, kernel_info);
    
    get_input_clause(construct, kernel_info);
    get_output_clause(construct, kernel_info);
    get_inout_clause(construct, kernel_info);
    get_temp_clause(construct, kernel_info);
    
    get_if_clause(construct, kernel_info);
    
    get_in_pattern_clause(construct, kernel_info);
    get_out_pattern_clause(construct, kernel_info);

    // DEBUG: print use and def list
    std::cout << "\n====================================================\n";
    std::cout << "USE LIST for kernel\n";
    for (ObjectList<DataReference>::iterator it = kernel_info->_use_list.begin();
         it != kernel_info->_use_list.end();
         ++it)
    {
        std::string var = it->prettyprint();
        std::cout << " - " << var << "\n";
    }
    std::cout << "====================================================\n";
    std::cout << "DEF LIST for kernel\n";
    for (ObjectList<DataReference>::iterator it = kernel_info->_def_list.begin();
         it != kernel_info->_def_list.end();
         ++it)
    {
        std::string var = it->prettyprint();
        std::cout << " - " << var << "\n";
    }
    std::cout << "====================================================\n\n";
    
    parallel_for_fun(construct, for_statement, kernel_info);

    construct.get_ast().replace(statement.get_ast());
}

void GFNPragmaPhase::use_in_parallel(PragmaCustomConstruct construct)
{
    // Add define GFN_WORKER
    Source result;
    
    result
        << comment("*/ #ifdef GFN_WORKER /*")
        << construct.get_statement()
        << comment("*/ #endif /*");
        
    AST_t use_par_tree = result.parse_statement(construct.get_ast(),
            construct.get_scope_link());
        
    construct.get_ast().replace(use_par_tree);
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

void GFNPragmaPhase::get_if_clause(PragmaCustomConstruct construct,
                                   KernelInfo *kernel_info)
{
    TL::PragmaCustomClause if_clause = construct.get_clause("if");
    
    ObjectList<Expression> cond_list = if_clause.get_expression_list();
    if (cond_list.size() >= 1)
    {
        kernel_info->_level_1_condition = (std::string)cond_list[0];
    }
    if (cond_list.size() >= 2)
    {
        kernel_info->_level_2_condition = (std::string)cond_list[1];
    }
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

void GFNPragmaPhase::get_reduction_clause(PragmaCustomConstruct construct,
                                          KernelInfo *kernel_info)
{
    TL::PragmaCustomClause reduction_clause = construct.get_clause("reduction");
    if (reduction_clause.is_defined())
    {
        ObjectList<std::string> list_arg = reduction_clause.get_arguments();
        for (int i = 0; i < list_arg.size(); ++i)
        {
            std::string reduction_arg = list_arg[i];
            size_t colon_pos = reduction_arg.find(":");
            if (colon_pos == std::string::npos)
            {
                // TODO: Parse error
            }

            std::string opr = reduction_arg.substr(0, colon_pos);
            TL::ObjectList<std::string> reduc_var_list;
            parse_var_list_and_append(reduction_arg.substr(colon_pos+1), reduc_var_list);

            for (int j = 0; j < reduc_var_list.size(); ++j)
            {
                std::string var_name = reduc_var_list[j];
                // Find variable index
                for (int k = 0; k < kernel_info->_var_info.size(); ++k)
                {
                    if (kernel_info->_var_info[k]._name == var_name)
                    {
                        kernel_info->_var_info[k]._is_reduction = true;
                        kernel_info->_var_info[k]._is_output = true;
                        kernel_info->_var_info[k]._reduction_type = op_to_op_type(opr);

                        kernel_info->_has_reduction_clause = true;
                    }
                }
            }
        }
    }
}

void GFNPragmaPhase::get_input_clause(TL::PragmaCustomConstruct construct,
                                      KernelInfo *kernel_info)
{
    TL::PragmaCustomClause input_clause = construct.get_clause("input");
    get_copy_clause(input_clause, kernel_info, "input");
}

void GFNPragmaPhase::get_output_clause(TL::PragmaCustomConstruct construct,
                                       KernelInfo *kernel_info)
{
    TL::PragmaCustomClause output_clause = construct.get_clause("output");
    get_copy_clause(output_clause, kernel_info, "output");
}

void GFNPragmaPhase::get_inout_clause(TL::PragmaCustomConstruct construct, 
                                      KernelInfo* kernel_info)
{
    TL::PragmaCustomClause inout_clause = construct.get_clause("inout");
    get_copy_clause(inout_clause, kernel_info, "inout");
}

void GFNPragmaPhase::get_temp_clause(PragmaCustomConstruct construct,
                                     KernelInfo* kernel_info)
{
    TL::PragmaCustomClause temp_clause = construct.get_clause("temp");
    get_copy_clause(temp_clause, kernel_info, "temp");
}

void GFNPragmaPhase::get_copy_clause(TL::PragmaCustomClause &copy_clause,
                                     KernelInfo *kernel_info,
                                     std::string copy_type_str)
{
    if (copy_clause.is_defined())
    {
        ObjectList<TL::Expression> expr_list = copy_clause.get_expression_list();
        for (ObjectList<TL::Expression>::iterator it = expr_list.begin();
             it != expr_list.end();
             ++it)
        {
            TL::Expression var_expr = *it;
            std::string var_name = "";
            unsigned dim_num = 0;
            TL::ObjectList<TL::Expression> subscript_list;
            
            // Array input/output
            if (var_expr.is_array_subscript())
            {
                TL::Expression tmp_expr = var_expr;
                
                while (!tmp_expr.is_id_expression())
                {
                    TL::Expression subscript_expr = tmp_expr.get_subscript_expression();
                    subscript_list.push_back( subscript_expr );
                    /* Add size variable to input
                       TODO: for complex expression as size like x+y+z */
                    {
                        ObjectList<IdExpression> symbol_list = subscript_expr.all_symbol_occurrences(TL::Statement::ONLY_VARIABLES);
                        ObjectList<VariableInfo> &kernel_name_list = kernel_info->_var_info;
                        for (ObjectList<IdExpression>::iterator sit = symbol_list.begin();
                            sit != symbol_list.end();
                            sit++)
                        {
                            DataReference data_ref(sit->get_expression());
                            std::string name = sit->get_symbol().get_name();

                            if (kernel_info->get_var_info_index_from_var_name(name) < 0)
                            {
                                TL::Type var_type = data_ref.get_type();
                                VariableInfo var_info(name);
                                if (var_type.is_array() || var_type.is_pointer())
                                    var_info._is_array_or_pointer = true;
                                var_info._is_input = true;
                                kernel_info->_var_info.append(var_info);
                                kernel_info->_var_ref.append(data_ref);
                            }
                        }
                    }

                    tmp_expr = tmp_expr.get_subscripted_expression();
                    dim_num++;
                }
                
                var_name = tmp_expr.get_id_expression().get_symbol().get_name();
            }
            // Scalar input/output
            else if (var_expr.is_id_expression())
            {
                var_name = var_expr.get_id_expression().get_symbol().get_name();
                dim_num = 0;
            }
            // Invalid parameter
            else
            {
                std::cerr << "warning : clause \"" << copy_type_str
                          << "\" unknown variable \"" << var_expr.prettyprint()
                          << "\" in parallel region " 
                          << __FILE__ << ":" << __LINE__ << std::endl;
                continue; /* Get next parameter */
            }
            
            
            int idx = kernel_info->get_var_info_index_from_var_name(var_name);
            if (idx != -1)
            {
                //std::cout << "Found " << copy_type_str << " : " << *it << std::endl;
                if (copy_type_str == "input")
                    kernel_info->_var_info[idx]._is_input = true;
                else if (copy_type_str == "output")
                    kernel_info->_var_info[idx]._is_output = true;
                else if (copy_type_str == "inout")
                {
                    kernel_info->_var_info[idx]._is_input = true;
                    kernel_info->_var_info[idx]._is_output = true;
                }
                else if (copy_type_str == "temp")
                {
                    kernel_info->_var_info[idx]._is_input = false;
                    kernel_info->_var_info[idx]._is_output = false;
                }
                else std::cerr << "warning : " << __LINE__ << " at " << __FILE__ << std::endl;
                              
                // Save dimension size data
                kernel_info->_var_info[idx]._dimension_num = dim_num;
                std::cout << "Size of " << var_name << " = ";
                for (int i = 0; i < dim_num; ++i)
                {
                    if (i != 0) std::cout << ":";
                    std::cout << subscript_list[i].prettyprint();
                    kernel_info->_var_info[idx]._dim_size.push_back( subscript_list[i] );
                }
                std::cout << std::endl;
            }
            else
            {
                std::cerr << "warning : clause \"" << copy_type_str
                          << "\" unknown variable \"" << *it
                          << "\" in parallel region " << std::endl;
            }
        }
    }
}

void GFNPragmaPhase::get_in_pattern_clause(TL::PragmaCustomConstruct construct,
                                           KernelInfo *kernel_info)
{
    TL::PragmaCustomClause in_pattern_clause = construct.get_clause("in_pattern");
    get_pattern_clause(in_pattern_clause, kernel_info, "in");
}

void GFNPragmaPhase::get_out_pattern_clause(TL::PragmaCustomConstruct construct,
                                            KernelInfo *kernel_info)
{
    TL::PragmaCustomClause out_pattern_clause = construct.get_clause("out_pattern");
    get_pattern_clause(out_pattern_clause, kernel_info, "out");
}

void GFNPragmaPhase::get_pattern_clause(PragmaCustomClause &pattern_clause,
                                        KernelInfo *kernel_info,
                                        std::string inout_pattern /* "in" or "out" */)
{
    
    if (pattern_clause.is_defined())
    {
        ObjectList<std::string> list_arg = pattern_clause.get_arguments();
        
        ObjectList<std::string> var_list;
        ObjectList<int> pattern_type_list;
        ObjectList< ObjectList<std::string> > pattern_array_list;
        
        
        for (ObjectList<std::string>::iterator it = list_arg.begin();
             it != list_arg.end();
             ++it)
        {
            parse_pattern_list_and_append(*it, var_list,
                                          pattern_type_list, pattern_array_list);
        }
        
        for (int i = 0; i < var_list.size(); ++i)
        {
            // find variable index
            int idx = kernel_info->get_var_info_index_from_var_name(var_list[i]);
            
            if (idx != -1 && inout_pattern == "in")
            {
                // TODO: out_pattern 
                kernel_info->_var_info[idx]._in_pattern_type = pattern_type_list[i];
                kernel_info->_var_info[idx]._in_pattern_array = pattern_array_list[i];
                
                // Debug print
                std::cout << "IN PATTERN FOR " << var_list[i] << " is " 
                    << ((pattern_type_list[i] == GFN_PATTERN_RANGE)? "RANGE" : "SPEC")
                    << " and value are ";
                for (ObjectList<std::string>::iterator it = pattern_array_list[i].begin();
                     it != pattern_array_list[i].end(); ++it)
                    std::cout << *it << " | ";
                std::cout << "\n";
            }
            else if (idx != -1 && inout_pattern == "out")
            {
                // TODO: out_pattern 
                kernel_info->_var_info[idx]._out_pattern_type = pattern_type_list[i];
                kernel_info->_var_info[idx]._out_pattern_array = pattern_array_list[i];
                
                // Debug print
                std::cout << "IN PATTERN FOR " << var_list[i] << " is " 
                    << ((pattern_type_list[i] == GFN_PATTERN_RANGE)? "RANGE" : "SPEC")
                    << " and value are ";
                for (ObjectList<std::string>::iterator it = pattern_array_list[i].begin();
                     it != pattern_array_list[i].end(); ++it)
                    std::cout << *it << " | ";
                std::cout << "\n";
            }
            else
            {
                std::cerr << "warning : clause \"" << "in_pattern" /* TODO: */
                          << "\" unknown variable \"" << var_list[i]
                          << "\" in parallel region " << std::endl;
            }
        }
    }
}

void GFNPragmaPhase::find_use_and_def_list(TL::Statement compound_stmt,
                                           KernelInfo *kernel_info)
{

    /*if (!compound_stmt.is_compound_statement())
    {
        std::cerr
            << "Error at gfn-parallel_for.cpp:find_use_and_def_list\n";
        return;
    }*/

    ObjectList<Statement> statements;
    if (compound_stmt.is_compound_statement())
    {
        statements = compound_stmt.get_inner_statements();
    }
    else
    {
        statements.push_back(compound_stmt);
    }

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

void GFNPragmaPhase::collect_variable_info(Statement stmt,
                                           KernelInfo *kernel_info)
{
    if (stmt.is_declaration())
    {

    }
    else if (ForStatement::predicate(stmt.get_ast()))
    {
        TL::ForStatement for_stmt = ForStatement(stmt.get_ast(), stmt.get_scope_link());
        // TODO: init, cond, incre ?
        collect_variable_info(for_stmt.get_loop_body(), kernel_info);
    }
    else if (stmt.is_compound_statement())
    {
        ObjectList<Statement> statements = stmt.get_inner_statements();
        for (ObjectList<Statement>::iterator it = statements.begin();
             it != statements.end();
             ++it)
        {
            collect_variable_info(*it, kernel_info);
        }
    }
    else if (stmt.is_expression())
    {
        collect_variable_info(stmt.get_expression(), kernel_info);
    }
}

void GFNPragmaPhase::collect_variable_info(Expression expr,
                                           KernelInfo *kernel_info,
                                           int found_idx_at)
{
    if (expr.is_id_expression())
    {
        IdExpression id_expr = expr.get_id_expression();
        Symbol sym = id_expr.get_symbol();

        if (found_idx_at >= 0)
        {
            /*std::cout << "Found at index : " << found_idx_at
                      << " for " << sym.get_name() << "\n";*/
            int idx = kernel_info->get_var_info_index_from_var_name(sym.get_name());

            if (kernel_info->_var_info[idx]._shared_dimension >= 0)
            {
                std::cerr << "Conflict at " << __FILE__ << ":" << __LINE__ << "\n";
            }

            kernel_info->_var_info[idx]._shared_dimension = found_idx_at;
        }

        // Traverse child nodes
        // TODO: collect_variable_info(, kernel_info); ??
    }
    else if (expr.is_array_subscript())
    {
        //std::cout << "Subscript : " << expr.get_subscript_expression() << "\n";
        //std::cout << "Subscripted : " << expr.get_subscripted_expression() << "\n";

        if (found_idx_at < 0)
        {
            bool found_idx = false;
            TL::Expression subscript = expr.get_subscript_expression();
            ObjectList<IdExpression> vars_in_subscript = subscript.all_symbol_occurrences(Expression::ONLY_VARIABLES);
            for (ObjectList<IdExpression>::iterator it = vars_in_subscript.begin();
                 it != vars_in_subscript.end();
                 ++it)
            {
                Symbol sym = it->get_symbol();
                if (sym.get_name() == kernel_info->loop_index_var_name)
                {
                    found_idx = true;
                    found_idx_at = 0;
                    break;
                }
            }
        }
        else
        {
            found_idx_at++;
        }

        // Traverse child nodes
        collect_variable_info(expr.get_subscript_expression(), kernel_info);
        collect_variable_info(expr.get_subscripted_expression(), kernel_info, found_idx_at);
    }
    else if (expr.is_member_access() || expr.is_pointer_member_access())
    {
        // Traverse child nodes
        collect_variable_info(expr.get_accessed_entity(), kernel_info);
    }
    else if (expr.is_assignment())
    {
        // Traverse child nodes
        collect_variable_info(expr.get_first_operand(), kernel_info);
        collect_variable_info(expr.get_second_operand(), kernel_info);
    }
    else if (expr.is_operation_assignment())
    {
        // Traverse child nodes
        collect_variable_info(expr.get_first_operand(), kernel_info);
        collect_variable_info(expr.get_second_operand(), kernel_info);
    }
    else if (expr.is_binary_operation())
    {
        // Traverse child nodes
        collect_variable_info(expr.get_first_operand(), kernel_info);
        collect_variable_info(expr.get_second_operand(), kernel_info);
    }
    else if (expr.is_unary_operation())
    {
        Expression::OperationKind kind = expr.get_operation_kind();

        switch ((int)kind)
        {
            case Expression::PREINCREMENT:
            case Expression::PREDECREMENT:
            case Expression::POSTINCREMENT:
            case Expression::POSTDECREMENT:
            case Expression::MINUS :
            case Expression::BITWISE_NOT :
            case Expression::LOGICAL_NOT :
            case Expression::DERREFERENCE :
            default:
                break;
        }

        // Traverse child nodes
        collect_variable_info(expr.get_unary_operand(), kernel_info);
    }
    else if (expr.is_conditional())
    {
        // Traverse child nodes
        collect_variable_info(expr.get_condition_expression(), kernel_info);
        collect_variable_info(expr.get_true_expression(), kernel_info);
        collect_variable_info(expr.get_false_expression(), kernel_info);
    }
    else if (expr.is_function_call())
    {
        // Traverse child nodes
        // TODO:
    }
    else if (expr.is_casting())
    {
        // Traverse child nodes
        collect_variable_info(expr.get_casted_expression(), kernel_info);
    }
    else if (expr.is_constant())
    {
        // TODO: insert replace expression that want
    }
    else if (expr.is_literal())
    {
        // 0.5 4.0 is literal
        // TODO: insert replace expression that want
    }
    else
    {
        std::cerr << "Error in collect_variable_info, What type of this expr : "
                  << expr << "\n";
    }
}

void GFNPragmaPhase::collect_loop_info(TL::ForStatement for_stmt,
                                       KernelInfo *kernel_info)
{
    TL::Expression lower_bound_expr = for_stmt.get_lower_bound();
    TL::Expression upper_bound_expr = for_stmt.get_upper_bound();

    TL::IdExpression induction_var = for_stmt.get_induction_variable();
    TL::Symbol sym = induction_var.get_symbol();

    /* Find index variable and set flag */
    int idx_idx = kernel_info->get_var_info_index_from_var_name(sym.get_name());
    kernel_info->_var_info[idx_idx]._is_index = true;

    //if (upper_bound_expr.is_constant())
    {
        kernel_info->_is_const_loop_upper_bound = true;
        kernel_info->_const_upper_bound = (std::string)upper_bound_expr;
        //std::cout << "Upper bound expr : " << (std::string)upper_bound_expr << std::endl;
    }
}


EXPORT_PHASE(TL::GFN::GFNPragmaPhase)
