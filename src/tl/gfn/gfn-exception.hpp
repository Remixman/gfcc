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



#ifndef GFN_EXCEPTION_HPP
#define GFN_EXCEPTION_HPP

#include "gfn-common.hpp"
#include "tl-ast.hpp"
#include "tl-langconstruct.hpp"
#include <iostream>

namespace TL
{
    namespace GFN
    {
        //! \addtogroup GFN Girffon
        //! @{
        //! Exception throwable inside GFN code
        struct LIBGFN_CLASS GFNException
        {
            public:
                GFNException(TL::LangConstruct place, const std::string& message)
                    : _ast(place.get_ast()), _message(message)
                {
                }
                GFNException(TL::AST_t place, const std::string& message)
                    : _ast(place), _message(message)
                {
                }
                GFNException(const std::string& message)
                    : _ast(NULL), _message(message)
                {
                }
            private:
                TL::AST_t _ast;
                std::string _message;

                friend std::ostream& operator<<(std::ostream &o, const GFNException&);
        };

        LIBGFN_EXTERN std::ostream& operator<<(std::ostream &o, const GFNException& e);
        //! @}
    }
}


#endif // GFN_EXCEPTION_HPP
