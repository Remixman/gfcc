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
    ObjectList<IdExpression> loop_size_var_list;
    bool enable_opencl = true;

    // C/C++ and MPI sources
    TL::Source send_input, recv_output;
    TL::Source worker_input_buffer_init, worker_input_buffer_free;
    TL::Source worker_func_def, worker_recv_input, worker_send_output;
    TL::Source worker_scatter_input, worker_gather_output;
    TL::Source worker_var_decl, worker_gen_var_decl;
    TL::Source worker_init_gen_var, worker_free_gen_var;
    TL::Source worker_root_free_gen_var;

    // MPI loop size sources
    TL::Source send_loop_size;
    TL::Source worker_recv_loop_size, worker_scatter_loop_size;

    // CUDA sources
    TL::Source kernel_actual_param, kernel_formal_param;

    // OpenCL sources
    TL::Source cl_var_decl, cl_init_var, cl_free_var;
    TL::Source cl_write_input, cl_read_output;
    TL::Source cl_create_kernel, cl_set_kernel_arg;
    TL::Source cl_kernel_src_str, cl_kernel, cl_launch_kernel;
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
    std::string step_str = (std::string)step;
    //std::string bound_opr = (std::string)_for_stmt.get_bound_operator();
    std::string loop_iter_size;
    Statement loop_body = _for_stmt.get_loop_body();
	
	std::string induction_var_name = (std::string)induction_var;
	std::string new_induction_var_name = GFN_PREFIX_LOCAL + induction_var_name;

    /*== ---------- Create source about loop size ------------------==*/
    loop_size_var_list.append( lower_bound.all_symbol_occurrences(TL::Statement::ONLY_VARIABLES) );
    loop_size_var_list.append( upper_bound.all_symbol_occurrences(TL::Statement::ONLY_VARIABLES) );
    // TODO: eliminate duplicate variable in loop_size_var_list
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

        send_loop_size
            << "_SendInputMsg((void *)&" << var_name
            << ", sizeof (" << c_type_str << "));";

        worker_recv_loop_size
            << "_RecvInputMsg((void *)&" << var_name
            << ", sizeof (" << c_type_str << "));";
        worker_scatter_loop_size
            << create_mpi_bcast(("&"+var_name), "1", mpi_type_str);
    }

    /*== ----- Create MPI block distribution for statement ---------==*/
    std::string local_idx_var = GFN_PREFIX_LOCAL + induction_var_name;
    std::string local_start_idx_var = GFN_PREFIX_LOCAL + induction_var_name + "_start";
    std::string local_end_idx_var = GFN_PREFIX_LOCAL + induction_var_name + "_end";
    std::string loop_step_var = "_loop_step";
    std::string local_cl_start_idx_var = "_local_cl_" + induction_var_name + "_start";
    std::string local_cl_end_idx_var = "_local_cl_" + induction_var_name + "_end";
    std::string cl_loop_step_var = "_cl_loop_step";
    std::string loop_size_var = "_loop_size";
    worker_gen_var_decl
        << "int " << local_start_idx_var << "," << local_end_idx_var << ";"
        << "int " << loop_step_var << ";"
        << "int " << loop_size_var << ";"
		<< "int " << new_induction_var_name << " = 0;";
    worker_init_gen_var
        << local_start_idx_var << " = _CalcLocalStartIndex("
        << lower_bound.prettyprint() << ","
        << upper_bound.prettyprint() << ",_gfn_num_proc,_gfn_rank+1);"
        << local_end_idx_var << " = _CalcLocalEndIndex("
        << lower_bound.prettyprint() << ","
        << upper_bound.prettyprint() << ",_gfn_num_proc,_gfn_rank+1);"
        << loop_size_var << " = _CalcLoopSize("
        << lower_bound.prettyprint() << ","
        << upper_bound.prettyprint() << ","
        << step.prettyprint() << ");"
        << loop_step_var << " = " << step_str << ";";

    // XXX: we indicate with only step symbol
    bool is_incre_loop = (step_str[0] == '-')? false : true;
    if (is_incre_loop)
    {
        loop_iter_size = "(" + upper_bound.prettyprint() + "-"
                + lower_bound.prettyprint() + "+1)";
    }
    else
    {
        loop_iter_size = "(" + lower_bound.prettyprint() + "-"
                + upper_bound.prettyprint() + "+1)";
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
        std::string var_cnts = "_" + var_name + "_cnts"; // for array or pointer
        std::string var_disp = "_" + var_name + "_disp"; // for array or pointer
        std::string var_sub_size = "_sub_size_" + var_name;
        std::string var_buf_name = "_buffer_" + var_name;// for array or pointer
        std::string var_cl_name = "_cl_mem_" + var_name;
		std::string var_cl_global_reduction = "_global_reduction_" + var_name;
		std::string var_local_name = GFN_PREFIX_LOCAL + var_name;
        std::string var_cl_local_mem_name = "_cl_local_mem_" + var_name;
        std::string ptr_stars = var_info.get_pointer_starts();

        std::string var_cl_mem_type;
        if (var_info._is_input && var_info._is_output)
            var_cl_mem_type = "_get_cl_mem_read_write()";
        else if (var_info._is_input)
            var_cl_mem_type = "_get_cl_mem_read_only()";
        else if (var_info._is_output)
            var_cl_mem_type = "_get_cl_mem_write_only()";


        TL::Type type = var_ref.get_type();
        std::string mpi_type_str = type_to_mpi_type(type);
        std::string c_type_str = type_to_ctype(type);
        std::string size_str;
        if (type.is_array() || type.is_pointer())
        {
            size_str = "(sizeof(" + c_type_str + ") * " + var_info.get_mem_size() + ")";
        }
        else
        {
            size_str = "sizeof(" + c_type_str  + ")";
        }
        //std::cout << "Type of " << var_info._name << " is " << c_type_str << std::endl;

        std::string full_expr_sub_size = "sizeof(" + c_type_str + ")";
        if (var_info._is_array_or_pointer)
        {
            full_expr_sub_size += " * " + var_sub_size;
        }

        /* Declaration necessary variable */
        if (var_info._is_array_or_pointer)
        {
            worker_var_decl
                << c_type_str << " * " << var_name << ";";
        }
        else if (var_info._is_reduction)
        {
            worker_var_decl
                << c_type_str << " " << var_local_name << " = "
                << reduction_op_init_value(var_info._reduction_type) << ";";
        }
        else
        {
            worker_var_decl
                << var_ref.get_type().get_declaration(var_ref.get_scope(), var_name) << ";";

            if (!var_info._is_index)
            {
                cl_kernel_var_decl
                    << var_ref.get_type().get_declaration(var_ref.get_scope(), var_name) << ";\n";
            }
        }


        /* Declaration generated variable */
        if (var_info._is_input || var_info._is_output)
        {
            // About size code (only array or pointer)
            if (var_info._shared_dimension != 0)
            {
                worker_gen_var_decl
                    // ipc recv buffer variable
                    << c_type_str << " * " << var_buf_name << ";"
                    // sub size of variable
                    << "int " << var_sub_size << ";"
                    // send counts variable
                    << "int " << var_cnts << "[_gfn_num_proc];"
                    // send displacement variable
                    << "int " << var_disp << "[_gfn_num_proc];";

                worker_init_gen_var
                    // init sub size
                    << var_sub_size << " = _CalcSubSize("
                    << var_info.get_distributed_mem_size() << ",_gfn_num_proc,_gfn_rank"
                    << "," << var_info.get_distributed_mem_block() << ");"

                    // allocate sub array
                    << var_name << " = (" << c_type_str
                    <<"*)malloc(sizeof(" << c_type_str << ") * "
                    << var_sub_size << ");"

                    // init counts
                    << "_CalcCnts(" << var_info.get_distributed_mem_size()
                    << ",_gfn_num_proc," << var_cnts
                    << "," << var_info.get_distributed_mem_block() << ");"
                    // init displacement
                    << "_CalcDisp(" << var_info.get_distributed_mem_size()
                    << ",_gfn_num_proc," << var_disp
                    << "," << var_info.get_distributed_mem_block() << ");";

                worker_free_gen_var
                    << "free(" << var_name << ");";
            }

            // About OpenCL parameter code
            if (var_info._is_array_or_pointer || var_info._is_output)
            {
                cl_var_decl
                    << "cl_mem " << var_cl_name << ";";

                cl_init_var
                    << var_cl_name << " = "
                    << create_cl_create_buffer("_gfn_context", var_cl_mem_type,
                       (var_info._shared_dimension != 0) ? full_expr_sub_size : size_str, "0");

                cl_free_var
                    << create_cl_release_mem_object(var_cl_name);

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
            else
            {
                TL::Source cl_actual_param;
                cl_actual_param
                    << c_type_str << " " << var_name;
                cl_actual_params.append_with_separator(cl_actual_param, ",");
            }
        }

        if (var_info._is_input)
        {
            send_input
                << "_SendInputMsg((void*)" << ((var_info._is_array_or_pointer)? "" : "&")
                << var_name << "," << size_str << ");";

            if (var_info._shared_dimension != 0)
            {
                /* use VAR_BUF_NAME that name of buffer before scatter */

                worker_recv_input
                    << var_buf_name << " = (" << c_type_str
                    <<"*)malloc(" << size_str << ");";
                worker_recv_input
                    << "_RecvInputMsg((void*)" << var_buf_name << ","
                    << size_str << ");";

                worker_root_free_gen_var
                    << "free(" << var_buf_name << ");";

                worker_scatter_input
                    << create_mpi_scatterv(var_buf_name, var_cnts, var_disp,
                                           mpi_type_str, var_name,
                                           var_sub_size, mpi_type_str);

                cl_write_input
                    << create_cl_enqueue_write_buffer("_gfn_cmd_queue",
                           var_cl_name, true, "0", full_expr_sub_size, var_name);
            }
            else
            {
                /* use VAR_NAME because we don't create new buffer for scatter */

                if (var_info._is_array_or_pointer)
                {
                    worker_input_buffer_init
                        << var_name << " = (" << c_type_str <<"*)malloc(" << size_str << ");";

                    worker_input_buffer_free
                        << "free(" << var_name << ");";
                }

                worker_recv_input
                    << "_RecvInputMsg((void*)" << ((var_info._is_array_or_pointer)? "" : "&")
                    << var_name << "," << size_str << ");";

                std::string bcast_vname = ((var_info._is_array_or_pointer)? "" : "&") + var_name;
                worker_scatter_input
                    << create_mpi_bcast(bcast_vname, var_info.get_mem_size(), mpi_type_str);

                if (var_info._is_array_or_pointer)
                {
                    // TODO: cl_write_input for shared all thread variable
                    // if constant write to constant
                    cl_write_input
                        << create_cl_enqueue_write_buffer("_gfn_cmd_queue",
                               var_cl_name, true, "0", size_str, var_name);
                }
                else
                {
                    // TODO: Pass to kernel parameter
                    cl_set_kernel_arg
                        << create_cl_set_kernel_arg("_kernel", kernel_arg_num++, c_type_str, var_name);
                }
            }
        }

        if (var_info._is_output)
        {
            if (var_info._is_array_or_pointer /* TODO: change to shared_dimension != 0 */)
            {
                worker_recv_input
                    << var_buf_name << " = (" << c_type_str
                    << "*)malloc(" << size_str << ");";

                worker_root_free_gen_var
                    << "free(" << var_buf_name << ");";

                recv_output
                    << "_RecvOutputMsg((void*)" << var_name
                    << "," << size_str << ");";

                worker_send_output
                    << "_SendOutputMsg((void*)" << var_buf_name
                    << "," << size_str << ");";

                worker_gather_output
                    << create_mpi_gatherv(var_name, var_sub_size, mpi_type_str,
                                          var_buf_name, var_cnts, var_disp,
                                          mpi_type_str);

                cl_read_output
                    << create_cl_enqueue_read_buffer("_gfn_cmd_queue",
                           var_cl_name, true, "0", full_expr_sub_size, var_name);
            }
            else
            {
                recv_output
                    << "_RecvOutputMsg((void*)&" << var_name
                    << "," << size_str << ");";

                worker_send_output
                    << "_SendOutputMsg((void*)&" << var_name
                    << "," << size_str << ");";
            }
        }

        if (var_info._is_reduction)
        {
            std::string local_reduce_var_name = "_local_" + var_name;
            worker_gen_var_decl
                // TODO: initialize value
                << c_type_str << " " << var_name << " = "
                << reduction_op_init_value(var_info._reduction_type) << ";";

            worker_gather_output
                << local_reduce_var_name << " = " << var_name << ";"
                << create_mpi_reduce(local_reduce_var_name, var_name,
                       "1", mpi_type_str, op_to_mpi_op(var_info._reduction_type));

            cl_kernel_var_decl
                << c_type_str << " " << var_name << " = "
                << reduction_op_init_value(var_info._reduction_type) << ";\n";

            cl_kernel_reduce_init_if
                << var_cl_local_mem_name << "[get_local_id(0)] = "
                << var_name << ";\n";

            cl_kernel_reduce_init_else
                << var_cl_local_mem_name << "[get_local_id(0)] = "
                << reduction_op_init_value(var_info._reduction_type) << ";\n";

            cl_kernel_reduce_local_reduce
                << var_cl_local_mem_name << "[get_local_id(0)] += "
                << var_cl_local_mem_name << "[get_local_id(0)+_stride];\n";

            cl_kernel_reduce_global_init
                << "*" << var_cl_global_reduction << " = "
                << reduction_op_init_value(var_info._reduction_type) << ";\n";

            cl_kernel_reduce_global_reduce
                << create_cl_help_atomic_call(var_cl_global_reduction, var_cl_local_mem_name,
                                              var_info._reduction_type, type);

            cl_set_kernel_arg
                << create_cl_set_kernel_arg("_kernel", kernel_arg_num++, c_type_str, "0");

            TL::Source cl_actual_param;
            cl_actual_param
                << "__local " << c_type_str << " * " << var_cl_local_mem_name;
            cl_actual_params.append_with_separator(cl_actual_param, ",");


            cl_read_output
                << create_cl_enqueue_read_buffer("_gfn_cmd_queue",
                    var_cl_name, true, "0", full_expr_sub_size, ("&" + var_name));
        }
    }

    /*== --------------- Replace code in loop body -----------------==*/
    std::vector<bool> replace_types(GFN_REPLACE_LAST_TYPE, false);
    replace_types[GFN_REPLACE_ARRAY_ND] = true;
	replace_types[GFN_REPLACE_ARRAY_INDEX] = true;
    replace_parallel_loop_body(loop_body, replace_types, induction_var_name, new_induction_var_name);

    /*== ----- Create MPI block distribution for statement ---------==*/
    std::cout << "Create MPI Block\n";
    mpi_block_dist_for_stmt
        << "for(" << induction_var.prettyprint() << " = "
        << local_start_idx_var << ";" << induction_var.prettyprint()
        << ((is_incre_loop)? "<" : ">") << local_end_idx_var << ";"
        << _for_stmt.get_iterating_expression() << ", "
        << new_induction_var_name << "+=_loop_step)" << loop_body;

    /*== ---------------- Create OpenCL kernel ---------------------==*/
    cl_decl_init_work_item_var
        << "size_t _work_item_num = _CalcSubSize(_loop_size, _gfn_num_proc, _gfn_rank, 1);"
        << "size_t _work_group_item_num = 64;"
        << "size_t _global_item_num = _GfnCalcGlobalItemNum(_work_item_num, _work_group_item_num);"
        << "cl_int " + local_cl_start_idx_var + " = " << local_start_idx_var << ";"
        << "cl_int " + local_cl_end_idx_var + " = " << local_end_idx_var << ";"
        << "cl_int " + cl_loop_step_var + " = " << loop_step_var << ";";

    // Add new start and end index to kernel argument
    cl_set_kernel_arg
        << create_cl_set_kernel_arg("_kernel", kernel_arg_num++, "cl_int", local_cl_start_idx_var);
    cl_set_kernel_arg
        << create_cl_set_kernel_arg("_kernel", kernel_arg_num++, "cl_int", local_cl_end_idx_var);
    cl_set_kernel_arg
        << create_cl_set_kernel_arg("_kernel", kernel_arg_num++, "cl_int", cl_loop_step_var);

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
            << ") / " << loop_step_var << ";\n"
        //<< "int _thread_id_dim_1 = get_global_id(1);"
        //<< "int _thread_id_dim_2 = get_global_id(2);"
		<< "int " << new_induction_var_name << " = "
		<< "get_global_id(0) * " << loop_step_var << ";\n"
        << "int " << induction_var << " = " << new_induction_var_name 
        <<" + " << local_start_idx_var << ";\n";
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
            << "if (get_global_id(0) < " << loop_size_var << ") {\n"
            << cl_kernel_reduce_init_if
            << "} else {\n"
            << cl_kernel_reduce_init_else
            << "}\n"
            // [Reduction Step] -
            << "for (int _stride = get_local_size(0)/2; _stride > 0; _stride /= 2) {\n"
            << "barrier(CLK_LOCAL_MEM_FENCE);\n"
            << "if (get_local_id(0) < _stride) {\n"
            << cl_kernel_reduce_local_reduce
            << "}\n"
            << "}\n"
            // [Reduction Step] - Initialize global sum
            << "if (get_global_id(0) == 0) {\n"
            << cl_kernel_reduce_global_init
            << "}\nbarrier(CLK_GLOBAL_MEM_FENCE);\n"
            // [Reduction Step] - Set subsum of local memory to global memory
            << "if (get_local_id(0) == 0) {\n"
            << cl_kernel_reduce_global_reduce
            << "}\n";
    }

    // Kernel main funcion
    cl_kernel
        << "__kernel void _kernel_" << int_to_string(_kernel_info->kernel_id)
        << "(" << cl_actual_params << ") {" << "\n"
        << cl_kernel_var_decl
            << "if (" << induction_var << " <= "
            << ((_kernel_info->_is_const_loop_upper_bound)? _kernel_info->_const_upper_bound : "ERRORVAR" ) << ") {\n"
                << loop_body << "\n"
            << "}\n"
        << cl_kernel_reduction
        << "}";

    cl_kernel_src_str
        << show_cl_source_in_comment(cl_kernel)
        << "const char *_kernel_" << int_to_string(_kernel_info->kernel_id) << "_src = "
        << source_to_kernel_str(cl_kernel) << ";";
    cl_var_decl
        << "cl_kernel _kernel;";
    cl_create_kernel
        << "_kernel = _CreateKernelFromSource(\"_kernel_"
        << int_to_string(_kernel_info->kernel_id) << "\",_kernel_"
        << int_to_string(_kernel_info->kernel_id) << "_src,_gfn_context,_gfn_device_id);";


    /*== -------------  Create GPU kernel function -----------------==*/
    TL::Source result, device_var_decl, memcpy_h2d, memcpy_d2h;
    TL::Source new_param_list;
    std::string kernel_name = get_new_kernel_name();

    // declare thread id variable
    TL::Source thread_id_decl = do_thread_id_declaration();
    TL::Source loop_index_decl = do_loop_index_declaration(induction_var.get_symbol(),
                                                           step,
                                                           lower_bound);
    TL::Source private_var_decl;
    TL::Source kernel_def, loop_body_replace;

    kernel_def
        << "__global__ void " << kernel_name
        << "(" << kernel_actual_param << ") { "
        << thread_id_decl
        << loop_index_decl
        //<< private_var_decl
        << "if (" << induction_var.prettyprint() << "<" << GFN_UPPER_BOUND << ")"
        << loop_body << "}";

    cl_launch_kernel
        << "_gfn_status = "
        << create_cl_enqueue_nd_range_kernel("_gfn_cmd_queue", "_kernel", "1", "0",
            "&_global_item_num", "&_work_group_item_num", "0", "0", "0")
        << create_gfn_check_cl_status("_gfn_status", "LAUNCH KERNEL");

