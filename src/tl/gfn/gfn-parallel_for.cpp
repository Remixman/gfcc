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



#include "gfn-parallel_for.hpp"
#include "gfn-config.hpp"
#include "gfn-common.hpp"
#include <sstream>

using namespace TL::GFN;

ParallelFor TL::GFN::parallel_for(PragmaCustomConstruct construct, 
                                  TL::ForStatement for_stmt, 
                                  KernelInfo *kernel_info)
{
    return ParallelFor(construct, for_stmt, kernel_info);
}


TL::Source ParallelFor::get_source()
{
    // Nothing at this moment
    return do_parallel_for();
}

ParallelFor::ParallelFor(PragmaCustomConstruct construct, 
                         ForStatement for_stmt, 
                         KernelInfo *kernel_info)
     : _construct(construct), _for_stmt(for_stmt), 
       _function_def(0), _kernel_info(kernel_info)
{
     ;
}

TL::Source ParallelFor::do_kernel_config(Expression &lower_bound,
                                         Expression &upper_bound,
                                         Expression &step)
{
    // calculate thread num [ ast-node.cxx : SetThreadNum() ]
    TL::Source range, thread_num, block_num, thread_p_block_num;
    {
        // range = (upper - lower)
        bool lower_bound_is_const = false;
        int lower_bound_val = lower_bound.evaluate_constant_int_expression(lower_bound_is_const);
        if (lower_bound_is_const && (lower_bound_val == 0)) {
            range
                << "( " << upper_bound.prettyprint() << " )";
        } else {
            range
                << "( " << upper_bound.prettyprint() << " - " << lower_bound.prettyprint() << " )";
        }

        // thread_num = (range / step) + 1
        bool step_is_const = false;
        int step_val = step.evaluate_constant_int_expression(step_is_const);
        if (step_is_const && (step_val == 1)) {
            thread_num
                << "( " << range << " + 1 )";
        } else {
            thread_num
                << "( ( " << range << " / " << step.prettyprint() << " ) + 1 )";
        }

        // TODO: reduce expr?
        bool valid = true;
        /*if (!expr.is_constant())
        {
            throw HLTException(expr, "factor clause argument should be a constant expression");
        }
        thread_num.parse_expression(_for_stmt.get_ast(), _for_stmt.get_scope_link()).get_expression();
        unroll_factor = expr.evaluate_constant_int_expression(valid);
        if (!valid)
        {
            throw HLTException(expr, "factor clause argument expression could not be evaluated");
        }*/
        
        
        if (0 /* thread_num is integer literal */) 
        {
            ;
        }
        else
        {
            // thread_p_block = 512
            thread_p_block_num
                << "512";
            // block_num = 
            block_num
                << "( ( " << thread_num << " - 1 ) + 512.0) / 512";
            // TODO: reduce expr??
        }
    }
    TL::Source new_config;
    new_config << "<<<" << block_num << "," << thread_p_block_num << ">>>" ;
    return new_config;
}

void ParallelFor::xxxx(TL::Statement stmt)
{
    if (stmt.is_compound_statement())
    {

        TL::ObjectList<TL::Statement> list = stmt.get_inner_statements();
        for (TL::ObjectList<TL::Statement>::iterator it = list.begin();
                it != list.end();
                it++)
        {
            xxxx(*it);
        }
    }
    else
    {

    }
}

