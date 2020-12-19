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



#ifndef GFN_DATA_HPP
#define GFN_DATA_HPP

#include "tl-langconstruct.hpp"
#include "gfn-common.hpp"
#include "gfn-transform.hpp"

namespace TL
{
    namespace GFN
    {
        //! \addtogroup GFN Griffon
        //! @{

        //! TODO descript class
        /*! 
          This class implements ...
          */
        class LIBGFN_CLASS Data : public BaseTransform
        {
            protected:
                virtual Source get_source();
            private:
                PragmaCustomConstruct _construct;
                Statement _stmt;
                TransferInfo *_transfer_info;
                
                ScopeLink _scope_link;
                AST_t _translation_unit;
                FILE* _kernel_decl_file;
                
                Source do_data();

            public:
                //! Creates a Data object
                /*!
                  \param 
                 */
                Data(PragmaCustomConstruct construct, 
                     Statement stmt, 
                     TransferInfo *transfer_info,
                     ScopeLink scope_link,
                     AST_t translation_unit,
                     FILE *kernel_decl_file);
        };

        //! Creates a Data object
        /*!
          \param for_stmt Regular loop
         */
        LIBGFN_EXTERN Data data(PragmaCustomConstruct construct,
                                Statement stmt, 
                                TransferInfo *transfer_info,
                                ScopeLink scope_link,
                                AST_t translation_unit,
                                FILE *kernel_decl_file);

        //! @}
    }
}

#endif // GFN_DATA_HPP
