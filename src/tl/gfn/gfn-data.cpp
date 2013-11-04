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



#include "gfn-data.hpp"
#include "gfn-config.hpp"
#include "gfn-common.hpp"
#include <sstream>

using namespace TL::GFN;

Data TL::GFN::data(PragmaCustomConstruct construct, 
                   Statement stmt, 
                   TransferInfo *transfer_info,
                   ScopeLink scope_link,
                   AST_t translation_unit,
                   FILE *kernel_decl_file)
{
    return Data(construct, stmt, transfer_info, scope_link,
                translation_unit, kernel_decl_file);
}


TL::Source Data::get_source()
{
    // Nothing at this moment
    return do_data();
}

Data::Data(PragmaCustomConstruct construct, 
           Statement stmt, 
           TransferInfo *transfer_info,
           ScopeLink scope_link,
           AST_t translation_unit,
           FILE *kernel_decl_file)
     : _construct(construct), _stmt(stmt), 
       _transfer_info(transfer_info), _kernel_decl_file(kernel_decl_file)
{
     _scope_link = scope_link;
     _translation_unit = translation_unit;
}

TL::Source Data::do_data()
{
    bool debug = false;
    
    TL::Source worker_send_func_name, worker_recv_func_name;
    
    TL::Source ret_src;
    
    TL::Source master_send_call_function_send, master_send_call_function_recv;
    //TL::Source master_send_scalar_src;
    TL::Source master_send_src, master_recv_src;
    TL::Source master_lock_transfer_src, master_unlock_transfer_src;
    
    //TL::Source worker_recv_scalar_src;
    TL::Source worker_recv_src, worker_send_src;
    TL::Source worker_lock_transfer_src, worker_unlock_transfer_src;
    
    /* Send and recieve function source */
    TL::Source worker_send_function;
    TL::Source worker_recv_function;
    TL::Source worker_decl_var_src;
    TL::Source worker_decl_gen_var_src;
    TL::Source worker_allocate_src;
    TL::Source worker_free_src;
    
    /* Transfer information configuration */
    std::string level1_cond = "1";//_kernel_info->_level_1_condition;
    std::string level2_cond = "1";//_kernel_info->_level_2_condition;
    
    worker_send_func_name << "_Function_send_" << _transfer_info->send_func_id;
    worker_recv_func_name << "_Function_recv_" << _transfer_info->recv_func_id;

    for (int i = 0; i < _transfer_info->_var_info.size(); ++i)
    {
        VariableInfo var_info = _transfer_info->_var_info[i];
        DataReference var_ref = _transfer_info->_var_ref[i];
        
        /* Name for generated variable */
        std::string var_name = var_info.get_name();
        std::string var_unique_id_name = var_info.get_id_name();
        std::string var_cl_name = var_info.get_cl_name();
        std::string ptr_stars = var_info.get_pointer_starts();
        
        /* Datatype */
        TL::Type type = var_ref.get_type();
        std::string mpi_type_str = type_to_mpi_type(type);
        std::string c_type_str = type_to_ctype(type);
        std::string size_str = var_info.get_allocate_size_in_byte(type);
        
        // TODO: _GFN_MEM_ALLOC_HOST_PTR()
        std::string var_cl_mem_type;
        if (var_info._is_input && var_info._is_output)
            var_cl_mem_type = "_GFN_MEM_READ_WRITE()";
        else if (var_info._is_output)
            var_cl_mem_type = "_GFN_MEM_WRITE_ONLY()";
        else if (var_info._is_input)
            var_cl_mem_type = "_GFN_MEM_READ_ONLY()";
        /* TODO: temp var type ?? _GFN_MEM_READ_WRITE?? or local */
        
        /* (1). Declare array variables */
        if (var_info._is_array_or_pointer &&
            (var_info._is_input || var_info._is_output))
        {
            worker_decl_var_src
                << c_type_str << " " << ptr_stars << var_name << ";";
        }
        else if (var_info._is_array_or_pointer)
        {
            // TODO: private, create case
        }
        else
        {
            // scalar case
            worker_decl_var_src
                << c_type_str << " " << var_name << ";";
        }
        
        /* (2). Declaration generated variables */
        if (var_info._is_array_or_pointer &&
            (var_info._is_input || var_info._is_output))
        {
            worker_decl_gen_var_src
                << "long long " << var_unique_id_name << ";"
                << "cl_mem " << var_cl_name << " = 0;";
        }
        
        /* (XX). Allocate and Deallocate memory */
        if (var_info._is_array_or_pointer &&
            (var_info._is_input || var_info._is_output))
        {
            worker_allocate_src
                << create_gfn_malloc_nd(var_name, var_cl_name, var_unique_id_name,
                                        mpi_type_str, var_info._dimension_num, var_info._dim_size,
                                        var_cl_mem_type, level1_cond, level2_cond);
            
            worker_free_src
                << create_gfn_free(var_unique_id_name, level1_cond, level2_cond);
        }
        
        if (var_info._is_input)
        {
            if (var_info._is_array_or_pointer)
            {
                master_send_src
                    << "_SendInputMsg((void*)" << var_name 
                    << var_info.get_subscript_to_1d_buf() << "," << size_str << ");";
                    
                worker_recv_src
                    << create_gfn_q_bcast_nd(var_name, var_cl_name, mpi_type_str, 
                                            var_info._dimension_num, var_info._dim_size,
                                            level1_cond, level2_cond);
                    
                master_lock_transfer_src
                    << "_GfnLockTransfer((void*)" << var_name 
                    << var_info.get_subscript_to_1d_buf() << ");";
                    
                worker_lock_transfer_src
                    << "_GfnLockTransfer((void*)" << var_name 
                    << var_info.get_subscript_to_1d_buf() << ");";
            }
            else
            {
                master_send_src
                    << "_SendInputMsg((void*)&" << var_name << "," << size_str << ");";
                    
                worker_recv_src
                    << create_gfn_q_bcast_scalar(var_name, mpi_type_str);
            }
        }
        
        
        if (var_info._is_output)
        {
            if (var_info._is_array_or_pointer)
            {
                master_recv_src
                    << "_RecvOutputMsg((void*)" << ((var_info._is_array_or_pointer)? "" : "&")
                    << var_name << var_info.get_subscript_to_1d_buf() << "," << size_str << ");";
                    
                worker_send_src
                    << "_SendOutputMsg((void*)" << ((var_info._is_array_or_pointer)? "" : "&")
                    << var_name << var_info.get_subscript_to_1d_buf() << "," << size_str << ");";
                
                master_unlock_transfer_src
                    << "_GfnUnlockTransfer((void*)" << var_name 
                    << var_info.get_subscript_to_1d_buf() << ");";
                    
                worker_unlock_transfer_src
                    << "_GfnUnlockTransfer((void*)" << var_name 
                    << var_info.get_subscript_to_1d_buf() << ");";
            }
            else
            {
                master_recv_src
                    << "_RecvOutputMsg((void*)&" << var_name << "," << size_str << ");";
                    
                worker_send_src
                    << create_gfn_q_reduce_scalar(var_name, var_cl_name, mpi_type_str, op_to_mpi_op(var_info._reduction_type), 
                                                  "_global_item_num/_work_group_item_num", level1_cond, level2_cond);
            }
        }
    }
    
    master_send_call_function_send
        << "_SendCallFuncMsg(" << _transfer_info->send_func_id << ");";
        
    master_send_call_function_recv
        << "_SendCallFuncMsg(" << _transfer_info->recv_func_id << ");";
        
    /* Create master source */
    ret_src
        << comment("Send call send function message")
        << master_send_call_function_send
        << master_send_src
        << master_lock_transfer_src
        << _stmt
        << comment("Send call recieve function message")
        << master_send_call_function_recv
        << master_unlock_transfer_src
        << master_recv_src;
        
    /* Create send function source */
    worker_send_function
        << comment("SEND_FUNCTION " + int_to_string(_transfer_info->send_func_id) + " " + (std::string)worker_send_func_name)
        << "void " << worker_send_func_name << "() {"
            
            << comment("Declare Variables")
            << worker_decl_var_src
            
            << comment("Declare Generated Variables")
            << worker_decl_gen_var_src
            
            << comment("Allocate Array Memory")
            << worker_allocate_src
            
            << comment("Distribute Array Memory")
            << worker_recv_src
            
            << comment("Lock Transfer")
            << worker_lock_transfer_src
            
            << comment("Deallocate Array Memory")
            << worker_free_src
            
        << "}";
        
    print_to_kernel_decl_file(_scope_link, _translation_unit, _kernel_decl_file, worker_send_function);
    
    /* Create receive function source */
    worker_recv_function
        << comment("RECV_FUNCTION " + int_to_string(_transfer_info->recv_func_id) + " " + (std::string)worker_recv_func_name)
        << "void " << worker_recv_func_name << "() {"
            
            << comment("Declare Variables")
            << worker_decl_var_src
            
            << comment("Declare Generated Variables")
            << worker_decl_gen_var_src
            
            << comment("Allocate Array Memory")
            << worker_allocate_src
            
            << comment("Unlock Transfer")
            << worker_unlock_transfer_src
            
            << comment("Gather Array Memory")
            << worker_send_src
            
            << comment("Deallocate Array Memory")
            << worker_free_src
            
        << "}";
        
    print_to_kernel_decl_file(_scope_link, _translation_unit, _kernel_decl_file, worker_recv_function);

    return ret_src;
}
