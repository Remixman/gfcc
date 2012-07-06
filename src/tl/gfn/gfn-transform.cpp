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



#include "gfn-transform.hpp"
#include "gfn-exception.hpp"

using namespace TL;
using namespace TL::GFN;

bool TL::GFN::enable_instrumentation = false;

unsigned BaseTransform::_kernel_count = 0;
std::map< Scope, ObjectList<std::string> > BaseTransform::_device_var_decl_map;

BaseTransform::BaseTransform()
    : _identity(false), _ostream(std::cerr)
{
}

BaseTransform::BaseTransform(std::ostream &o)
    : _identity(false), _ostream(o)
{
}

BaseTransform::operator Source()
{
    return get_source_impl();
}

BaseTransform::operator std::string()
{
    return get_source_impl();
}

TL::Source BaseTransform::get_source_impl()
{
    if (_identity)
    {
        throw GFNException(_identity_tree, "invalid transformation");
    }
    else
    {
        return this->get_source();
    }
}

void BaseTransform::set_identity(AST_t tree)
{
    _identity = true;
    _identity_tree = tree;
}

std::string BaseTransform::get_new_kernel_name()
{
    std::stringstream kernel_name;
    kernel_name << "_gfn_kernel_" << _kernel_count;
    _kernel_count += 1;
    
    return kernel_name.str();
}

void BaseTransform::add_to_scope(Scope scope, std::string device_var)
{
    if (_device_var_decl_map.find(scope) == _device_var_decl_map.end())
    {
        ObjectList<std::string> new_var_list;
        _device_var_decl_map[scope] = new_var_list;
    }
    
    if (!_device_var_decl_map[scope].contains(device_var))
    {
        _device_var_decl_map[scope].push_back(device_var);
    }
}

bool BaseTransform::is_already_declare(Scope scope, std::string device_var)
{
    if ((_device_var_decl_map.find(scope) != _device_var_decl_map.end()) &&
        _device_var_decl_map[scope].contains(device_var))
    {
        return true;
    }
    
    return false;
}

Source BaseTransform::get_declaration(PragmaCustomConstruct &construct,
                                      DataReference data_ref,
                                      std::string name_prefix)
{
    Source declaration;

    Symbol sym = data_ref.get_symbol();
    Type type = sym.get_type();
    
    declaration
            << type.get_declaration(construct.get_scope(), name_prefix + sym.get_name());
    
    return declaration;
}

Source BaseTransform::get_declarations_inline(PragmaCustomConstruct &construct,
                                              ObjectList<DataReference> data_reference,
                                              std::string ref_type_name)
{
    Source declarations;
    
    for (ObjectList<DataReference>::iterator it = data_reference.begin();
         it != data_reference.end();
         it++)
    {
        declarations 
            << comment(ref_type_name + " entity : '" + it->get_symbol().get_qualified_name() + "'");
        declarations
            << get_declaration(construct, *it)
            << ";";
    }
    return declarations;
}
