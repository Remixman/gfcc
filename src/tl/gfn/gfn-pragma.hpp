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



#ifndef GFN_PRAGMA_HPP
#define GFN_PRAGMA_HPP

#include "tl-pragmasupport.hpp"
#include "gfn-kernel_core.hpp"

namespace TL
{
    namespace GFN
    {
        //! \addtogroup GFN High Level Transformations
        //! @{

        //! Compiler phase that implements a pragma interface to the GFN transformations
        /*!
          This class implements several pragmas

          \code
#pragma gfn unroll factor(N)
  regular-for-loop
          \endcode
          \sa TL::HLT::LoopUnroll


          */
        class GFNPragmaPhase : public PragmaCustomCompilerPhase
        {
            public:
                GFNPragmaPhase();
                virtual void run(TL::DTO& dto);
            private:
                void start(PragmaCustomConstruct construct);
                void finish(PragmaCustomConstruct construct);
                void parallel_for(PragmaCustomConstruct construct);
                void overlapcompute(PragmaCustomConstruct construct);
                void barrier(PragmaCustomConstruct construct);
                void atomic(PragmaCustomConstruct construct);

                void get_kernelname_clause(PragmaCustomConstruct construct,
                                           KernelInfo *kernel_info);
                void get_waitfor_clause(PragmaCustomConstruct construct,
                                        KernelInfo *kernel_info);
                void get_private_clause(PragmaCustomConstruct construct,
                                        KernelInfo *kernel_info,
                                        ObjectList<IdExpression> &symbol_list);
                void get_accurate_clause(PragmaCustomConstruct construct,
                                         KernelInfo *kernel_info);
                void get_reduction_clause(PragmaCustomConstruct construct,
                                          KernelInfo *kernel_info);
                void get_size_clause(PragmaCustomConstruct construct,
                                     KernelInfo *kernel_info);
                void get_input_clause(PragmaCustomConstruct construct,
                                      KernelInfo *kernel_info);
                void get_output_clause(PragmaCustomConstruct construct,
                                       KernelInfo *kernel_info);
                void get_copy_clause(PragmaCustomClause &copy_clause,
                                     KernelInfo *kernel_info,
                                     std::string copy_type_str);
                void get_parallel_if_clause(PragmaCustomConstruct construct,
                                            KernelInfo *kernel_info);

                // TODO: Change name to gather_data_for_analysis (and write data that it graps)
                void find_use_and_def_list(Statement compound_stmt,
                                           KernelInfo *kernel_info);
                void collect_variable_info(Statement stmt,
                                           KernelInfo *kernel_info);
                void collect_variable_info(Expression expr,
                                           KernelInfo *kernel_info,
                                           int found_idx_at = 0 // For _shared_dimension
                        );
                void collect_loop_info(ForStatement for_stmt,
                                       KernelInfo *kernel_info);

                ScopeLink _scope_link;
                AST_t _translation_unit;
        };

        //! @}
    }

}

#endif // GFN_PRAGMA_HPP
