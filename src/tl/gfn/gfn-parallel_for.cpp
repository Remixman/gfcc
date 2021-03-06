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
#include <src/tl/tl-source.hpp>
#include <sstream>

using namespace TL::GFN;

ParallelFor TL::GFN::parallel_for(PragmaCustomConstruct construct, 
                                  TL::ForStatement for_stmt, 
                                  KernelInfo *kernel_info,
                                  ScopeLink scope_link,
                                  AST_t translation_unit,
                                  FILE *kernel_decl_file,
                                  int optimization_level)
{
    return ParallelFor(construct, for_stmt, kernel_info, scope_link,
                       translation_unit, kernel_decl_file, optimization_level);
}


TL::Source ParallelFor::get_source()
{
    // Nothing at this moment
    return do_parallel_for();
}

ParallelFor::ParallelFor(PragmaCustomConstruct construct, 
                         ForStatement for_stmt, 
                         KernelInfo *kernel_info,
                         ScopeLink scope_link,
                         AST_t translation_unit,
                         FILE *kernel_decl_file,
                         int optimization_level)
     : _construct(construct), _for_stmt(for_stmt), 
       _function_def(0), _kernel_info(kernel_info),
       _kernel_decl_file(kernel_decl_file)
{
     _scope_link = scope_link;
     _translation_unit = translation_unit;
     _optimization_level = optimization_level;
}

