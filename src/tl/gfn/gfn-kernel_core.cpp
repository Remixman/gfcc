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
using namespace TL::GFN;

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

void KernelInfo::set_thread_num(TL::Source thread_num)
{
    _thread_num = thread_num;
}

TL::Source KernelInfo::get_thread_num()
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

void KernelInfo::set_private_list(ObjectList<TL::DataReference> &private_list)
{
    _private_list = private_list;
}

ObjectList<TL::DataReference> KernelInfo::get_private_list()
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

