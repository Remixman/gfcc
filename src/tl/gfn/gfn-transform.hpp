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



#ifndef GFN_TRANSFORM_HPP
#define GFN_TRANSFORM_HPP

#include "tl-ast.hpp"
#include "tl-source.hpp"
#include "gfn-common.hpp"
#include "gfn-exception.hpp"
#include "gfn-kernel_core.hpp"
#include <string>
#include <iostream>
#include <map>

namespace TL
{
    //! Griffon for TL
    namespace GFN
    {
        //! \addtogroup GFN Griffon
        //! @{

        //! Base for all transformations
        /*!
          All GFN transformations derive from this class.
         */
        struct LIBGFN_CLASS BaseTransform
        {
            private:
                AST_t _identity_tree;
                bool _identity;
                Source get_source_impl();

                static unsigned _kernel_count;
                static std::map< Scope, ObjectList<std::string> > _device_var_decl_map;

            protected:
                //! Every subclass of GFN will implement this
                /*!
                   This function implements the transformation and returns
                   the transformed code
                 */
                virtual Source get_source() = 0;
                /*!
                  Sets that this transformation will not do anything but
                  returning \a tree as identity code

                  \param tree Returned code representing the identity of the transformation
                 */
                void set_identity(AST_t tree);

                /*!
                 */
                std::string get_new_kernel_name();
                
                void add_to_scope(Scope scope, std::string device_var);
                bool is_already_declare(Scope scope, std::string device_var);
                
                Source get_declaration(PragmaCustomConstruct &construct,
                                       DataReference data_ref,
                                       std::string name_prefix = "");
                Source get_declarations_inline(PragmaCustomConstruct &construct,
                                               ObjectList<DataReference> data_reference,
                                               std::string ref_type_name);
                
                //! Where warnings or error messages can be written
                /*!
                  By default this is std::cerr
                 */
                std::ostream &_ostream;
            public:
                 operator Source();
                 operator std::string();

                 BaseTransform();
                 //! Constructs a BaseTransform specifying the output stream
                 BaseTransform(std::ostream &o);
                 virtual ~BaseTransform() { }
        };

        extern bool enable_instrumentation;
        //! @}
    }
}

#endif // GFN_TRANSFORM_HPP