TL::Source ParallelFor::do_parallel_for()
{
    if (!_for_stmt.regular_loop())
    {
        throw GFNException(_for_stmt, "support only simple for loop");
    }

    /* Store loop size variable, may be mutiple like
       for(i = 0; i < n + m; i++ ) */
    TL::ObjectList<IdExpression> loop_size_var_list;
    bool enable_opencl = true;

    // Core worker sources
    TL::Source worker_declare_variables_src;
    TL::Source worker_declare_generated_variables_src;
    TL::Source worker_boardcast_scalar_src;
    TL::Source worker_allocate_array_memory_src;
    TL::Source worker_initialize_generated_variables_src;
    TL::Source worker_distribute_array_memory_src;
    TL::Source worker_computing_workload_src;
    TL::Source worker_gather_array_memory_src;
    TL::Source worker_reduce_scalar_src;
    TL::Source worker_free_array_memory_src;
    
    // Core master sources
    TL::Source master_send_scalar_input, master_send_array_input;
    TL::Source master_recv_output;
    
    // C/C++ and MPI sources
    TL::Source worker_input_buffer_free;
    TL::Source worker_func_def, worker_recv_input, worker_send_output;
    TL::Source worker_root_free_gen_var;

    // MPI loop size sources
    TL::Source send_loop_size;
    TL::Source worker_recv_loop_size, worker_scatter_loop_size;

    // OpenCL sources
    TL::Source kernel_actual_param, kernel_formal_param;
    TL::Source cl_var_decl, cl_init_var, cl_free_var;
    TL::Source cl_create_kernel, cl_set_kernel_arg;
    TL::Source cl_kernel_src_str, cl_kernel, cl_launch_kernel, cl_release_kernel;
    TL::Source cl_decl_init_work_item_var;
    TL::Source cl_kernel_body, cl_finalize;
    TL::Source cl_actual_params, cl_kernel_var_decl;
    TL::Source cl_kernel_var_init;
    int kernel_arg_num = 0;

    // OpenCL reduction sources
    TL::Source cl_kernel_reduce_init_if, cl_kernel_reduce_init_else;
    TL::Source cl_kernel_reduce_local_reduce;
    TL::Source cl_kernel_reduce_global_init, cl_kernel_reduce_global_reduce;

    //
    TL::Source mpi_block_dist_for_stmt; // TODO : other for stmt


    /* _function_def is function that call for_stmt */
    _function_def = new FunctionDefinition(_for_stmt.get_enclosing_function());

    /* Get parts of the loop */
    IdExpression induction_var = _for_stmt.get_induction_variable();
    Expression lower_bound = _for_stmt.get_lower_bound();
    Expression upper_bound = _for_stmt.get_upper_bound();
    Expression step = _for_stmt.get_step();
    std::string loop_start = lower_bound.prettyprint();
    std::string loop_end = upper_bound.prettyprint();
    std::string loop_step = (std::string)step;
    std::string full_size = _kernel_info->get_full_size(); // return empty if don't have partitioned shared array
    //std::string bound_opr = (std::string)_for_stmt.get_bound_operator();
    std::string loop_iter_size;
    TL::Statement original_loop_body = _for_stmt.get_loop_body();
    // Copy loop body for cluster level and GPU level
    TL::Statement cluster_loop_body(original_loop_body);
    TL::Statement gpu_loop_body(original_loop_body);

    std::string induction_var_name = (std::string)induction_var;
    std::string new_induction_var_name = GFN_PREFIX_LOCAL + induction_var_name;
    
    std::string level1_cond = _kernel_info->_level_1_condition;
    std::string level2_cond = _kernel_info->_level_2_condition;

    /*== ---------- Create source about loop size ------------------==*/
    loop_size_var_list.append( lower_bound.all_symbol_occurrences(TL::Statement::ONLY_VARIABLES) );
    loop_size_var_list.append( upper_bound.all_symbol_occurrences(TL::Statement::ONLY_VARIABLES) );
    for (int i = 0; i < loop_size_var_list.size(); ++i)
    {
        DataReference data_ref(loop_size_var_list[i].get_expression());
        std::string var_name = data_ref.get_symbol().get_name();
        TL::Type type = data_ref.get_type();
        std::string c_type_str = type_to_ctype(type);
        std::string mpi_type_str = type_to_mpi_type(type);

        if (!type.is_scalar_type())
        {
            std::cerr << "Error : Variable in loop expression must be scalar\n";
        }

        master_send_scalar_input
            << "_SendInputMsg((void *)&" << var_name
            << ", sizeof (" << c_type_str << "));";

        worker_boardcast_scalar_src
            << create_gfn_q_bcast_scalar(var_name, mpi_type_str);
            
        TL::Source cl_actual_param;
                cl_actual_param
                    << c_type_str << " " << var_name;
                cl_actual_params.append_with_separator(cl_actual_param, ",");
            
        cl_set_kernel_arg
            << create_cl_set_kernel_arg("_kernel", kernel_arg_num++, c_type_str, var_name);
            
        // set is_loop_variable flag
        int idx = _kernel_info->get_var_info_index_from_var_name(var_name);
        if (idx >= 0) _kernel_info->_var_info[idx]._is_loop_variable = true;
    }

    /*== ----- Create MPI block distribution for statement ---------==*/
    std::string local_start_idx_var = GFN_PREFIX_LOCAL + induction_var_name + "_start";
    std::string local_end_idx_var = GFN_PREFIX_LOCAL + induction_var_name + "_end";
    std::string loop_step_var = "_loop_step";
    std::string cl_loop_step_var = "_cl_loop_step";
    std::string loop_size_var = "_loop_size";
    if (full_size != "")
    {
        worker_declare_generated_variables_src
            << "int _local_data_start, _local_data_end;";
    }
    worker_declare_generated_variables_src
        << "int " << local_start_idx_var << "," << local_end_idx_var << ";"
        << "int " << loop_step_var << ";"
        << "int " << loop_size_var << ";"
        << "int " << new_induction_var_name << " = 0;"
        << "size_t _work_item_num, _work_group_item_num, _global_item_num;"
        << "cl_kernel _kernel;";
    if (full_size != "")
    {
        worker_initialize_generated_variables_src
            << "_local_data_start = _GfnCalcLocalDataStart(0, (" << full_size << ")-1);"
            << "_local_data_end = _GfnCalcLocalDataEnd(0, (" << full_size << ")-1);"
            << local_start_idx_var << " = _GfnCalcLocalLoopStart(_local_data_start, "
            << loop_start << ", " << loop_step << ");"
            << local_end_idx_var << " = _GfnCalcLocalLoopEnd(_local_data_end, "
            << loop_end << ");";
    }
    else
    {
        worker_initialize_generated_variables_src
            << local_start_idx_var << " = _CalcLocalStartIndex("
            << loop_start << ","
            << loop_end << ",_gfn_num_proc,_gfn_rank+1);"
            << local_end_idx_var << " = _CalcLocalEndIndex("
            << loop_start << ","
            << loop_end << ",_gfn_num_proc,_gfn_rank+1);";
    }
    worker_initialize_generated_variables_src
        << loop_size_var << " = _CalcLoopSize("
        << loop_start << ","
        << loop_end << ","
        << step.prettyprint() << ");"
        << loop_step_var << " = " << loop_step << ";";
    worker_initialize_generated_variables_src
        << "_work_item_num = _CalcSubSize(_loop_size, _gfn_num_proc, _gfn_rank, 1);"
        << "_work_group_item_num = 64;"
        << "_global_item_num = _GfnCalcGlobalItemNum(_work_item_num, _work_group_item_num);";


    // XXX: we indicate with only step symbol
    bool is_incre_loop = (loop_step[0] == '-')? false : true;
    if (is_incre_loop)
    {
        loop_iter_size = "(" + loop_end + "-"
                + loop_start + "+1)";
    }
    else
    {
        loop_iter_size = "(" + loop_start + "-"
                + loop_end + "+1)";
    }

    /* Sort variable in kernel (sort by size) */
    _kernel_info->sort_var_info();

    /* Replace with call function */
    TL::Source send_call_func;
    send_call_func
        << comment("Send call function message")
        << "_SendCallFuncMsg(" << int_to_string(_kernel_info->kernel_id) << ");";

    for (int i = 0; i < _kernel_info->_var_info.size(); ++i)
    {
        VariableInfo var_info = _kernel_info->_var_info[i];
        DataReference var_ref = _kernel_info->_var_ref[i];

        std::string var_name = var_info._name;
        std::string var_unique_id_name = "_id_" + var_name;
        std::string var_cl_name = "_cl_mem_" + var_name;
        std::string var_cl_global_reduction = "_global_reduction_" + var_name;
        std::string var_local_name = GFN_PREFIX_LOCAL + var_name;
        std::string var_cl_local_mem_name = "_cl_local_mem_" + var_name;
        std::string ptr_stars = var_info.get_pointer_starts();
        bool is_partition = (var_info._shared_dimension != 0);

        // TODO: _GFN_MEM_ALLOC_HOST_PTR()
        std::string var_cl_mem_type;
        if (var_info._is_input && var_info._is_output)
            var_cl_mem_type = "_GFN_MEM_READ_WRITE()";
        else if (var_info._is_output)
            var_cl_mem_type = "_GFN_MEM_WRITE_ONLY()";
        else if (var_info._is_input)
            var_cl_mem_type = "_GFN_MEM_READ_ONLY()";
        /* TODO: temp var type ?? _GFN_MEM_READ_WRITE?? or local */
        
        std::string in_pattern_name = "0"; // NULL
        std::string out_pattern_name = "0"; // NULL
        int in_pattern_type = var_info._in_pattern_type;
        int out_pattern_type = var_info._out_pattern_type;
        if (in_pattern_type != GFN_PATTERN_NONE)
            in_pattern_name = "_pattern_in_" + var_name;
        if (out_pattern_type != GFN_PATTERN_NONE)
            out_pattern_name = "_pattern_out_" + var_name;
        TL::ObjectList<std::string> &in_pattern_array = var_info._in_pattern_array;
        TL::ObjectList<std::string> &out_pattern_array = var_info._out_pattern_array;

        TL::Type type = var_ref.get_type();
        std::string mpi_type_str = type_to_mpi_type(type);
        std::string c_type_str = type_to_ctype(type);
        std::string size_str = var_info.get_allocate_size_in_byte(type);
        //std::cout << "Type of " << var_info._name << " is " << c_type_str << std::endl;

        /* 1. Declaration necessary variable */
        if (var_info._is_array_or_pointer)
        {
            worker_declare_variables_src
                << c_type_str << ptr_stars << var_name << ";";
        }
        /*else if (var_info._is_reduction)
        {
            worker_var_decl
                << c_type_str << " " << var_local_name << " = "
                << reduction_op_init_value(var_info._reduction_type) << ";";
        }*/
        else
        {
            worker_declare_variables_src
                << var_ref.get_type().get_declaration(var_ref.get_scope(), var_name) << ";";

            if (!var_info._is_index && 
                !var_info._is_input /* Scalar input is function parameters, not need to declare again */)
            {
                cl_kernel_var_decl
                    << var_ref.get_type().get_declaration(var_ref.get_scope(), var_name) << ";\n";
            }
        }


        /* 2. Declaration generated variable for each variable */
        if (var_info._is_input || var_info._is_output)
        {
            if (var_info._is_array_or_pointer)
            {
                worker_declare_generated_variables_src
                    << "long long " << var_unique_id_name << ";";
            }
            
            if (in_pattern_type != GFN_PATTERN_NONE)
            {
                worker_declare_generated_variables_src
                    << "int " << in_pattern_name << "[" << in_pattern_array.size() << "];";
                    
                for (int p = 0; p < in_pattern_array.size(); ++p)
                {
                    worker_initialize_generated_variables_src
                        << in_pattern_name << "[" << p << "] = "
                        << in_pattern_array[p] << ";";
                }
            }
            
            if (out_pattern_type != GFN_PATTERN_NONE)
            {
                worker_declare_generated_variables_src
                    << "int " << out_pattern_name << "[" << out_pattern_array.size() << "];";
                    
                for (int p = 0; p < out_pattern_array.size(); ++p)
                {
                    worker_initialize_generated_variables_src
                        << out_pattern_name << "[" << p << "] = "
                        << out_pattern_array[p] << ";";
                }
            }
            
            // About OpenCL parameter code
            if (var_info._is_array_or_pointer || var_info._is_output)
            {
                worker_declare_generated_variables_src
                    << "cl_mem " << var_cl_name << " = 0;";

                cl_set_kernel_arg
                    << create_cl_set_kernel_arg("_kernel", kernel_arg_num++, "cl_mem", var_cl_name);

                TL::Source cl_actual_param;
                if (var_info._is_reduction)
                {
                    cl_actual_param
                        << "__global " << c_type_str << " * "
                        << var_cl_global_reduction;
                }
                else
                {
                    cl_actual_param
                        << "__global " << ((!var_info._is_output)? "const " : "")
                        << c_type_str << " * " << var_name;
                }
                cl_actual_params.append_with_separator(cl_actual_param, ",");

                TL::Source actual_param;
                actual_param << c_type_str << " * " << var_name;
                TL::Source formal_param;
                formal_param << GFN_DEVICE_PREFIX << var_name;
                kernel_actual_param.append_with_separator(actual_param, ",");
                kernel_formal_param.append_with_separator(formal_param, ",");
            }
            else if (var_info._is_input && !var_info._is_loop_variable)
            {
                TL::Source cl_actual_param;
                cl_actual_param
                    << c_type_str << " " << var_name;
                cl_actual_params.append_with_separator(cl_actual_param, ",");
                
                cl_set_kernel_arg
                    << create_cl_set_kernel_arg("_kernel", kernel_arg_num++, c_type_str, var_name);
            }
        }
        
        /* 3. Allocate memory for array or pointer */
        if ((var_info._is_input || var_info._is_output) && var_info._is_array_or_pointer)
        {
            master_send_scalar_input
                << "_SendConstInputMsg((long long)&" << var_name << ");";

            worker_boardcast_scalar_src
                << create_gfn_q_bcast_scalar(var_unique_id_name, "_GFN_TYPE_LONG_LONG_INT()");
            
            worker_allocate_array_memory_src
                << create_gfn_malloc_nd(var_name, var_cl_name, var_unique_id_name,
                                        mpi_type_str, var_info._dimension_num, var_info._dim_size,
                                        var_cl_mem_type, level1_cond, level2_cond);
                
            worker_free_array_memory_src
                << create_gfn_free(var_unique_id_name, level1_cond, level2_cond);
        }

        if (var_info._is_input)
        {
            /* Master code */
            if (is_partition /* also be array */)
            {
                master_send_array_input
                    << create_send_input_nd_msg(var_name, mpi_type_str, loop_start, loop_end, loop_step,
                                                var_info._shared_dimension, in_pattern_type, 
                                                var_info._dimension_num, in_pattern_array.size(),
                                                var_info._dim_size, in_pattern_array);
            }
            else if (var_info._is_array_or_pointer)
            {
                master_send_array_input
                    << "_SendInputMsg((void*)" << var_name 
                    << var_info.get_subscript_to_1d_buf() << "," << size_str << ");";
            }
            else
            {
                master_send_scalar_input
                    << "_SendInputMsg((void*)&" << var_name << "," << size_str << ");";
            }

            /* Worker code */
            if (var_info._is_array_or_pointer && is_partition)
            {
                worker_distribute_array_memory_src
                    << create_gfn_q_scatter_nd(var_name, var_cl_name, mpi_type_str, 
                                               loop_start, loop_end, loop_step, var_info._dimension_num,
                                               var_info._dim_size, var_info._shared_dimension, 
                                               var_cl_mem_type, in_pattern_name, in_pattern_array.size(),
                                               in_pattern_type, level1_cond, level2_cond);
            }
            else if (var_info._is_array_or_pointer)
            {
                worker_distribute_array_memory_src
                    << create_gfn_q_bcast_nd(var_name, var_cl_name, mpi_type_str, 
                                             var_info._dimension_num, var_info._dim_size,
                                             level1_cond, level2_cond);
            }
            else
            {
                worker_boardcast_scalar_src
                    << create_gfn_q_bcast_scalar(var_name, mpi_type_str);
            }
        }

        if (var_info._is_output)
        {
            /* Master code */
            if (is_partition)
            {
                master_recv_output
                    << create_recv_output_nd_msg(var_name, mpi_type_str, loop_start, loop_end, loop_step,
                                                var_info._shared_dimension, out_pattern_type,
                                                var_info._dimension_num, out_pattern_array.size(),
                                                var_info._dim_size, out_pattern_array);
            }
            else
            {
                master_recv_output
                    << "_RecvOutputMsg((void*)" << ((var_info._is_array_or_pointer)? "" : "&")
                    << var_name << var_info.get_subscript_to_1d_buf() << "," << size_str << ");";
            }
            
            /* Worker code */
            if (var_info._is_array_or_pointer && is_partition)
            {
                worker_gather_array_memory_src
                    << create_gfn_q_gather_nd(var_name, var_cl_name, mpi_type_str, 
                                              loop_start, loop_end, loop_step, var_info._dimension_num, 
                                              var_info._dim_size, var_info._shared_dimension, 
                                              var_cl_mem_type, out_pattern_name, out_pattern_array.size(),
                                              out_pattern_type, level1_cond, level2_cond);
            }
            else if (var_info._is_array_or_pointer)
            {
                std::cerr << "Error: try to gather non-parition array " << var_name << "\n";
            }
            else
            {
                worker_reduce_scalar_src
                    << create_gfn_q_reduce_scalar(var_name, var_cl_name, mpi_type_str, 
                                                  op_to_mpi_op(var_info._reduction_type), level1_cond, level2_cond);
            }
        }

        if (var_info._is_reduction)
        {
            worker_allocate_array_memory_src
                << create_gfn_malloc_reduce(var_name, var_cl_name, mpi_type_str,
                                            level1_cond, level2_cond);

            worker_free_array_memory_src
                << create_gfn_free_reduce(var_cl_name, level1_cond, level2_cond);
            
            cl_kernel_var_init
                << var_name << " = "
                << reduction_op_init_value(var_info._reduction_type) << ";" << CL_EOL;

            cl_kernel_reduce_init_if
                << var_cl_local_mem_name << "[get_local_id(0)] = "
                << var_name << ";" << CL_EOL;

            cl_kernel_reduce_init_else
                << var_cl_local_mem_name << "[get_local_id(0)] = "
                << reduction_op_init_value(var_info._reduction_type) << ";" << CL_EOL;

            cl_kernel_reduce_local_reduce
                << var_cl_local_mem_name << "[get_local_id(0)] += "
                << var_cl_local_mem_name << "[get_local_id(0)+_stride];" << CL_EOL;

            cl_kernel_reduce_global_init
                << "*" << var_cl_global_reduction << " = "
                << reduction_op_init_value(var_info._reduction_type) << ";" << CL_EOL;

            cl_kernel_reduce_global_reduce
                << create_cl_help_atomic_call(var_cl_global_reduction, var_cl_local_mem_name,
                                              var_info._reduction_type, type);

            cl_set_kernel_arg
                << create_cl_set_kernel_arg("_kernel", kernel_arg_num++, c_type_str, "0");

            TL::Source cl_actual_param;
            cl_actual_param
                << "__local " << c_type_str << " * " << var_cl_local_mem_name;
            cl_actual_params.append_with_separator(cl_actual_param, ",");
        }
    }
    
    /*== ----- Create MPI block distribution for statement ---------==*/
    //std::vector<bool> mpi_replace_types(GFN_REPLACE_LAST_TYPE, false);
    //mpi_replace_types[GFN_REPLACE_ARRAY_ND] = false;
    //mpi_replace_types[GFN_REPLACE_ARRAY_INDEX] = false;
    //replace_parallel_loop_body(cluster_loop_body, mpi_replace_types, induction_var_name, new_induction_var_name);
    //std::cout << "Create MPI Block\n";
    mpi_block_dist_for_stmt
        << "for(" << induction_var.prettyprint() << " = "
        << local_start_idx_var << ";" << induction_var.prettyprint()
        << ((is_incre_loop)? "<=" : ">=") << local_end_idx_var << ";"
        << _for_stmt.get_iterating_expression() << ", "
        << new_induction_var_name << "+=_loop_step)" << cluster_loop_body;
        
    /*== --------------- Replace code in loop body -----------------==*/
    std::vector<bool> cl_replace_types(GFN_REPLACE_LAST_TYPE, false);
    cl_replace_types[GFN_REPLACE_ARRAY_ND] = true;
    cl_replace_types[GFN_REPLACE_ARRAY_INDEX] = false;
    replace_parallel_loop_body(gpu_loop_body, cl_replace_types, induction_var_name, new_induction_var_name);

    // Add new start and end index to kernel argument, e.g. local_i_start, local_i_end
    cl_set_kernel_arg
        << create_cl_set_kernel_arg("_kernel", kernel_arg_num++, "int", local_start_idx_var);
    cl_set_kernel_arg
        << create_cl_set_kernel_arg("_kernel", kernel_arg_num++, "int", local_end_idx_var);
    cl_set_kernel_arg
        << create_cl_set_kernel_arg("_kernel", kernel_arg_num++, "int", loop_step_var);

    // TODO: Refactor this
    TL::Source tmp_src_1;
    tmp_src_1 << "int " << local_start_idx_var;
    cl_actual_params.append_with_separator(tmp_src_1, ",");
    TL::Source tmp_src_2;
    tmp_src_2 << "int " << local_end_idx_var;
    cl_actual_params.append_with_separator(tmp_src_2, ",");
    TL::Source tmp_src_3;
    tmp_src_3 << "int " << loop_step_var;
    cl_actual_params.append_with_separator(tmp_src_3, ",");

    cl_kernel_var_decl
        << "int " << loop_size_var << " = ("
            << local_end_idx_var << " - " << local_start_idx_var
            << ") / " << loop_step_var << ";" << CL_EOL
        //<< "int _thread_id_dim_1 = get_global_id(1);"
        //<< "int _thread_id_dim_2 = get_global_id(2);"
        << "int " << new_induction_var_name << " = "
        << "get_global_id(0) * " << loop_step_var << ";" << CL_EOL
        << "int " << induction_var << " = " << new_induction_var_name 
        <<" + " << local_start_idx_var << ";" << CL_EOL;
    // Kernel helper function
    cl_kernel
        << create_cl_ext_pragma()
        << create_cl_help_barrier() // TODO: insert if use
        << create_cl_help_atomic_add_int() // TODO: insert if use
        << create_cl_help_atomic_add_float() // TODO: insert if use
        << create_cl_help_atomic_add_double(); // TODO: insert if use

    // Kernel reduction implement
    TL::Source cl_kernel_reduction;
    if (_kernel_info->_has_reduction_clause)
    {
        cl_kernel_reduction
            // [Reduction Step] -
            << "if (get_global_id(0) < " << loop_size_var << ") {" << CL_EOL
            << cl_kernel_reduce_init_if
            << "} else {" << CL_EOL
            << cl_kernel_reduce_init_else
            << "}" << CL_EOL
            // [Reduction Step] -
            << "for (int _stride = get_local_size(0)/2; _stride > 0; _stride /= 2) {" << CL_EOL
            << "barrier(CLK_LOCAL_MEM_FENCE);" << CL_EOL
            << "if (get_local_id(0) < _stride) {" << CL_EOL
            << cl_kernel_reduce_local_reduce
            << "}" << CL_EOL
            << "}" << CL_EOL
            // [Reduction Step] - Initialize global sum
            << "if (get_global_id(0) == 0) {" << CL_EOL
            << cl_kernel_reduce_global_init
            << "}" << CL_EOL 
            << "barrier(CLK_GLOBAL_MEM_FENCE);" << CL_EOL
            // [Reduction Step] - Set subsum of local memory to global memory
            << "if (get_local_id(0) == 0) {" << CL_EOL
            << cl_kernel_reduce_global_reduce
            << "}" << CL_EOL;
    }

    // Kernel main funcion
    cl_kernel
        << "__kernel void _kernel_" << int_to_string(_kernel_info->kernel_id)
        << "(" << cl_actual_params << ") {" << CL_EOL
        << cl_kernel_var_decl
        << cl_kernel_var_init
            << "if (" << induction_var << " <= "
            << ((_kernel_info->_is_const_loop_upper_bound)? _kernel_info->_const_upper_bound : "ERRORVAR" ) << ") {" << CL_EOL
                << gpu_loop_body << CL_EOL
            << "}" << CL_EOL
        << cl_kernel_reduction
        << "}" << CL_EOL;

    cl_kernel_src_str
        << show_cl_source_in_comment(cl_kernel)
        << "const char *_kernel_" << int_to_string(_kernel_info->kernel_id) << "_src = "
        << source_to_kernel_str(cl_kernel) << ";";
    cl_create_kernel
        << "_kernel = _GfnCreateKernel(\"_kernel_"
        << int_to_string(_kernel_info->kernel_id) << "\",_kernel_"
        << int_to_string(_kernel_info->kernel_id) << "_src,_gfn_context,_gfn_device_id);";
    cl_release_kernel
        << "_GfnClearKernel(_kernel);";


    /*== -------------  Create GPU kernel function -----------------==*/
    TL::Source result, device_var_decl, memcpy_h2d, memcpy_d2h;
    TL::Source new_param_list;
    std::string kernel_name = get_new_kernel_name();

    // declare thread id variable
    TL::Source thread_id_decl = do_thread_id_declaration();
    TL::Source loop_index_decl = do_loop_index_declaration(induction_var.get_symbol(),
                                                           step,
                                                           lower_bound);

    cl_launch_kernel
        << "_gfn_status = "
        << create_cl_enqueue_nd_range_kernel("_gfn_cmd_queue", "_kernel", "1", "0",
            "&_global_item_num", "&_work_group_item_num", "0", "0", "0")
        << create_gfn_check_cl_status("_gfn_status", "LAUNCH KERNEL");


    /*==----------------  Create worker function -------------------==*/
    worker_func_def
        << comment("*/ #ifdef GFN_WORKER /*")
            /* GPU */ << cl_kernel_src_str
        << "void _Function_" << int_to_string(_kernel_info->kernel_id) << " () {"
        
            << comment("Declare Variables")
            << worker_declare_variables_src
        
            << comment("Declare Generated Variables")
            << worker_declare_generated_variables_src
        
            << comment("Boardcast Scalar Value")
            << worker_boardcast_scalar_src
            << create_gfn_f_bcast_scalar()
            
            << comment("Allocate Array Memory")
            << worker_allocate_array_memory_src
            
            << comment("Initialize Generated Variables")
            << worker_initialize_generated_variables_src
            
            << comment("Distribute Array Memory")
            << worker_distribute_array_memory_src
            << create_gfn_f_dist_array()
            
            << comment("Compute Workload")
            // GPU or GPU Cluster (depend on level1 condition)
            << "if (" << level2_cond << ") {"
                << cl_create_kernel
                << cl_set_kernel_arg
                << cl_launch_kernel
                << cl_release_kernel
            // Cluster only
            << "} else {"
                << mpi_block_dist_for_stmt
            << "}"
            
            << comment("Gather Array Memory")
            << worker_gather_array_memory_src
            << create_gfn_f_gather_array()
            
            << comment("Reduce Scalar Value")
            << worker_reduce_scalar_src
            << create_gfn_f_reduce_scalar()
            
            << comment("Deallocate Array Memory")
            << worker_free_array_memory_src
        << "}"
        << comment("*/ #endif /*");

    /*std::cout << " ================= Worker Function ================\n";
    std::cout << (std::string) worker_func_def << "\n";
    std::cout << " ==================================================\n";*/

    TL::AST_t worker_func_tree = worker_func_def.parse_declaration(
            _function_def->get_point_of_declaration(),
            _function_def->get_scope_link());
    _function_def->get_ast().prepend_sibling_function(worker_func_tree);


    send_call_func
        << send_loop_size
        << master_send_scalar_input
        << master_send_array_input
        << master_recv_output;

    return send_call_func;
}

