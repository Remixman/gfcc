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
            // (0 is shared and must boardcast to all node)
            unsigned        _shared_dimension;
            unsigned        _dimension_num; // for scalar dim num equal 0
            std::string     _dim_size[7];
            
            // Stencil pattern
            int             _in_pattern_type;
            int             _out_pattern_type;
            ObjectList<std::string> _in_pattern_array;
            ObjectList<std::string> _out_pattern_array;

            // TODO: change to bit
            bool            _is_input;
            bool            _is_output;
            bool            _is_index;
            bool            _is_reduction;
            bool            _is_array_or_pointer;
            bool            _is_use;
            bool            _is_prop_use; // e.g. pass reference to function
            bool            _is_def;
            bool            _is_loop_variable; // use in for expr e.g. 'n' in for(i=0;i<n;i++)

            VariableInfo(std::string n) :
                _name(n), _access_type(VAR_ACCESS_SHARED),
                _reduction_type(REDUCTION_UNKNOWN), _shared_dimension(0), _dimension_num(0),
                _is_input(0), _is_output(0), _is_index(0), _is_reduction(0),
                _is_array_or_pointer(0), _is_use(0), _is_prop_use(0), _is_def(0), _is_loop_variable(0),
                _in_pattern_type(GFN_PATTERN_NONE), _out_pattern_type(GFN_PATTERN_NONE) {
                for (int i = 0; i < 7; i++) _dim_size[i] = "1";
            }

            // Code generated function
            std::string get_mem_size();
            std::string get_distributed_mem_size();
            std::string get_distributed_mem_block();
            std::string get_pointer_starts();
            std::string get_subscript_to_1d_buf();
            std::string get_array_index_in_1D(std::string idx_name1,
                                              std::string idx_name2 = "",
                                              std::string idx_name3 = "");
            std::string get_allocate_size_in_byte(TL::Type vartype);

            void print();
        };

        //! GFN Kernel Infomation
        /*!
          .........
         */
        struct LIBGFN_CLASS KernelInfo
        {
            private:
                std::string _kernel_name;

                ObjectList<std::string> _wait_for;
                ObjectList<std::string> _all_var;
                
                ObjectList<DataReference> _private_list;

                ObjectList<std::string> _shared_var;
                
                Source _thread_num;
                Source _block_num;
                Source _thread_per_block;

            public:
                //operator Source();
                //operator std::string();

                static int kernel_count;
                int kernel_id;

                ObjectList<DataReference> _var_ref;
                ObjectList<VariableInfo> _var_info;
                int get_var_info_index_from_var_name(std::string var_name);
                void sort_var_info();

                KernelInfo();
                KernelInfo(std::string &kernel_name);
                ~KernelInfo();
                
                void set_kernel_name(std::string &name);
                std::string get_kernel_name();
                void set_thread_num(Source thread_num);
                Source get_thread_num();
                void set_wait_for(ObjectList<std::string> &wait_for);
                ObjectList<std::string> get_wait_for();

                void set_private_list(ObjectList<DataReference> &private_list);
                ObjectList<DataReference> get_private_list();

                int get_use_list_index(std::string var);
                int get_def_list_index(std::string var);
                void push_to_use_list(DataReference &data_ref);
                void push_to_def_list(DataReference &data_ref);
                
                // return empty if don't have partitioned shared array
                std::string get_full_size();
                
                static std::map< std::string, KernelInfo > _kernel_map;

                std::string loop_index_var_name;
                bool _has_reduction_clause;
                ObjectList<DataReference> _use_list;
                ObjectList<DataReference> _def_list;

                std::string _level_1_condition;
                std::string _level_2_condition;
                
                // Loop iterator
                bool _is_const_loop_upper_bound;
                std::string _const_upper_bound; // if const
        };
        //! @}
    }
}

#endif // GFN_KERNEL_CORE_HPP
