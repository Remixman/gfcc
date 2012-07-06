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



#ifndef GFN_KERNEL_CORE_HPP
#define GFN_KERNEL_CORE_HPP

#include "tl-pragmasupport.hpp"
#include "tl-ast.hpp"
#include "tl-datareference.hpp"
#include "tl-source.hpp"
#include "gfn-common.hpp"
#include <string>
#include <map>
#include <iostream>

namespace TL
{
    //! Griffon for TL
    namespace GFN
    {
        //! \addtogroup GFN Griffon
        //! @{

        //! GFN Kernel Infomation
        /*!
          .........
         */
        struct LIBGFN_CLASS KernelInfo
        {
            private:
                std::string _kernel_name;
                ObjectList<std::string> _wait_for;
                ObjectList<std::string> _all_var;
                
                ObjectList<TL::DataReference> _private_list;

                ObjectList<std::string> _shared_var;
                
                ObjectList<TL::DataReference> _use_list;
                ObjectList<TL::DataReference> _def_list;
                
                GFN_ACCURATE _accurate;
                
                TL::Source _thread_num;
                TL::Source _block_num;
                TL::Source _thread_per_block;

            public:
                //operator Source();
                //operator std::string();

                KernelInfo();
                KernelInfo(std::string &kernel_name);
                ~KernelInfo();
                
                void set_kernel_name(std::string &name);
                std::string get_kernel_name();
                void set_thread_num(TL::Source thread_num);
                TL::Source get_thread_num();
                void set_wait_for(ObjectList<std::string> &wait_for);
                ObjectList<std::string> get_wait_for();

                void set_private_list(ObjectList<TL::DataReference> &private_list);
                ObjectList<TL::DataReference> get_private_list();

                void set_accurate(GFN_ACCURATE accurate);
                GFN_ACCURATE get_accurate();
                
                static std::map< std::string, KernelInfo > _kernel_map;
        };
        //! @}
    }
}

#endif // GFN_KERNEL_CORE_HPP