#if 0
void ParallelFor::extract_define_device_function(Statement compound_stmt) {
    Statement function_body = gpu_function.get_enclosing_statement();
    
    ObjectList<IdExpression> symbol_list = function_body.all_symbol_occurrences(TL::Statement::ONLY_FUNCTIONS);
    OjjectList<TL::AST_t> device_list;
    for (ObjectList<IdExpression>::iterator it = symbol_list.begin();
         it != symbol_list.end();
         it++)
    {
        if (0/* prefix is GFN_GEN_PREFIX or have definition in gpu eg. sin cos tan*/)
        {
            // DO NOTHING
        }
        else if (1/*can find definition*/)
        {
            FunctionDefinition func_def = it->get_enclosing_function();
            std::cout << func_def.prettyprint() << std::endl;
            // TODO: define as __device__
            TL::Source device_func_src;
            //device_func_src
                //<< "__device__"
                //<< 
            // TODO: change function name to gfn generate in function call and definition
            // TODO: check that it is not recirsive
            
            TL::AST_t device_func_tree = device_func_src.parse_declaration(
                gpu_function->get_point_of_declaration(),
                gpu_function->get_scope_link());
            
            gpu_function.prepend_sibling_function(device_func_tree);
            device_list.push_back(device_func_tree);
        }
        else
        {
            throw GFNException("Cannot found definition of '%s' function.", it->prettyprint());
        }
    }
    
    FunctionDefinition (AST_t ref, ScopeLink scope_link)
    
    // do it recursively
    for (ObjectList<TL::AST_t>::iterator it = device_list.begin();
         it != device_list.end();
         it++)
    {
        extract_define_device_function(*it);
    }
}
#endif

