/*--------------------------------------------------------------------
  (C) Copyright 2006-2009 Barcelona Supercomputing Center 
                          Centro Nacional de Supercomputacion
  
  This file is part of Mercurium C/C++ source-to-source compiler.
  
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

#ifndef TL_NANOS_HPP
#define TL_NANOS_HPP

#include <string>

#include "tl-pragmasupport.hpp"

namespace TL
{
    namespace Nanos
    {
        class Version
        {
            public:
                const static int DEFAULT_VERSION;
                const static char* DEFAULT_FAMILY;

                static int version;
                static std::string family;

                static bool is_family(const std::string &family);
                static bool is_version(int version);
                static bool is_interface(const std::string &family, int version);
        };
        
        class Interface : public PragmaCustomCompilerPhase
        {
            public:
                Interface();
                void interface_preorder(PragmaCustomConstruct);
                void interface_postorder(PragmaCustomConstruct);

                virtual void run(TL::DTO& dto);

                ~Interface() { }
        };
    }
}

#endif // TL_NANOS_HPP