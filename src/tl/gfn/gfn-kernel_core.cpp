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
}

KernelInfo::KernelInfo(std::string &kernel_name) :
    _has_reduction_clause(false)
{
  
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
        int min_size = (var_i._size._dim1_size == "1")? 1 : 100;//var_i._size._dim1_size;
        int min_idx = i;
        for (int j = i + 1; j < _var_info.size(); ++j)
        {
            VariableInfo var_j = _var_info[j];
            int j_size = (var_j._size._dim1_size == "1")? 1 : 100;//var_j._size._dim1_size;
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

void KernelInfo::set_accurate(GFN_ACCURATE accurate)
{
    _accurate = accurate;
}

GFN_ACCURATE KernelInfo::get_accurate() 
{
    return _accurate;
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
    std::string result = "";

    result += "(";
    result += "(" + _size._dim1_size + ")";
    if (_size._dim2_size != "1")
    {
        result += " * (" + _size._dim2_size + ")";
    }
    if (_size._dim3_size != "1")
    {
        result += " * (" + _size._dim3_size + ")";
    }
    result += ")";

    return result;
}

std::string VariableInfo::get_distributed_mem_size()
{
    std::string result = "";

    if (_shared_dimension == 1)
    {
        result = "(" + _size._dim1_size + ")";
    }
    else if (_shared_dimension == 2)
    {
        result = "(" + _size._dim2_size + ")";
    }
    else if (_shared_dimension == 3)
    {
        result = "(" + _size._dim3_size + ")";
    }

    return result;
}

std::string VariableInfo::get_distributed_mem_block()
{
    std::string result = "";

    result += "(";
    if (_shared_dimension == 1)
    {
        result += "(" + _size._dim2_size + ") * (" + _size._dim3_size + ")";
    }
    else if (_shared_dimension == 2)
    {
        result += "(" + _size._dim1_size + ") * (" + _size._dim3_size + ")";
    }
    else if (_shared_dimension == 3)
    {
        result += "(" + _size._dim1_size + ") * (" + _size._dim2_size + ")";
    }
    result += ")";

    return (result == "()")? "1" : result;
}

std::string VariableInfo::get_pointer_starts()
{
    // TODO: check dimension from variable TL::Type
    if (_size._dim3_size != "1") return " *** ";
    if (_size._dim2_size != "1") return " ** ";
    if (_is_array_or_pointer) return " * ";
    return " ";
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
        result = "(" + idx_name1 + ") * (" + _size._dim1_size
               + ") + (" + idx_name2 + ")";
    }
    else
    {
        result = "(" + idx_name1 + ")";
    }

    return result;
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