// Replace function
void ParallelFor::replace_parallel_loop_body(Statement stmt,
                                             std::vector<bool> &replace_types,
                                             std::string old_idx_name,
											 std::string new_idx_name)
{
    if (stmt.is_declaration())
    {
    }
    else if (ForStatement::predicate(stmt.get_ast()))
    {
        TL::ForStatement for_stmt = ForStatement(stmt.get_ast(), stmt.get_scope_link());
        // TODO: init, cond, incre ?
        replace_parallel_loop_body(for_stmt.get_loop_body(), replace_types, old_idx_name, new_idx_name);
    }/*
    else if (IfStatement::predicate(stmt.get_ast()))
    {

    }*/
    else if (stmt.is_compound_statement())
    {
        ObjectList<Statement> statements = stmt.get_inner_statements();
        for (ObjectList<Statement>::iterator it = statements.begin();
             it != statements.end();
             ++it)
        {
            replace_parallel_loop_body(*it, replace_types, old_idx_name, new_idx_name);
        }
    }
    else if (stmt.is_expression())
    {
        replace_parallel_loop_body(stmt.get_expression(), replace_types, old_idx_name, new_idx_name);
    }
    else
    {
        std::cerr << "Error in replace_parallel_loop_body, What type of this stmt : "
                  << stmt << "\n";
    }
}

