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



#ifndef GFN_COMMON_HPP
#define GFN_COMMON_HPP

#ifdef _WIN32
  #ifdef LIBGFN_DLL_EXPORT
    #define LIBGFN_EXTERN extern __declspec(dllexport)
    #define LIBGFN_CLASS __declspec(dllexport)
  #else
    #define LIBGFN_EXTERN extern __declspec(dllimport)
    #define LIBGFN_CLASS __declspec(dllimport)
  #endif
  #define LIBGFN_ALWAYS_EXPORT __declspec(dllexport)
#else
  #define LIBGFN_EXTERN extern
  #define LIBGFN_CLASS
  #define LIBGFN_ALWAYS_EXPORT 
#endif

#define GFN_GENERATED_PREFIX "_gfn_"
#define GFN_DEVICE_PREFIX "_dev_"
#define GFN_THREAD_ID_VAR "_thread_id"
#define GFN_UPPER_BOUND "_upper_bound"

enum GFN_ACCURATE
{
    ACCURATE_LOW,
    ACCURATE_HIGH
};

enum CUDA_MEMCPY_KIND
{
    CUDA_MEMCPY_H2D,
    CUDA_MEMCPY_D2H,
    CUDA_MEMCPY_D2D
};

#endif // GFN_COMMON_HPP