#if 0
    TL::AST_t kernel_def_tree = kernel_def.parse_declaration(
            _function_def->get_point_of_declaration(),
            _function_def->get_scope_link());
    _function_def->get_ast().prepend_sibling_function(kernel_def_tree);
#endif

    if (enable_opencl == false)
    {
        TL::Source blank_source;
        cl_var_decl = blank_source;
        cl_init_var = blank_source;
        cl_write_input = blank_source;
        cl_create_kernel = blank_source;
        cl_set_kernel_arg = blank_source;
        cl_read_output = blank_source;
        cl_free_var = blank_source;
    }

    /*==----------------  Create worker function -------------------==*/
    worker_func_def
        << comment("*/ #ifdef GFN_WORKER /*")
            /* GPU */ << cl_kernel_src_str
        << "void _Function_" << int_to_string(_kernel_info->kernel_id) << " () {"
        << worker_var_decl
            /* GPU */ << cl_var_decl
        << comment("Generated variable")
        << worker_gen_var_decl

        << create_run_only_root_stmt( worker_recv_loop_size )
        << worker_scatter_loop_size

        << worker_input_buffer_init
        << create_run_only_root_stmt( worker_recv_input )
        << comment("Init generated variable")
        << worker_init_gen_var
            /* GPU */ << cl_init_var
        << comment("Send data to all process")
        << worker_scatter_input

        << comment("Compute work-load")
        << "if (0) {"
            << cl_decl_init_work_item_var
            << comment("TODO: Overlap node data transfer and device data transfer")
            << cl_write_input
            << cl_create_kernel
            << cl_set_kernel_arg
            << cl_launch_kernel
            << cl_read_output
        << "} else {"
            << mpi_block_dist_for_stmt
        << "}"

        << comment("Gather data from all process")
        << worker_gather_output
        << create_run_only_root_stmt( worker_send_output )
        << worker_free_gen_var
        << worker_input_buffer_free
        << create_run_only_root_stmt( worker_root_free_gen_var )
            /* GPU */ << cl_free_var
        << "}"
        << comment("*/ #endif /*");

    std::cout << " ================= Worker Function ================\n";
    std::cout << (std::string) worker_func_def << "\n";
    std::cout << " ==================================================\n";

    TL::AST_t worker_func_tree = worker_func_def.parse_declaration(
            _function_def->get_point_of_declaration(),
            _function_def->get_scope_link());
    _function_def->get_ast().prepend_sibling_function(worker_func_tree);


    send_call_func
        << send_loop_size
        << send_input
        << recv_output;

    return send_call_func;