void ParallelFor::replace_parallel_loop_body(Expression expr,
                                             std::vector<bool> &replace_types,
											 std::string old_idx_name,
											 std::string new_idx_name)
{
    if (expr.is_id_expression())
    {
        // Traverse child nodes
        // TODO: collect_variable_info(, kernel_info); ??
    }
    else if (expr.is_array_subscript())
    {
        // XXX: replace multi-dimension array to 1D array
        if (replace_types[GFN_REPLACE_ARRAY_ND] == true)
        {
            Expression tmp_expr = expr;
            std::string new_subscript = "";
            std::vector< std::string > subscript_list;
            int subscript_count = 0;

            // Counts subscript
            while (!tmp_expr.is_id_expression())
            {
                subscript_count++;
                Expression subscript_expr = tmp_expr.get_subscript_expression();
                if (replace_types[GFN_REPLACE_ARRAY_INDEX] == true)
                {
                    replace_loop_index_name(subscript_expr, old_idx_name, new_idx_name);
                }
                subscript_list.push_back( (std::string)subscript_expr );
                tmp_expr = tmp_expr.get_subscripted_expression();
            }

            std::string var_name = tmp_expr.get_id_expression().get_symbol().get_name();
            int idx = _kernel_info->get_var_info_index_from_var_name(var_name);
            VariableInfo var_info = _kernel_info->_var_info[idx];
            std::string dim1_size = _kernel_info->_var_info[idx]._dim_size[1];
            std::string dim2_size = _kernel_info->_var_info[idx]._dim_size[2];
            std::string dim3_size = _kernel_info->_var_info[idx]._dim_size[3];

            /*
             * Declare A[d1][d2]
             * Use A[i][j] => A[(i*d2)+j]
             * 
             * Declare A[d1][d2][d3]
             * Use A[i][j][k] => A[(i*d2*d3)+(j*d3)+k]
             * 
             */
            if (subscript_count >= 1)
            {
                new_subscript = "(" + subscript_list[0] + ")";
            }
            if (subscript_count >= 2)
            {
                // TODO: Now support only 2D and 3D
                std::string mul_size = (var_info._dimension_num == 2)? dim2_size : dim3_size;
                new_subscript = "((" + subscript_list[1] + ") * " +
                        mul_size + ") + " + new_subscript;
            }
            if (subscript_count >= 3)
            {
                new_subscript = "((" + subscript_list[2] + ") * " +
                        dim3_size + "*" + dim2_size + ") + " + new_subscript;
            }
            // TODO: multi dimensions array


            TL::Source new_array_expr;
            new_array_expr << var_name << "[" << new_subscript << "]";

            std::cout << "REPLACE {" << (std::string)expr << "} with {" << (std::string)new_array_expr << "}\n";

            // Replace
            AST_t new_array_expr_tree = new_array_expr.parse_expression(
                        expr.get_ast(), expr.get_scope_link());
            expr.get_ast().replace(new_array_expr_tree);
        }
        else if (replace_types[GFN_REPLACE_ARRAY_INDEX] == true)
        {
            Expression subscript_expr = expr.get_subscript_expression();
            replace_loop_index_name(subscript_expr, old_idx_name, new_idx_name);
            
            TL::Source new_array_expr;
            new_array_expr << subscript_expr;

            AST_t new_array_expr_tree = new_array_expr.parse_expression(
                        expr.get_ast(), expr.get_scope_link());
            expr.get_subscript_expression().get_ast().replace(new_array_expr_tree);
            
            replace_parallel_loop_body(expr.get_subscripted_expression(), replace_types, old_idx_name, new_idx_name);
        }
        
        // Traverse child nodes
        //replace_parallel_loop_body(expr.get_subscript_expression(), replace_types);
        //replace_parallel_loop_body(expr.get_subscripted_expression(), replace_types);
    }
    else if (expr.is_member_access() || expr.is_pointer_member_access())
    {
        // Traverse child nodes
        replace_parallel_loop_body(expr.get_accessed_entity(), replace_types, old_idx_name, new_idx_name);
    }
    else if (expr.is_assignment())
    {
        // Traverse child nodes
        replace_parallel_loop_body(expr.get_first_operand(), replace_types, old_idx_name, new_idx_name);
        replace_parallel_loop_body(expr.get_second_operand(), replace_types, old_idx_name, new_idx_name);
    }
    else if (expr.is_operation_assignment())
    {
        // Traverse child nodes
        replace_parallel_loop_body(expr.get_first_operand(), replace_types, old_idx_name, new_idx_name);
        replace_parallel_loop_body(expr.get_second_operand(), replace_types, old_idx_name, new_idx_name);
    }
    else if (expr.is_binary_operation())
    {
        // Traverse child nodes
        replace_parallel_loop_body(expr.get_first_operand(), replace_types, old_idx_name, new_idx_name);
        replace_parallel_loop_body(expr.get_second_operand(), replace_types, old_idx_name, new_idx_name);
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
        replace_parallel_loop_body(expr.get_unary_operand(), replace_types, old_idx_name, new_idx_name);
    }
    else if (expr.is_conditional())
    {
        // Traverse child nodes
        replace_parallel_loop_body(expr.get_condition_expression(), replace_types, old_idx_name, new_idx_name);
        replace_parallel_loop_body(expr.get_true_expression(), replace_types, old_idx_name, new_idx_name);
        replace_parallel_loop_body(expr.get_false_expression(), replace_types, old_idx_name, new_idx_name);
    }
    else if (expr.is_function_call())
    {
        // Traverse child nodes
        // TODO:
    }
    else if (expr.is_casting())
    {
        // Traverse child nodes
        replace_parallel_loop_body(expr.get_casted_expression(), replace_types, old_idx_name, new_idx_name);
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
        std::cerr << "Error in replace_parallel_loop_body, What type of this expr : "
                  << expr << "\n";
    }
}

