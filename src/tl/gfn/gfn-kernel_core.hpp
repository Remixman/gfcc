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



#ifndef GFN_KERNEL_CORE_HPP
#define GFN_KERNEL_CORE_HPP

#include "tl-pragmasupport.hpp"
#include "tl-ast.hpp"
#include "tl-datareference.hpp"
#include "tl-source.hpp"
#include "gfn-common.hpp"
#include <string>
#include <stack>
#include <map>
#include <iostream>
#include <sstream>

namespace TL
{
    //! Griffon for TL
    namespace GFN
    {
        //! \addtogroup GFN Griffon
        //! @{

        struct LIBGFN_CLASS VariableInfo
        {
            std::string     _name;
            VAR_ACCESS_T    _access_type;
            REDUCTION_T     _reduction_type;
            // For multi-dimension var, what dimension that we must distributed
            // (0 is shared and must broadcast to all node)
            int             _shared_dimension;
            unsigned        _dimension_num; // for scalar dim num equal 0
            TL::ObjectList<TL::Expression> _subarray_start;
            TL::ObjectList<TL::Expression> _subarray_end;
            TL::ObjectList<TL::Expression> _alloc_start;
            TL::ObjectList<TL::Expression> _alloc_end;
            TL::ObjectList<TL::Expression> _dim_size; // TODO: remove this
            
            // Stencil pattern
            int             _in_pattern_type;
            int             _out_pattern_type;
            TL::ObjectList<std::string> _in_pattern_array;
            TL::ObjectList<std::string> _out_pattern_array;

            // TODO: change to bit
            bool            _is_input;
            bool            _is_output;
            bool            _is_temp_array;
            bool            _is_index;
            bool            _is_present;
            bool            _is_private;
            bool            _is_reduction;
            bool            _is_array_or_pointer;
            bool            _is_loop_variable; // use in for expr e.g. 'n' in for(i=0;i<n;i++)
            
            bool            _is_use;
            bool            _is_prop_use; // e.g. pass reference to function
            bool            _is_def;
            bool            _is_def_before_use;
            

            VariableInfo(std::string n) :
                _name(n), _access_type(VAR_ACCESS_SHARED),
                _reduction_type(REDUCTION_UNKNOWN), _shared_dimension(-1), _dimension_num(0),
                _is_input(0), _is_output(0), _is_temp_array(0), _is_index(0), _is_present(0), 
                _is_private(0), _is_reduction(0), _is_array_or_pointer(0), _is_loop_variable(0), 
                _is_use(0), _is_prop_use(0), _is_def(0), _is_def_before_use(0),
                _in_pattern_type(GFN_PATTERN_NONE), _out_pattern_type(GFN_PATTERN_NONE) {
            }

            std::string get_name();
            std::string get_id_name();
            std::string get_cl_name();
            
            // Code generated function
            std::string get_mem_size();
            TL::Source get_distributed_mem_size();
            TL::Source get_distributed_mem_block();
            std::string get_pointer_starts();
            std::string get_subscript_to_1d_buf();
            TL::Source get_array_index_in_1D(std::string idx_name1,
                                              std::string idx_name2 = "",
                                              std::string idx_name3 = "");
            std::string get_allocate_size_in_byte(TL::Type vartype);

            void print();
        };

        //! GFN Transfer Infomation
        /*!
          .........
         */
        struct LIBGFN_CLASS TransferInfo
        {
            public:
                TransferInfo() {}
                virtual ~TransferInfo() {}
                
                int send_func_id;
                int recv_func_id;
                
                ObjectList<DataReference> _var_ref;
                ObjectList<VariableInfo> _var_info;
                
                int get_var_info_index_from_var_name(std::string var_name);
                void sort_var_info();
                VariableInfo& get_var_info(std::string var_name);
        };
        
        //! GFN Kernel Infomation
        /*!
          .........
         */
        struct LIBGFN_CLASS KernelInfo : public LIBGFN_CLASS TransferInfo
        {
            private:
                std::string _kernel_name;

                ObjectList<std::string> _wait_for;
                
                Source _thread_num;
                Source _block_num;
                Source _thread_per_block;

            public:
                int kernel_id;

                KernelInfo();
                KernelInfo(std::string &kernel_name);
                ~KernelInfo();
                
                void set_kernel_name(std::string &name);
                std::string get_kernel_name();
                void set_thread_num(Source thread_num);
                Source get_thread_num();
                void set_wait_for(ObjectList<std::string> &wait_for);
                ObjectList<std::string> get_wait_for();
                
                static std::map< std::string, KernelInfo > _kernel_map;

                std::string loop_index_var_name;
                bool _has_reduction_clause;

                std::string _level_1_condition;
                std::string _level_2_condition;
                
                // Loop iterator
                bool _is_const_loop_upper_bound;
                std::string _const_upper_bound; // if const
                
                // Inner loop information
                bool _has_inner_loop;
                std::string _inner_lower_bound_expr;
                std::string _inner_upper_bound_expr;
                std::string _inner_loop_step;
                std::string _inner_induction_var;
        };
        //! @}
    }
}

#endif // GFN_KERNEL_CORE_HPP
