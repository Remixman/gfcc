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

int KernelInfo::kernel_count = 0;

KernelInfo::KernelInfo() : 
    _accurate(ACCURATE_HIGH), _has_reduction_clause(false)
{
    // Loop bound
    _is_const_loop_upper_bound = false;
    _const_upper_bound = "";
}

KernelInfo::KernelInfo(std::string &kernel_name) :
    _has_reduction_clause(false)
{
    // Loop bound
    _is_const_loop_upper_bound = false;
    _const_upper_bound = "";
}

KernelInfo::~KernelInfo()
{
}

int KernelInfo::get_var_info_index_from_var_name(std::string var_name)
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

void KernelInfo::sort_var_info()
{
    for (int i = 0; i < _var_info.size(); ++i)
    {
        VariableInfo var_i = _var_info[i];
        int min_size = (var_i._dim_size[1] == "1")? 1 : 100;
        int min_idx = i;
        for (int j = i + 1; j < _var_info.size(); ++j)
        {
            VariableInfo var_j = _var_info[j];
            int j_size = (var_j._dim_size[1] == "1")? 1 : 100;
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

void KernelInfo::set_private_list(ObjectList<DataReference> &private_list)
{
    _private_list = private_list;
}

ObjectList<DataReference> KernelInfo::get_private_list()
{
    return _private_list;
}

int KernelInfo::get_use_list_index(std::string var)
{
    int i = 0;
    for (ObjectList<DataReference>::iterator it = _use_list.begin();
         it != _use_list.end();
         ++it, ++i)
    {
        if (it->get_base_symbol().get_name() == var)
        {
            return i;
        }
    }
    return -1;
}

int KernelInfo::get_def_list_index(std::string var)
{
    int i = 0;
    for (ObjectList<DataReference>::iterator it = _def_list.begin();
         it != _def_list.end();
         ++it, ++i)
    {
        if (it->get_base_symbol().get_name() == var)
        {
            return i;
        }
    }
    return -1;
}

void KernelInfo::push_to_use_list(DataReference &data_ref)
{
    if (loop_index_var_name == data_ref.get_base_symbol().get_name())
    {
        return;
    }

    if (get_use_list_index(data_ref.get_base_symbol().get_name()) == -1)
    {
        _use_list.push_back(data_ref);
    }
}

void KernelInfo::push_to_def_list(DataReference &data_ref)
{
    if (get_def_list_index(data_ref.get_base_symbol().get_name()) == -1)
    {
        _def_list.push_back(data_ref);
    }
}


std::string VariableInfo::get_mem_size()
{
    std::stringstream result;

    result << "(";
    result << "(" << _dim_size[1] << ")";
    if (_dimension_num >= 2) result << " * (" << _dim_size[2] << ")";
    if (_dimension_num >= 3) result << " * (" << _dim_size[3] << ")";
    if (_dimension_num >= 4) result << " * (" << _dim_size[4] << ")";
    if (_dimension_num >= 5) result << " * (" << _dim_size[5] << ")";
    if (_dimension_num >= 6) result << " * (" << _dim_size[6] << ")";
    result << ")";

    return result.str();
}

std::string VariableInfo::get_distributed_mem_size()
{
    std::string result = "";

    // FIXME:
    if (_shared_dimension == 1)
    {
        result = "(" + _dim_size[1] + ")";
    }
    else if (_shared_dimension == 2)
    {
        result = "(" + _dim_size[2] + ")";
    }
    else if (_shared_dimension == 3)
    {
        result = "(" + _dim_size[3] + ")";
    }

    return result;
}

std::string VariableInfo::get_distributed_mem_block()
{
    std::stringstream result;
    
    result << "(";
    for (int i = 1; i <= _dimension_num; ++i)
    {
        if (i != 1) result << "*";
        if (i != _shared_dimension) result << "(" << _dim_size[i] << ")";
    }
    result << ")";
    
    return (result.str() == "()")? "1" : result.str();
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
        default: std::cerr << "Unsupport multi-dimension\n";
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

std::string VariableInfo::get_array_index_in_1D(std::string idx_name1,
                                                std::string idx_name2,
                                                std::string idx_name3)
{
    std::string result;

    if (idx_name3 != "")
    {
        std::cerr << "TODO : " << __FILE__ << " : " << __LINE__ << std::endl;
    }
    else if (idx_name2 != "")
    {
        result = "(" + idx_name1 + ") * (" + _dim_size[1]
               + ") + (" + idx_name2 + ")";
    }
    else
    {
        result = "(" + idx_name1 + ")";
    }

    return result;
}

std::string VariableInfo::get_allocate_size_in_byte(TL::Type vartype)
{
    std::stringstream result;
    std::string type_name = type_to_ctype(vartype);
    
    if (vartype.is_array() || vartype.is_pointer())
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
    std::cout << _name << " prop[";
    if (_is_input) std::cout << "IN/";
    if (_is_output) std::cout << "OUT/";
    if (_is_index) std::cout << "IDX/";
    if (_is_reduction) std::cout << "REDUC/";
    std::cout << "]" << std::endl;
}
