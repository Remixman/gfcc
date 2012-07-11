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

KernelInfo::KernelInfo() : 
    _accurate(ACCURATE_HIGH)
{
}

KernelInfo::KernelInfo(std::string &kernel_name)
{
  
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
    if (get_use_list_index(data_ref.prettyprint()) == -1)
    {
        _use_list.push_back(data_ref);
    }
}

void KernelInfo::push_to_def_list(DataReference &data_ref)
{
    if (get_def_list_index(data_ref.prettyprint()) == -1)
    {
        _def_list.push_back(data_ref);
    }
}
