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
                /* Directives */
                void parallel_for(PragmaCustomConstruct construct);
                void data(PragmaCustomConstruct construct);
                void data_post(PragmaCustomConstruct construct);
                void fission_loop(PragmaCustomConstruct construct);
                void loop(PragmaCustomConstruct construct);
                //void use_in_parallel(PragmaCustomConstruct construct);
                void barrier(PragmaCustomConstruct construct);
                void atomic(PragmaCustomConstruct construct);

                /* Clauses */
                void get_if_clause(PragmaCustomConstruct construct,
                                   KernelInfo *kernel_info);
                void get_private_clause(PragmaCustomConstruct construct,
                                        KernelInfo *kernel_info,
                                        ObjectList<IdExpression> &symbol_list);
                void get_reduction_clause(PragmaCustomConstruct construct,
                                          KernelInfo *kernel_info);
                
                /* Data Clauses */
                void get_copy_clause(PragmaCustomConstruct construct, TransferInfo *transfer_info);
                void get_copyin_clause(PragmaCustomConstruct construct, TransferInfo *transfer_info);
                void get_copyout_clause(PragmaCustomConstruct construct, TransferInfo *transfer_info);
                void get_create_clause(PragmaCustomConstruct construct, TransferInfo *transfer_info);
                void get_present_clause(PragmaCustomConstruct construct, TransferInfo *transfer_info);
                void get_pcopy_clause(PragmaCustomConstruct construct, TransferInfo *transfer_info);
                void get_pcopyin_clause(PragmaCustomConstruct construct, TransferInfo *transfer_info);
                void get_pcopyout_clause(PragmaCustomConstruct construct, TransferInfo *transfer_info);
                void get_pcreate_clause(PragmaCustomConstruct construct, TransferInfo *transfer_info);
                void get_data_clause(PragmaCustomClause &copy_clause,
                                     TransferInfo *transfer_info,
                                     AST_t ref_tree, 
                                     TL::ScopeLink scope_link,
                                     bool ispresent,
                                     std::string copy_type_str);
                
                /* Allocate Clause */
                void get_allocate_clause(PragmaCustomConstruct construct, 
                                         TransferInfo *transfer_info,
                                         AST_t ref_tree, 
                                         TL::ScopeLink scope_link);
                
                /* Pattern Clauses */
                void get_in_pattern_clause(PragmaCustomConstruct construct,
                                           KernelInfo *kernel_info);
                void get_out_pattern_clause(PragmaCustomConstruct construct,
                                            KernelInfo *kernel_info);
                void get_pattern_clause(PragmaCustomClause &pattern_clause,
                                        KernelInfo *kernel_info,
                                        std::string inout_pattern);

                // TODO: Change name to gather_data_for_analysis (and write data that it graps)
                void collect_variable_info(Statement stmt,
                                           TransferInfo *transfer_info);
                void collect_variable_info(Expression expr,
                                           TransferInfo *transfer_info,
                                           int found_idx_at = -1 /* For _shared_dimension */);
                int get_dimension_form_decl(TL::Declaration decl, 
                                            std::string var_name,
                                            ObjectList<std::string> &dim_size_params);
                
                // Assert condition
                void post_collect_variable_info(TransferInfo *transfer_info);
                void collect_loop_info(ForStatement for_stmt,
                                       KernelInfo *kernel_info,
                                       bool is_inner_loop = false);
                
                void add_expr_to_input_var(TransferInfo *transfer_info, 
                                           TL::Expression &expr);
                
                void show_variable_prop(TransferInfo *transfer_info);

                ScopeLink _scope_link;
                AST_t _translation_unit;
                
                FILE* _kernel_decl_file;
                int _openacc_opt_level;
                
                std::stack< TransferInfo* > transfer_info_stack;
        };

        //! @}
    }

}

#endif // GFN_PRAGMA_HPP