TL::Source ParallelFor::do_parallel_for()
{
    bool debug = false;
    
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
    TL::Source worker_broadcast_scalar_src;
    TL::Source worker_allocate_array_memory_src;
    TL::Source worker_initialize_generated_variables_src;
    TL::Source worker_allocate_reduce_scalar_src;
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
    
    // Optimization code
    TL::Source stream_loop_variable;

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
    //std::string bound_opr = (std::string)_for_stmt.get_bound_operator();
    std::string loop_iter_size;
    TL::Statement original_loop_body = _for_stmt.get_loop_body();
    // Copy loop body for cluster level and GPU level
    TL::Statement cluster_loop_body(original_loop_body);
    TL::Statement gpu_loop_body(original_loop_body);

    std::string induction_var_name = (std::string)induction_var;
    std::string inner_induction_var_name = _kernel_info->_inner_induction_var;
    std::string new_induction_var_name = GFN_PREFIX_LOCAL + induction_var_name;
    
    std::string level1_cond = _kernel_info->_level_1_condition;
    std::string level2_cond = _kernel_info->_level_2_condition;
    
    TL::Source worker_function_name;
    TL::Source kernel_name;
    
    worker_function_name << "_Function_" << _kernel_info->kernel_id;
    kernel_name << "_kernel_" << _kernel_info->kernel_id;
    
    stream_loop_variable
        << "int _stream_loop_size;"
        << "int _stream_completed = 0;"
        << "int _sequence_id = 0;"
        << "size_t _stream_global_item_num;"
        << "size_t _stream_work_group_item_num;";

    /*== ---------- Create source about loop size ------------------==*/
    if (debug) std::cout << "create source about loop size\n";
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

        worker_broadcast_scalar_src
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
    std::string kernel_id_var = "_kernel_id";
    std::string kernel_info_var = "_kernel_info";
    
    std::string inner_start_idx_var = "_" + inner_induction_var_name + "_start";
    std::string inner_end_idx_var = "_" + inner_induction_var_name + "_end";
    std::string inner_loop_step_var = "_inner" + loop_step_var;
    std::string inner_cl_loop_step_var = "_cl_inner_loop_step";
    std::string inner_loop_size_var = "_inner" + loop_size_var;

    std::string gen_index = "_gen_idx";
    std::string gen_loop_size = "_gen_loop_size";
    
    worker_declare_generated_variables_src
	<< "int " << kernel_id_var << " = " << _kernel_info->kernel_id << ";"
        << "int " << local_start_idx_var << "," << local_end_idx_var << ";"
        << "int " << loop_step_var << ";"
        << "int " << loop_size_var << ";"
        << "int " << new_induction_var_name << " = 0;"
	<< "struct _kernel_information * " << kernel_info_var << " = 0;"
        << "size_t _work_item_num, _work_group_item_num, _global_item_num;"
        << "cl_kernel _kernel;";
    if (_kernel_info->_has_inner_loop)
    {
        worker_declare_generated_variables_src
            << "int " << inner_start_idx_var << "," << inner_end_idx_var << ";"
            << "int " << inner_loop_step_var << ";"
            << "int " << inner_loop_size_var << ";";
        worker_declare_generated_variables_src
            << "int " << gen_index << ";"
            << "int " << gen_loop_size << ";";
    }
    
    
    worker_initialize_generated_variables_src
        << local_start_idx_var << " = _GfnCalcLocalLoopStart("
        << loop_start << ", " << loop_end << ", " << loop_step << ");"
        << local_end_idx_var << " = _GfnCalcLocalLoopEnd("
        << loop_start << ", " << loop_end << ", " << loop_step << ");"
        << loop_size_var << " = _CalcLoopSize("
        << loop_start << ","
        << loop_end << ","
        << step.prettyprint() << ");"
        << loop_step_var << " = " << loop_step << ";";
    if (_kernel_info->_has_inner_loop)
    {
        worker_initialize_generated_variables_src
            << inner_start_idx_var << " = " << _kernel_info->_inner_lower_bound_expr << ";"
            << inner_end_idx_var << " = " << _kernel_info->_inner_upper_bound_expr << ";"
            << inner_loop_step_var << " = " << _kernel_info->_inner_loop_step << ";"
            << inner_loop_size_var << " = _CalcLoopSize("
                << inner_start_idx_var << ","
                << inner_end_idx_var << ","
                << inner_loop_step_var << ");";
        worker_initialize_generated_variables_src
            << gen_loop_size << " = " << loop_size_var << " * " << inner_loop_size_var << ";";
    }
    worker_initialize_generated_variables_src
        << "_work_item_num = _CalcSubSize("
        << ((_kernel_info->_has_inner_loop)? gen_loop_size : loop_size_var)
        << ", _gfn_num_proc, _gfn_rank, 1);"
        << "_work_group_item_num = 64;"
        << "_global_item_num = _GfnCalcGlobalItemNum(_work_item_num, _work_group_item_num);";
    if (_optimization_level > 0)
    {
        worker_initialize_generated_variables_src
            << "_GfnStreamSeqKernelRegister(_kernel_id, _local_" << induction_var_name << "_start, "
            << "_local_" << induction_var_name << "_end, " << loop_start << "," << loop_end << ", _loop_step, &" 
	    << kernel_info_var << ");"; 
    }
        
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
        << "_SendCallFuncMsg(" << _kernel_info->kernel_id << ");";

    std::cout << "Generate code about variables\n";
    for (int i = 0; i < _kernel_info->_var_info.size(); ++i)
    {
        VariableInfo var_info = _kernel_info->_var_info[i];
        DataReference var_ref = _kernel_info->_var_ref[i];

        std::string var_name = var_info.get_name();
        if (debug)
            std::cout << "GENERATED CODE FOR " <<  var_name << "\n";
        std::string var_unique_id_name = var_info.get_id_name();
        std::string var_cl_name = var_info.get_cl_name();
        std::string var_cl_global_reduction = "_global_reduction_" + var_name;
        std::string var_local_name = GFN_PREFIX_LOCAL + var_name;
        std::string var_cl_local_mem_name = "_cl_local_mem_" + var_name;
        std::string ptr_stars = var_info.get_pointer_starts();
        bool is_partition = (var_info._shared_dimension >= 0);

        // TODO: _GFN_MEM_ALLOC_HOST_PTR(), [Research] Opttimize memory type
        std::string var_cl_mem_type;
        if (var_info._is_input && var_info._is_output)
            var_cl_mem_type = "_GFN_MEM_READ_WRITE()";
        else if (var_info._is_input)
            var_cl_mem_type = "_GFN_MEM_READ_ONLY()";
        else
            var_cl_mem_type = "_GFN_MEM_WRITE_ONLY()";
        
        int in_pattern_type = var_info._in_pattern_type;
        int out_pattern_type = var_info._out_pattern_type;
        TL::ObjectList<std::string> &in_pattern_array = var_info._in_pattern_array;
        TL::ObjectList<std::string> &out_pattern_array = var_info._out_pattern_array;

        TL::Type type = var_ref.get_type();
        while (type.is_pointer()) type = type.points_to();
        while (type.is_array()) type = type.array_element();
        std::string mpi_type_str = type_to_mpi_type(type);
        std::string c_type_str = type_to_ctype(type);
        std::string size_str = var_info.get_allocate_size_in_byte(type);
        //std::cout << i << ". Type of " << var_info._name << " is " << c_type_str << std::endl;

        /* 1. Declaration necessary variable */
        if (debug) std::cout << "\tSTEP 1\n";
        if (var_info._is_private)
        {
            //std::cout << "temp or private\n";
            // define as size in temp() clause
            TL::Source tmp_decl_src;
            tmp_decl_src << c_type_str << " " << var_name;
            for (int i = 0; i < var_info._dimension_num; ++i)
                tmp_decl_src << "[" << var_info._dim_size[i] << "]";
            tmp_decl_src << ";";
            
            //std::cout << "dim num = " << var_info._dimension_num << "\n";
                
            worker_declare_variables_src << tmp_decl_src;
            cl_kernel_var_decl << tmp_decl_src << CL_EOL;
        }
        else if (var_info._is_array_or_pointer)
        {
            //std::cout << "array or pointer\n";
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
            //std::cout << "other else\n";
            worker_declare_variables_src
                << var_ref.get_type().get_declaration(var_ref.get_scope(), var_name) << ";";

            if (!var_info._is_index && 
                !var_info._is_input /* Scalar input is function parameters, not need to declare again */)
            {
                cl_kernel_var_decl
                    << var_ref.get_type().get_declaration(var_ref.get_scope(), var_name) << ";" << CL_EOL;
                    std::cout << "Add code declaration on kernel " << var_name << "\n";
            }
        }

        /* 2. Declaration generated variable for each variable */
        if (debug) std::cout << "\tSTEP 2\n";
        if (var_info._is_input || var_info._is_output || var_info._is_temp_array || var_info._is_present)
        {
            if (var_info._is_array_or_pointer)
            {
                worker_declare_generated_variables_src
                    << "long long " << var_unique_id_name << ";";
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
                        << "__global " << ((!var_info._is_output && var_info._is_input)? "const " : "")
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
        if (debug) std::cout << "\tSTEP 3\n";
        if (var_info._is_array_or_pointer &&            
            (var_info._is_input || var_info._is_output || var_info._is_temp_array || var_info._is_present))
        {
            master_send_scalar_input
                << create_send_var_id_msg(var_name, var_info._dimension_num);

            worker_broadcast_scalar_src
                << create_gfn_q_bcast_scalar(var_unique_id_name, "_GFN_TYPE_LONG_LONG_INT()");

            worker_allocate_array_memory_src
                << create_gfn_malloc_nd(var_name, var_cl_name, var_unique_id_name,
                                        mpi_type_str, var_info._dimension_num, var_info._dim_size,
                                        var_cl_mem_type, level1_cond, level2_cond);

            worker_free_array_memory_src
                << create_gfn_free(var_unique_id_name, level1_cond, level2_cond);
        }

        /* 4. Broadcasr and scatter for input array */
        if (debug) std::cout << "\tSTEP 4\n";
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
            else if (!var_info._is_loop_variable)
            {
                master_send_scalar_input
                    << "_SendInputMsg((void*)&" << var_name << "," << size_str << ");";
            }

            /* Worker code */
            std::cout << var_name << " IS " << ((is_partition)?"":"NOT ") << "PARTITION\n";
            if (var_info._is_array_or_pointer && is_partition)
            {
                if (_optimization_level > 0)
                {
                    /* distribute first chunk and boundary */
                    worker_distribute_array_memory_src
                        << create_gfn_q_stream_scatter_nd(kernel_info_var,
                                                var_name, var_cl_name, var_unique_id_name, mpi_type_str, 
                                                loop_start, loop_end, loop_step, var_info._dimension_num,
                                                var_info._dim_size, var_info._shared_dimension, 
                                                var_cl_mem_type, in_pattern_array, in_pattern_array.size(),
                                                in_pattern_type, level1_cond, level2_cond);
                }
                else
                {
                    worker_distribute_array_memory_src
                        << create_gfn_q_scatter_nd(var_name, var_cl_name, var_unique_id_name, mpi_type_str, 
                                                loop_start, loop_end, loop_step, var_info._dimension_num,
                                                var_info._dim_size, var_info._shared_dimension, 
                                                var_cl_mem_type, in_pattern_array, in_pattern_array.size(),
                                                in_pattern_type, level1_cond, level2_cond);
                }
            }
            else if (var_info._is_array_or_pointer)
            {
                worker_distribute_array_memory_src
                    << create_gfn_q_bcast_nd(var_name, var_cl_name, var_unique_id_name, mpi_type_str, 
                                             var_info._dimension_num, var_info._dim_size,
                                             level1_cond, level2_cond);
            }
            else if (!var_info._is_loop_variable)
            {
                worker_broadcast_scalar_src
                    << create_gfn_q_bcast_scalar(var_name, mpi_type_str);
            }
        }
        
        /* 5. Gather data from output array */
        if (debug) std::cout << "\tSTEP 5\n";
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
                if (_optimization_level > 0)
                {
                    /* distribute first chunk and boundary */
                    worker_distribute_array_memory_src
                        << create_gfn_q_stream_gather_nd(kernel_info_var,
                                                var_name, var_cl_name, var_unique_id_name, mpi_type_str, 
                                                loop_start, loop_end, loop_step, var_info._dimension_num,
                                                var_info._dim_size, var_info._shared_dimension, 
                                                var_cl_mem_type, in_pattern_array, in_pattern_array.size(),
                                                in_pattern_type, level1_cond, level2_cond);
                    worker_gather_array_memory_src
                        << create_gfn_f_stream_gather_nd(kernel_info_var,
                                                var_name, var_cl_name, var_unique_id_name, mpi_type_str, 
                                                loop_start, loop_end, loop_step, var_info._dimension_num,
                                                var_info._dim_size, var_info._shared_dimension, 
                                                var_cl_mem_type, in_pattern_array, in_pattern_array.size(),
                                                in_pattern_type, level1_cond, level2_cond);
                }
                else
                {
                    worker_gather_array_memory_src
                        << create_gfn_q_gather_nd(var_name, var_cl_name, var_unique_id_name, mpi_type_str, 
                                                loop_start, loop_end, loop_step, var_info._dimension_num, 
                                                var_info._dim_size, var_info._shared_dimension, 
                                                var_cl_mem_type, out_pattern_array, out_pattern_array.size(),
                                                out_pattern_type, level1_cond, level2_cond);
                }
            }
            else if (var_info._is_array_or_pointer)
            {
                std::cerr << "Error: try to gather non-parition array " << var_name << "\n";
            }
            else
            {
                worker_reduce_scalar_src
                    << create_gfn_q_reduce_scalar(var_name, var_cl_name, mpi_type_str, op_to_mpi_op(var_info._reduction_type), 
                                                  "_global_item_num/_work_group_item_num", level1_cond, level2_cond);
            }
        }

        /* 6. Reduction */
        if (debug) std::cout << "\tSTEP 6\n";
        if (var_info._is_reduction)
        {
            worker_allocate_reduce_scalar_src
                << create_gfn_malloc_reduce(var_name, var_cl_name, mpi_type_str,
                                            "_global_item_num/_work_group_item_num", level1_cond, level2_cond);

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

            /*cl_kernel_reduce_global_init
                << "*" << var_cl_global_reduction << " = "
                << reduction_op_init_value(var_info._reduction_type) << ";" << CL_EOL;*/

            /*cl_kernel_reduce_global_reduce
                << create_cl_help_atomic_call(var_cl_global_reduction, var_cl_local_mem_name,
                                              var_info._reduction_type, type);*/
            cl_kernel_reduce_global_reduce
                << var_cl_global_reduction << "[get_group_id(0)] = "
                << var_cl_local_mem_name << "[0];" << CL_EOL;

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
    std::cout << "Create MPI Block\n";
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
    //std::cout << "After replace\n";

    if (_kernel_info->_has_inner_loop)
    {
        /*cl_set_kernel_arg
            << create_cl_set_kernel_arg("_kernel", kernel_arg_num++, "int", inner_start_idx_var);
        cl_actual_params.append_with_separator(("int " + inner_start_idx_var), ",");
        cl_set_kernel_arg
            << create_cl_set_kernel_arg("_kernel", kernel_arg_num++, "int", inner_end_idx_var);
        cl_actual_params.append_with_separator(("int " + inner_end_idx_var), ",");
        cl_set_kernel_arg
            << create_cl_set_kernel_arg("_kernel", kernel_arg_num++, "int", inner_loop_step_var);
        cl_actual_params.append_with_separator(("int " + inner_loop_step_var), ",");*/
        
        cl_set_kernel_arg
            << create_cl_set_kernel_arg("_kernel", kernel_arg_num++, "int", inner_loop_size_var);
        cl_actual_params.append_with_separator(("int " + inner_loop_size_var), ",");
        
        
        worker_initialize_generated_variables_src
            << local_start_idx_var << " = " << local_start_idx_var << " * _inner_loop_size;"
            << local_end_idx_var << " = " << local_end_idx_var << " * _inner_loop_size;";
            // TODO: step
    }
    
    
    std::string kernel_start_param, kernel_end_param;
    kernel_start_param = local_start_idx_var;
    kernel_end_param = local_end_idx_var;
    
    // Current local loop start
    if (_optimization_level > 0)
    {
	cl_set_kernel_arg
		<< create_cl_set_kernel_arg("_kernel", kernel_arg_num++, "int", "_kernel_info->stream_seq_start_idx");
	cl_actual_params.append_with_separator(("int " + local_start_idx_var), ",");
	// Current local loop end
	cl_set_kernel_arg
		<< create_cl_set_kernel_arg("_kernel", kernel_arg_num++, "int", "_kernel_info->stream_seq_end_idx");
	cl_actual_params.append_with_separator(("int " + local_end_idx_var), ",");
    }
    else
    {
	cl_set_kernel_arg
		<< create_cl_set_kernel_arg("_kernel", kernel_arg_num++, "int", local_start_idx_var);
	cl_actual_params.append_with_separator(("int " + local_start_idx_var), ",");
	// Current local loop end
	cl_set_kernel_arg
		<< create_cl_set_kernel_arg("_kernel", kernel_arg_num++, "int", local_end_idx_var);
	cl_actual_params.append_with_separator(("int " + local_end_idx_var), ",");
    }
    // Current loop step
    //if (loop_step != "1")
    {
        cl_set_kernel_arg
            << create_cl_set_kernel_arg("_kernel", kernel_arg_num++, "int", loop_step_var);
        cl_actual_params.append_with_separator(("int " + loop_step_var), ",");
    }
    

    if (_kernel_info->_has_inner_loop)
    {
        cl_kernel_var_decl
            << "int _tid = get_global_id(0) + " << local_start_idx_var << ";\n"
            << "int " << induction_var << " = _tid / " << inner_loop_size_var << ";\n"
            << inner_induction_var_name << " = _tid % " << inner_loop_size_var << ";\n";
    }
    else
    {
        cl_kernel_var_decl
            /*<< "int " << loop_size_var << " = ("
                << local_end_idx_var << " - " << local_start_idx_var
                << ") / " << loop_step_var << ";" << CL_EOL*/
            << "int " << new_induction_var_name << " = "
            << "get_global_id(0) * " << loop_step_var << ";" << CL_EOL
            << "int " << induction_var << " = " << new_induction_var_name 
            <<" + " << local_start_idx_var << ";" << CL_EOL;
    }
    // Kernel helper function
    //cl_kernel
        //<< create_cl_help_barrier() // TODO: insert if use
        //<< create_cl_help_atomic_add_int() // TODO: insert if use
        //<< create_cl_help_atomic_add_float() // TODO: insert if use
        //<< create_cl_help_atomic_add_double(); // TODO: insert if use

    // Kernel reduction implement
    TL::Source cl_kernel_reduction;
    if (_kernel_info->_has_reduction_clause)
    {
        cl_kernel_reduction
            << "if (" << induction_var << " <= " << loop_end << ") {" << CL_EOL
            // [Reduction Step] -
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
            /*<< "if (get_global_id(0) == 0) {" << CL_EOL
            << cl_kernel_reduce_global_init
            << "}" << CL_EOL*/ 
            << "barrier(CLK_GLOBAL_MEM_FENCE);" << CL_EOL
            // [Reduction Step] - Set subsum of local memory to global memory
            << "if (get_local_id(0) == 0) {" << CL_EOL
            << cl_kernel_reduce_global_reduce
            << "}" << CL_EOL;
    }

    /*== -------------  Create GPU kernel function -----------------==*/
    std::cout << "Create OpenCL Kernel\n";
    
    // Kernel main funcion
    cl_kernel
        << "__kernel void " << kernel_name
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
        << comment("KERNEL_DEFINITION " + (std::string)kernel_name)
        //<< show_cl_source_in_comment(cl_kernel)
        << "const char *" << kernel_name << "_src = "
        << source_to_kernel_str(cl_kernel) << ";";
    cl_create_kernel
        << "_kernel = _GfnCreateKernel(\"" << kernel_name << "\");";
    cl_release_kernel
        << "_GfnClearKernel(_kernel);";

    if (_optimization_level > 0)
    {
        cl_launch_kernel
            << "_GfnLaunchKernel(_kernel, &_stream_global_item_num, &_stream_work_group_item_num, " 
	    << kernel_info_var << ");";
    }
    else
    {
        cl_launch_kernel
            << "_GfnLaunchKernel(_kernel,&_global_item_num,&_work_group_item_num,0);";
    }


    /*==----------------  Create worker function -------------------==*/
    worker_func_def
        << comment("WORKER_FUNCTION " + int_to_string(_kernel_info->kernel_id) + " " + (std::string)worker_function_name)
        << "void " << worker_function_name << " () {"
        
            << comment("Declare Variables")
            << worker_declare_variables_src
        
            << comment("Declare Generated Variables")
            << worker_declare_generated_variables_src
        
            << comment("Broadcast Scalar Value")
            << worker_broadcast_scalar_src
            << create_gfn_f_bcast_scalar()
            
            << comment("Allocate Array Memory")
            << worker_allocate_array_memory_src
            
            << comment("Initialize Generated Variables")
            << worker_initialize_generated_variables_src
            
            << comment("Allocate Reduce Scalar Variables")
            << worker_allocate_reduce_scalar_src
            
            << comment("Distribute Array Memory")
            << worker_distribute_array_memory_src
            << create_gfn_f_dist_array();
            
            /* Optimization */
            if (_optimization_level > 0) 
            {
                worker_func_def
                    << "while (1) {"
                    << stream_loop_variable
                    << "_GfnStreamSeqKernelGetNextSequence(" << kernel_info_var << ", &_sequence_id, &_stream_global_item_num, &_stream_work_group_item_num);"
                    << "if (_GfnStreamSeqExec(_kernel_info->stream_seq_start_idx, _kernel_info->stream_seq_end_idx)) {";
            }
            
        worker_func_def
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
            << "}";
            
            /* Optimization */
            if (_optimization_level > 0) 
            {
                worker_func_def
                    << "}"
                    << "_GfnStreamSeqKernelFinishSequence(" << kernel_info_var << ");"
                    << "if (_kernel_info->is_complete) break;"
                    << "}";
            }
            
        worker_func_def
            << comment("Gather Array Memory")
            << worker_gather_array_memory_src
            << create_gfn_f_gather_array()
            
            << comment("Reduce Scalar Value")
            << worker_reduce_scalar_src
            << create_gfn_f_reduce_scalar()
            
            << comment("Deallocate Array Memory")
            << worker_free_array_memory_src
            
            //<< "_SyncMaster();"
        << "}";

    if(debug){
    std::cout << " ================= Worker Function ================\n";
    std::cout << (std::string) worker_func_def << "\n";
    std::cout << " ==================================================\n";}

    /*TL::AST_t worker_func_tree = worker_func_def.parse_declaration(
            _function_def->get_point_of_declaration(),
            _function_def->get_scope_link());*/
    //_function_def->get_ast().prepend_sibling_function(worker_func_tree);

    // print kernel to kernel declare file
    print_to_kernel_decl_file(_scope_link, _translation_unit, _kernel_decl_file, cl_kernel_src_str);
    print_to_kernel_decl_file(_scope_link, _translation_unit, _kernel_decl_file, worker_func_def);
    

    send_call_func
        << send_loop_size
        << master_send_scalar_input
        << master_send_array_input
        << master_recv_output
        //<< "_SyncWorker();"
        ;

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
}
#endif

// Replace function
void ParallelFor::replace_parallel_loop_body(Statement stmt,
                                             std::vector<bool> &replace_types,
                                             std::string old_idx_name,
											 std::string new_idx_name)
{
    bool debug = false;
    
    if (stmt.is_declaration())
    {
        if(debug)std::cout<<"REPLACE STMT DECL\n";
    }
    else if (TL::ForStatement::predicate(stmt.get_ast()))
    {
        if(debug)std::cout<<"REPLACE STMT FOR\n";
        
        TL::ForStatement for_stmt = ForStatement(stmt.get_ast(), stmt.get_scope_link());
        // TODO: init, cond, incre ?
        replace_parallel_loop_body(for_stmt.get_loop_body(), replace_types, old_idx_name, new_idx_name);
    }
    else if (TL::IfStatement::predicate(stmt.get_ast()))
    {
        if(debug)std::cout<<"REPLACE STMT IF\n";
        
        TL::IfStatement if_stmt = TL::IfStatement(stmt.get_ast(), stmt.get_scope_link());
        
        replace_parallel_loop_body(if_stmt.get_condition().get_expression(), 
                                   replace_types, old_idx_name, new_idx_name);
        replace_parallel_loop_body(if_stmt.get_then_body(),
                                   replace_types, old_idx_name, new_idx_name);
        if (if_stmt.has_else())
        {
            replace_parallel_loop_body(if_stmt.get_else_body(),
                                       replace_types, old_idx_name, new_idx_name);
        }
    }
    else if (TL::WhileStatement::predicate(stmt.get_ast()))
    {
        TL::WhileStatement while_stmt = TL::WhileStatement(stmt.get_ast(), stmt.get_scope_link());
        
        replace_parallel_loop_body(while_stmt.get_condition().get_expression(),
                                   replace_types, old_idx_name, new_idx_name);
        replace_parallel_loop_body(while_stmt.get_body(), 
                                   replace_types, old_idx_name, new_idx_name);
    }
    else if (TL::DoWhileStatement::predicate(stmt.get_ast()))
    {
        TL::DoWhileStatement do_while_stmt = TL::DoWhileStatement(stmt.get_ast(), stmt.get_scope_link());
        
        replace_parallel_loop_body(do_while_stmt.get_expression(),
                                   replace_types, old_idx_name, new_idx_name);
        replace_parallel_loop_body(do_while_stmt.get_body(),
                                   replace_types, old_idx_name, new_idx_name);
    }
    else if (TL::SwitchStatement::predicate(stmt.get_ast()))
    {
        TL::SwitchStatement switch_stmt = TL::SwitchStatement(stmt.get_ast(), stmt.get_scope_link());
        
        replace_parallel_loop_body(switch_stmt.get_condition().get_expression(),
                                   replace_types, old_idx_name, new_idx_name);
        
        // TODO: replace switch expression
    }
    /* Ignore case */
    else if (TL::BreakStatement::predicate(stmt.get_ast()) ||
             TL::ContinueStatement::predicate(stmt.get_ast()) ||
             TL::ReturnStatement::predicate(stmt.get_ast()) ||
             TL::EmptyStatement::predicate(stmt.get_ast()) ||
             TL::GotoStatement::predicate(stmt.get_ast()))
    {
        // do nothing
    }
    else if (stmt.is_compound_statement())
    {
        if(debug)std::cout<<"REPLACE STMT COMPOUND\n";
        
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
        if(debug)std::cout<<"REPLACE STMT EXPR\n";
        
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
    bool debug = false;
    
    if (expr.is_id_expression())
    {
        if(debug)std::cout<<"REPLACE EXPR ID\n";
        
        // Traverse child nodes
        // TODO: collect_variable_info(, kernel_info); ??
    }
    else if (expr.is_array_subscript())
    {
        if(debug)std::cout<<"REPLACE EXPR ARRAY\n";
        
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
            if(debug)std::cout<<"REP VARNAME : "<<var_name<<"\n";
            int idx = _kernel_info->get_var_info_index_from_var_name(var_name);
            if(debug)std::cout<<"REP INDEX : "<<idx<<"\n";
            VariableInfo var_info = _kernel_info->_var_info[idx];
            
            if (var_info._is_private) return; // Don't need to replace temp variable
            
            std::string dim1_size = (_kernel_info->_var_info[idx]._dimension_num >= 1)?
                (std::string)_kernel_info->_var_info[idx]._dim_size[0] : "1";
            std::string dim2_size = (_kernel_info->_var_info[idx]._dimension_num >= 2)?
                (std::string)_kernel_info->_var_info[idx]._dim_size[1] : "1";
            std::string dim3_size = (_kernel_info->_var_info[idx]._dimension_num >= 3)?
                (std::string)_kernel_info->_var_info[idx]._dim_size[2] : "1";
            if(debug)std::cout<<"REP DIM 1 SIZE : "<<dim1_size<<"\n";
            if(debug)std::cout<<"REP DIM 2 SIZE : "<<dim2_size<<"\n";
            if(debug)std::cout<<"REP DIM 3 SIZE : "<<dim3_size<<"\n";

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
        TL::ObjectList<TL::Expression> arg_list = expr.get_argument_list();
        for (TL::ObjectList<TL::Expression>::iterator it = arg_list.begin();
            it != arg_list.end(); it++)
        {
            replace_parallel_loop_body(*it, replace_types, old_idx_name, new_idx_name);
        }
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
        int idx = _kernel_info->get_var_info_index_from_var_name(var_name);
        std::cout << var_name << " index is " << idx << ((_kernel_info->_var_info[idx]._is_private)? " and private" : " and shared")
            << "\n";
        if (var_name == old_name &&
            !_kernel_info->_var_info[idx]._is_private) /// Don't need to replace temp variable
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
        std::cout << "unknown expression type in replace_loop_index_name\n";
        // TODO:
    }
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
