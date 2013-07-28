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



#ifndef GFN_PARALLEL_FOR_HPP
#define GFN_PARALLEL_FOR_HPP

#include "tl-langconstruct.hpp"
#include "gfn-common.hpp"
#include "gfn-transform.hpp"

//#include "hlt-unroll-omp.hpp"

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
        class LIBGFN_CLASS ParallelFor : public BaseTransform
        {
            protected:
                virtual Source get_source();
            private:
                PragmaCustomConstruct _construct;
                ForStatement _for_stmt;
                FunctionDefinition* _function_def;
                KernelInfo *_kernel_info;
                
                ScopeLink _scope_link;
                AST_t _translation_unit;
                FILE* _kernel_decl_file;

                
                // FIXME: move to file scope
                ObjectList<std::string> _device_function_name_list;

                Source do_parallel_for();

                void extract_define_device_function(AST_t gpu_function);
                void replace_parallel_loop_body(Statement stmt,
                                                std::vector<bool> &replace_types,
												std::string old_idx_name="", 
												std::string new_idx_name="");
                void replace_parallel_loop_body(Expression expr,
                                                std::vector<bool> &replace_types,
												std::string old_idx_name="", 
												std::string new_idx_name="");
				
				void replace_loop_index_name(Expression expr, 
											 std::string old_name, std::string new_name);
                
                /// helper method
                bool contain(ObjectList<DataReference> &list, DataReference &obj);

            public:
                //! Creates a ParallelFor object
                /*!
                  \param for_stmt Regular loop
                 */
                ParallelFor(PragmaCustomConstruct construct, 
                            ForStatement for_stmt, 
                            KernelInfo *kernel_info,
                            ScopeLink scope_link,
                            AST_t translation_unit,
                            FILE *kernel_decl_file);
        };

        //! Creates a ParallelFor object
        /*!
          \param for_stmt Regular loop
         */
        LIBGFN_EXTERN ParallelFor parallel_for(PragmaCustomConstruct construct,
                                               ForStatement for_stmt, 
                                               KernelInfo *kernel_info,
                                               ScopeLink scope_link,
                                               AST_t translation_unit,
                                               FILE *kernel_decl_file);

        //! @}
    }
}

#endif // GFN_PARALLEL_FOR_HPP
