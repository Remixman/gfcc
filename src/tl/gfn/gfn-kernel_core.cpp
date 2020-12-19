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



#include "gfn-kernel_core.hpp"
#include "gfn-exception.hpp"


using namespace TL;
using namespace GFN;


int TransferInfo::get_var_info_index_from_var_name(std::string var_name)
{
    for (int i = 0; i < _var_info.size(); ++i)
    {
        if (var_name == _var_info[i]._name)
        {
            return i;
        }
    }
    return -1;
}

void TransferInfo::sort_var_info()
{
    for (int i = 0; i < _var_info.size(); ++i)
    {
        VariableInfo var_i = _var_info[i];
        int min_size = (var_i._dimension_num == 0)? 1 : 100;
        int min_idx = i;
        for (int j = i + 1; j < _var_info.size(); ++j)
        {
            VariableInfo var_j = _var_info[j];
            int j_size = (var_j._dimension_num == 0)? 1 : 100;
            if (j_size < min_size)
            {
                min_size = j_size;
                min_idx = j;
            }
        }

        VariableInfo var_tmp = _var_info[i];
        _var_info[i] = _var_info[min_idx];
        _var_info[min_idx] = var_tmp;

        DataReference ref_tmp = _var_ref[i];
        _var_ref[i] = _var_ref[min_idx];
        _var_ref[min_idx] = ref_tmp;
    }
}

VariableInfo& TransferInfo::get_var_info(std::string var_name)
{
    int idx = get_var_info_index_from_var_name(var_name);
    
    if (idx >= 0)
    {
        return _var_info[idx];
    }
    else
    {
        std::cerr << "Error : get_var_info " << __FILE__ << ":" << __LINE__ << "\n";
    }
}

KernelInfo::KernelInfo() : 
    _has_reduction_clause(false), _has_inner_loop(false)
{
    // Loop bound
    _is_const_loop_upper_bound = false;
    _const_upper_bound = "";
    
    // Default is enable all parallel level
    _level_1_condition = "1";
    _level_2_condition = "1";
}

KernelInfo::KernelInfo(std::string &kernel_name) :
    _has_reduction_clause(false), _has_inner_loop(false)
{
    // Loop bound
    _is_const_loop_upper_bound = false;
    _const_upper_bound = "";
    
    // Default is enable all parallel level
    _level_1_condition = "1";
    _level_2_condition = "1";
}

KernelInfo::~KernelInfo()
{
}

void KernelInfo::set_kernel_name(std::string &name)
{
    // XXX: find in _kernel_map
    //throw
}

std::string KernelInfo::get_kernel_name()
{
    if (_kernel_name == "")
    {
        //throw 
    }
    return _kernel_name;
}

void KernelInfo::set_thread_num(Source thread_num)
{
    _thread_num = thread_num;
}

Source KernelInfo::get_thread_num()
{
    return _thread_num;
}

void KernelInfo::set_wait_for(ObjectList<std::string> &wait_for)
{
    _wait_for = wait_for;
    for (ObjectList<std::string>::iterator it = _wait_for.begin();
         it != _wait_for.end();
         it++)
    {
        *it = GFN_GENERATED_PREFIX + *it;
    }
}

ObjectList<std::string> KernelInfo::get_wait_for()
{
    return _wait_for;
}

std::string VariableInfo::get_name()
{
    return _name;
}

std::string VariableInfo::get_id_name()
{
    return ("_id_" + _name);
}

std::string VariableInfo::get_cl_name()
{
    return ("_cl_mem_" + _name);
}