void ParallelFor::replace_loop_index_name(Expression expr, 
										  std::string old_name, 
										  std::string new_name)
{
    if (expr.is_id_expression())
    {
        std::string var_name = expr.get_id_expression().get_symbol().get_name();
        if (var_name == old_name)
        {
        TL::Source new_name_expr;
            new_name_expr << new_name;

            std::cout << "REPLACE {" << (std::string)var_name << "} with {" << (std::string)new_name_expr << "}\n";

            // Replace
            AST_t new_name_expr_tree = new_name_expr.parse_expression(
                        expr.get_ast(), expr.get_scope_link());
            expr.get_ast().replace(new_name_expr_tree);
        }
    }
    else if (expr.is_array_subscript())
    {
        // TODO: SHOW ERROR : regular application ???

        // Traverse child nodes
        //replace_loop_index_name(expr.get_subscript_expression(), old_name, new_name);
        //replace_loop_index_name(expr.get_subscripted_expression(), old_name, new_name);
    }
    else if (expr.is_member_access() || expr.is_pointer_member_access())
    {
        // Traverse child nodes
        // TODO: ???
        //replace_loop_index_name(expr.get_accessed_entity(), old_name, new_name);
    }
    else if (expr.is_assignment())
    {
        // TODO: SHOW ERROR
    }
    else if (expr.is_operation_assignment())
    {
        // TODO: SHOW ERROR
    }
    else if (expr.is_binary_operation())
    {
        // Traverse child nodes
        replace_loop_index_name(expr.get_first_operand(), old_name, new_name);
        replace_loop_index_name(expr.get_second_operand(), old_name, new_name);
    }
    else if (expr.is_unary_operation())
    {
        // Traverse child nodes
        replace_loop_index_name(expr.get_unary_operand(), old_name, new_name);
    }
    else if (expr.is_conditional())
    {
        // TODO: SHOW ERROR
    }
    else
    {
        std::cout << "unknown type\n";
        // TODO:
    }
}

