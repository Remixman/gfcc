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
    
    TL::Source ret_src;
    
    TL::Source master_send_call_function_send, master_send_call_function_recv;
    //TL::Source master_send_scalar_src;
    TL::Source master_send_src, master_recv_src;
    TL::Source master_lock_transfer_src, master_unlock_transfer_src;
    
    //TL::Source worker_recv_scalar_src;
    TL::Source worker_recv_src, worker_send_src;
    TL::Source worker_lock_transfer_src, worker_unlock_transfer_src;
    
    // Transfer information configuration
    std::string level1_cond = "1";//_kernel_info->_level_1_condition;
    std::string level2_cond = "1";//_kernel_info->_level_2_condition;
    

    for (int i = 0; i < _transfer_info->_var_info.size(); ++i)
    {
        VariableInfo var_info = _transfer_info->_var_info[i];
        DataReference var_ref = _transfer_info->_var_ref[i];
        
        /* Name for generated variable */
        std::string var_name = var_info.get_name();
        std::string var_unique_id_name = var_info.get_id_name();
        std::string var_cl_name = var_info.get_cl_name();
        
        /* Datatype */
        TL::Type type = var_ref.get_type();
        std::string mpi_type_str = type_to_mpi_type(type);
        std::string c_type_str = type_to_ctype(type);
        std::string size_str = var_info.get_allocate_size_in_byte(type);
        
        
        if (var_info._is_input)
        {
            master_send_src
                << "_SendInputMsg((void*)" << var_name 
                << var_info.get_subscript_to_1d_buf() << "," << size_str << ");";
                
            worker_recv_src
                << create_gfn_q_bcast_nd(var_name, var_cl_name, mpi_type_str, 
                                         var_info._dimension_num, var_info._dim_size,
                                         level1_cond, level2_cond);
        }
        
        
        if (var_info._is_output)
        {
            master_recv_src
                << "_RecvOutputMsg((void*)" << ((var_info._is_array_or_pointer)? "" : "&")
                << var_name << var_info.get_subscript_to_1d_buf() << "," << size_str << ");";
                
            /*worker_send_src
                <<*/
        }
    }
    
    master_send_call_function_send
        << "_SendCallFuncMsg(3);"; // TODO: random number
        
    master_send_call_function_recv
        << "_SendCallFuncMsg(3);"; // TODO: random number
        
        
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

    return ret_src;
}