#if 0
    /*==--------------------- GPU CG section -----------------------==*/
    ObjectList<DataReference> private_list = _kernel_info->get_private_list();

    // XXX: get all variable in for loop
    _var_list = loop_body.all_symbol_occurrences(TL::Statement::ONLY_VARIABLES);
    ObjectList<std::string> formal_param_list;
    for (ObjectList<IdExpression>::iterator it = _var_list.begin();
         it != _var_list.end();
         it++)
    {
        DataReference data_ref = it->get_expression();
        if (it->prettyprint() != induction_var.prettyprint() &&
            !contain(private_list, data_ref))
        {
            formal_param_list.push_back(GFN_DEVICE_PREFIX + (it->prettyprint()));
            kernel_param.append_with_separator(
                get_declaration(_construct, data_ref), ",");

            // XXX: declare device variable
            if (!is_already_declare(_for_stmt.get_scope(), it->prettyprint()))
            {
                device_var_decl
                    << get_declaration(_construct, data_ref, GFN_DEVICE_PREFIX)
                    << ";";
                add_to_scope(_for_stmt.get_scope(), it->prettyprint());
            }
        }
    }
    formal_param_list.push_back(upper_bound.prettyprint());
    kernel_param.append_with_separator("int " GFN_UPPER_BOUND, ",");

    // XXX: memcpy host to device
    for (ObjectList<DataReference>::iterator it = _kernel_info->_use_list.begin();
         it != _kernel_info->_use_list.end();
         ++it)
    {
        if (!contain(private_list, *it))
        {
            std::string var_name = it->get_base_symbol().get_name();
            std::string size = _kernel_info->_dim_size_list[var_name].get_mem_size();

            memcpy_h2d
                << do_gfn_memcpy_h2d(var_name, size);
        }
    }

    // XXX: memcpy device to host
    for (ObjectList<DataReference>::iterator it = _kernel_info->_def_list.begin();
         it != _kernel_info->_def_list.end();
         ++it)
    {
        if (!contain(private_list, *it))
        {
            std::string var_name = it->get_base_symbol().get_name();
            std::string size = _kernel_info->_dim_size_list[var_name].get_mem_size();

            memcpy_d2h
                << do_gfn_memcpy_d2h(var_name, size);
        }
    }

    // XXX: declare private variable
    private_var_decl
        << get_declarations_inline(_construct, private_list, "private");

    
    // XXX: create kernel body
    ObjectList<Statement> statement_list = loop_body.get_inner_statements();
    for (ObjectList<Statement>::iterator it = statement_list.begin();
         it != statement_list.end();
         it++)
    {
        loop_body_replace << *it;
    }




    /* Add device variable declaration to top of function */
    TL::AST_t device_var_decl_tree = device_var_decl.parse_declaration(
            _function_def->get_point_of_declaration(),
            _function_def->get_scope_link());
    _function_def->get_function_body().get_inner_statements()
            .front().get_ast().prepend(device_var_decl_tree);

    result
        << memcpy_h2d
        << kernel_name
        << do_kernel_config(lower_bound, upper_bound, step) /// <<< block, thread_p_block >>>
        << "(" << new_param_list << ");"
        << memcpy_d2h;
    
    for (ObjectList<std::string>::iterator it = formal_param_list.begin();
         it != formal_param_list.end();
         it++)
    {
        new_param_list.append_with_separator(*it, ",");
    }
#endif
    return result;
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
            //std::string dim1_size = _kernel_info->_var_info[idx]._size._dim1_size;
            std::string dim2_size = _kernel_info->_var_info[idx]._size._dim2_size;
            std::string dim3_size = _kernel_info->_var_info[idx]._size._dim3_size;

            // FIXME:
            if (subscript_count >= 1)
            {
                new_subscript = "(" + subscript_list[0] + ")";
            }
            if (subscript_count >= 2)
            {
                new_subscript = "((" + subscript_list[1] + ") * " +
                        dim3_size + ") + " + new_subscript;
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
