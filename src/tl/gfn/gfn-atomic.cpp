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



#include "gfn-atomic.hpp"
#include <sstream>

using namespace TL::GFN;

Atomic TL::GFN::atomic(TL::Expression assign_expr)
{
    return Atomic(assign_expr);
}


TL::Source Atomic::get_source()
{
    // Nothing at the moment
    return do_atomic();
}

Atomic::Atomic(Expression assign_expr)
     : _assign_expr(assign_expr)
{
    ;
}

TL::Source Atomic::do_atomic()
{
    TL::Source result, api_func_name, global_sum, local_sum;
    
    result 
        << api_func_name 
        << "(&"
        << global_sum
        << ","
        << local_sum
        << ");";
    
    switch (_assign_expr.get_operation_kind())
    {
        case Expression::ADDITION:
            api_func_name << GFN_GENERATED_PREFIX "AtomicAdd";
            break;
        case Expression::MULTIPLICATION:
            api_func_name << GFN_GENERATED_PREFIX "AtomicMul";
            break;
        case Expression::MINUS:
            api_func_name << GFN_GENERATED_PREFIX "AtomicSub";
            break;
        case Expression::BITWISE_AND:
            api_func_name << GFN_GENERATED_PREFIX "AtomicAnd";
            break;
        case Expression::BITWISE_OR:
            api_func_name << GFN_GENERATED_PREFIX "AtomicOr";
            break;
        case Expression::BITWISE_XOR:
            api_func_name << GFN_GENERATED_PREFIX "AtomicXor";
            break;
        case Expression::LOGICAL_AND:
            api_func_name << GFN_GENERATED_PREFIX "AtomicAndAnd";
            break;
        case Expression::LOGICAL_OR:
            api_func_name << GFN_GENERATED_PREFIX "AtomicOrOr";
            break;
        default:
            /// min max
            ///throw
            break;
    }
    
    global_sum
        << _assign_expr.get_first_operand();
    local_sum
        << _assign_expr.get_second_operand();

    return result;
}