std::string VariableInfo::get_mem_size()
{
    if (_dimension_num <= 0 || _dimension_num >= 7) 
    {
        std::cerr << "Don't support memory size for " << _name << "\n";
        return "1";
    }
    
    std::stringstream result;

    result << "(";
    result << "(" << (std::string)_dim_size[0] << ")";
    if (_dimension_num >= 2) result << " * (" << (std::string)_dim_size[1] << ")";
    if (_dimension_num >= 3) result << " * (" << (std::string)_dim_size[2] << ")";
    if (_dimension_num >= 4) result << " * (" << (std::string)_dim_size[3] << ")";
    if (_dimension_num >= 5) result << " * (" << (std::string)_dim_size[4] << ")";
    if (_dimension_num >= 6) result << " * (" << (std::string)_dim_size[5] << ")";
    result << ")";

    return result.str();
}

TL::Source VariableInfo::get_distributed_mem_size()
{
    TL::Source result;
    result << "(" << _dim_size[_shared_dimension] << ")";
    return result;
}

TL::Source VariableInfo::get_distributed_mem_block()
{
    TL::Source result;
    
    result << "(";
    if (_dimension_num > 0)
    {
        for (int i = 0; i < _dimension_num; ++i)
        {
            if (i != 0) result << "*";
            if (i != _shared_dimension) result << "(" << _dim_size[i] << ")";
        }
    }
    else
    {
        result << "1";
    }
    result << ")";
    
    return result;
}

std::string VariableInfo::get_pointer_starts()
{
    if ( ! _is_array_or_pointer ) return " ";

    switch(_dimension_num)
    {
        case 1: return " * ";
        case 2: return " ** ";
        case 3: return " *** ";
        case 4: return " **** ";
        case 5: return " ***** ";
        case 6: return " ****** ";
        default: std::cerr << "Unsupport multi-dimension ," 
            << _name << " has " << _dimension_num << " dimension\n";
    }

    return " ";
}

std::string VariableInfo::get_subscript_to_1d_buf()
{
    switch(_dimension_num)
    {
        case 2: return "[0]";
        case 3: return "[0][0]";
        case 4: return "[0][0][0]";
        case 5: return "[0][0][0][0]";
        case 6: return "[0][0][0][0][0]";
        default: return "";
    }

    return "";
}

TL::Source VariableInfo::get_array_index_in_1D(std::string idx_name1,
                                                std::string idx_name2,
                                                std::string idx_name3)
{
    TL::Source result;

    if (idx_name3 != "")
    {
        std::cerr << "TODO : " << __FILE__ << " : " << __LINE__ << std::endl;
    }
    else if (idx_name2 != "")
    {
        result 
            << "(" << idx_name1 << ") * (" << _dim_size[1] << ") + (" << idx_name2 << ")";
    }
    else
    {
        result 
            << "(" << idx_name1 << ")";
    }

    return result;
}

std::string VariableInfo::get_allocate_size_in_byte(TL::Type vartype)
{
    std::stringstream result;
    std::string type_name = type_to_ctype(vartype);
    
    if (_is_array_or_pointer)
    {
        result << "(sizeof(" << type_name << ") * " << get_mem_size() << ")";
    }
    else
    {
        result << "sizeof(" << type_name << ")";
    }
    
    return result.str();
}

void VariableInfo::print()
{
    std::stringstream prop_str;
    
    prop_str << (_name + "\t\tprop[");
    if (_is_use) prop_str << " USE |";
    if (_is_def) prop_str << " DEF |";
    if (_is_def_before_use) prop_str << " DEF_BEF_USE |";
    if (_is_input) prop_str << " IN |";
    if (_is_output) prop_str << " OUT |";
    if (_is_index) prop_str << " IDX |";
    if (_is_reduction) prop_str << " REDUC |";
    //prop_str = prop_str.str().substr(0,prop_str.str().size()-1);
    prop_str << "\b]\n\t\t";
    
    if (_is_private) prop_str << "PRIVATE";
    else prop_str << "SHARED";
    
    if (_is_array_or_pointer) {
        prop_str << " " << _dimension_num 
            << "D ARRAY (SHARED AT " << _shared_dimension << ")";
    } else {
        prop_str << " SCALAR";
    }
    
    std::cout << prop_str.str() << std::endl;
}
