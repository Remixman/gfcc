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

#include "tl-pragmasupport.hpp"
#include "tl-ast.hpp"
#include "tl-datareference.hpp"
#include "tl-source.hpp"

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

/* Cluster define TODO: change to dynamic config */
#define GFN_PROC_NUM 8
#define GFN_PROC_NUM_VAR "_gfn_proc_num"
#define GFN_RANK_VAR "_gfn_rank"
#define GFN_COMM "_gfn_comm"

#define GFN_TRANS_MPI   0x00001
#define GFN_TRANS_CUDA  0x00002

enum VAR_ACCESS_T
{
    VAR_ACCESS_SHARED,
    VAR_ACCESS_PRIVATE
};

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


/*==--------------- Utility Function ----------------==*/
std::string type_to_mpi_type(TL::Type type);
TL::Source create_run_only_root_stmt(TL::Source src);

#define DEFAULT_MPI_COMM "MPI_COMM_WORLD"
#define DEFAULT_MPI_ROOT "0"
#define DEFAULT_MPI_STATUS "NULL"

TL::Source create_mpi_abort(std::string comm = DEFAULT_MPI_COMM);
TL::Source create_mpi_allgatherv(std::string send_buf_name,
                                 std::string send_cnt,
                                 std::string send_mpi_type,
                                 std::string recv_buf_name,
                                 std::string recv_cnts,
                                 std::string recv_disp,
                                 std::string recv_mpi_type,
                                 std::string comm = DEFAULT_MPI_COMM);
TL::Source create_mpi_allreduce(std::string send_buf_name,
                                std::string recv_buf_name,
                                std::string cnt,
                                std::string mpi_type,
                                std::string mpi_op,
                                std::string comm = DEFAULT_MPI_COMM);
TL::Source create_mpi_alltoallv(std::string send_buf_name,
                                std::string send_cnt,
                                std::string send_mpi_type,
                                std::string recv_buf_name,
                                std::string recv_cnt,
                                std::string recv_mpi_type,
                                std::string comm = DEFAULT_MPI_COMM);
TL::Source create_mpi_barrier(std::string comm = DEFAULT_MPI_COMM);
TL::Source
create_mpi_bcast(std::string buf_name,
                 std::string cnt,
                 std::string mpi_type,
                 std::string root = DEFAULT_MPI_ROOT,
                 std::string comm = DEFAULT_MPI_COMM);
TL::Source create_mpi_gatherv(std::string send_buf_name,
                              std::string send_cnt,
                              std::string send_mpi_type,
                              std::string recv_buf_name,
                              std::string recv_cnt,
                              std::string disps,
                              std::string recv_mpi_type,
                              std::string root = DEFAULT_MPI_ROOT,
                              std::string comm = DEFAULT_MPI_COMM);
TL::Source create_mpi_iprobe();
TL::Source create_mpi_irecv();
TL::Source create_mpi_isend();
TL::Source create_mpi_probe(std::string src,
                            std::string tag,
                            std::string comm = DEFAULT_MPI_COMM,
                            std::string status = DEFAULT_MPI_STATUS);
TL::Source create_mpi_recv(std::string buf_name,
                           std::string cnt,
                           std::string mpi_type,
                           std::string src,
                           std::string tag,
                           std::string comm = DEFAULT_MPI_COMM,
                           std::string status = DEFAULT_MPI_STATUS);
TL::Source create_mpi_send();
TL::Source create_mpi_reduce(std::string send_buf_name,
                             std::string recv_buf_name,
                             std::string cnt,
                             std::string mpi_type,
                             std::string mpi_op,
                             std::string root = DEFAULT_MPI_ROOT,
                             std::string comm = DEFAULT_MPI_COMM);
TL::Source create_mpi_scatterv(std::string send_buf_name,
                               std::string send_cnts,
                               std::string send_disp,
                               std::string send_mpi_type,
                               std::string recv_buf_name,
                               std::string recv_cnt,
                               std::string recv_mpi_type,
                               std::string root = DEFAULT_MPI_ROOT,
                               std::string comm = DEFAULT_MPI_COMM);


#endif // GFN_COMMON_HPP
