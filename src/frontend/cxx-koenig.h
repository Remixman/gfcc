/*
    Mercurium C/C++ Compiler
    Copyright (C) 2006-2007 - Roger Ferrer Ibanez <roger.ferrer@bsc.es>
    Barcelona Supercomputing Center - Centro Nacional de Supercomputacion
    Universitat Politecnica de Catalunya

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/
#ifndef CXX_KOENIG_H
#define CXX_KOENIG_H

#include "cxx-scope.h"
#include "cxx-buildscope.h"

MCXX_BEGIN_DECLS

char koenig_can_be_used(AST called_expression, decl_context_t decl_context);

scope_entry_list_t* koenig_lookup(
        int num_arguments,
        argument_type_info_t** argument_type_list, 
        decl_context_t decl_context);

MCXX_END_DECLS

#endif // CXX_KOENIG_H