TL::Source ParallelFor::do_loop_index_declaration(TL::Symbol loop_index,
                                                  TL::Expression loop_increment,
                                                  TL::Expression loop_lowerbound)
{
    TL::Source index_declaration;
    Type type = loop_index.get_type();
    
    // TOOD: simplify expression
    index_declaration
        << comment("loop index entity");
    index_declaration
        << type.get_declaration(_construct.get_scope(), loop_index.get_name())
        << "="
        << "(" << GFN_THREAD_ID_VAR << "*" << loop_increment << ")"
        << "+" << loop_lowerbound << ";";

    return index_declaration;
}

TL::Source ParallelFor::do_thread_id_declaration()
{
    TL::Source thread_id_decl;
    
    thread_id_decl
        << comment("kernel thread id entity");
    
    if (0 /* block_num == 1 */)
    {
        thread_id_decl
            << "int " GFN_THREAD_ID_VAR " = threadIdx.x;";
    }
    else
    {
        thread_id_decl
            << "int " GFN_THREAD_ID_VAR " = blockIdx.x * blockDim.x + threadIdx.x;";
    }
    
    return thread_id_decl;
}

bool ParallelFor::contain(ObjectList<DataReference> &list, DataReference &obj)
{
    for (ObjectList<DataReference>::iterator it = list.begin();
         it != list.end();
         it++)
    {
        if (it->get_base_symbol().get_name() == obj.get_base_symbol().get_name())
        {
            return true;
        }
    }
    return false;
}
