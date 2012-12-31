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
    
    register_construct("overlapcompute");
    on_directive_post["overlapcompute"].connect(functor(&GFNPragmaPhase::overlapcompute, *this));
    
    register_construct("barrier");
    on_directive_pre["barrier"].connect(functor(&GFNPragmaPhase::barrier, *this));
    
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
    find_use_and_def_list(loop_body, kernel_info);

    // get data from clauses
    get_kernelname_clause(construct, kernel_info);
    get_waitfor_clause(construct, kernel_info);
    get_private_clause(construct, kernel_info, symbol_list);
    get_accurate_clause(construct, kernel_info);
    get_reduction_clause(construct, kernel_info);
    get_size_clause(construct, kernel_info);
    get_input_clause(construct, kernel_info);
    get_output_clause(construct, kernel_info);
    get_parallel_if_clause(construct, kernel_info);

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

    // "MPI_Barrier(" << GFN_COMM << ");";
    // "__threadfence();"

    result
        << "_GfnBarrier();"
        << construct.get_statement(); // XXX: get startment of construct before replace

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
                        kernel_info->_var_info[k]._reduction_type = op_to_op_type(opr);
                    }
                }
            }
        }
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
                /* Set size to var info list */
                ObjectList<VariableInfo>::iterator viit = kernel_info->_var_info.begin();
                ObjectList<DataReference>::iterator vrit = kernel_info->_var_ref.begin();
                for (;viit != kernel_info->_var_info.end();
                     ++viit, ++vrit)
                {
                    if (*it == vrit->get_id_expression().get_symbol().get_name())
                    {
                        std::cout << "Size of " << *it << " = "
                                  << size_list[0] << ":"
                                  << size_list[1] << ":"
                                  << size_list[2] << std::endl;

                        viit->_size._dim1_size = size_list[0];
                        viit->_size._dim2_size = size_list[1];
                        viit->_size._dim3_size = size_list[2];
                    }
                }



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

void GFNPragmaPhase::get_copy_clause(TL::PragmaCustomClause &copy_clause,
                                     KernelInfo *kernel_info,
                                     std::string copy_type_str)
{
    if (copy_clause.is_defined())
    {
        ObjectList<std::string> var_list;

        ObjectList<std::string> list_arg = copy_clause.get_arguments();
        for (ObjectList<std::string>::iterator it = list_arg.begin();
             it != list_arg.end();
             ++it)
        {
            parse_var_list_and_append(*it, var_list);
        }

        for (ObjectList<std::string>::iterator it = var_list.begin();
             it != var_list.end();
             ++it)
        {
            int idx = kernel_info->get_var_info_index_from_var_name(*it);
            if (idx != -1)
            {
                //std::cout << "Found " << copy_type_str << " : " << *it << std::endl;
                if (copy_type_str == "input")
                    kernel_info->_var_info[idx]._is_input = true;
                else if (copy_type_str == "output")
                    kernel_info->_var_info[idx]._is_output = true;
                else
                    std::cerr << "warning : " << __LINE__
                              << " at " << __FILE__ << std::endl;
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

void GFNPragmaPhase::get_parallel_if_clause(TL::PragmaCustomConstruct construct,
                                            KernelInfo *kernel_info)
{
    TL::PragmaCustomClause parallel_if_clause = construct.get_clause("parallel_if");

    if (parallel_if_clause.is_defined())
    {
        ObjectList<std::string> list_arg = parallel_if_clause.get_arguments();

        if (list_arg.size() > 1)
        {
            std::cerr << "Error : multiple parallel_if clause\n";
        }

        kernel_info->_parallel_if_expr = list_arg[0];
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
