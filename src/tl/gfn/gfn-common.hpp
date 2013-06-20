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

#define GFN_PATTERN_NONE   1
#define GFN_PATTERN_RANGE  2
#define GFN_PATTERN_SPEC   3

#define GFN_PREFIX_LOCAL   "_local_"

#define CL_EOL "\\n\n"

enum VAR_ACCESS_T
{
    VAR_ACCESS_SHARED,
    VAR_ACCESS_PRIVATE
};

enum REDUCTION_T
{
    REDUCTION_UNKNOWN,

    REDUCTION_MAX,
    REDUCTION_MIN,
    REDUCTION_SUM,
    REDUCTION_PROD,
    REDUCTION_BAND,
    REDUCTION_BOR,
    REDUCTION_BXOR,
    REDUCTION_LAND,
    REDUCTION_LOR,
    REDUCTION_LXOR,
    REDUCTION_MAXLOC,
    REDUCTION_MINLOC
};

enum CUDA_MEMCPY_KIND
{
    CUDA_MEMCPY_H2D,
    CUDA_MEMCPY_D2H,
    CUDA_MEMCPY_D2D
};

enum GFN_REPLACE_TYPE
{
    GFN_REPLACE_ARRAY_ND          = 0,
	GFN_REPLACE_ARRAY_INDEX       = 1,
    GFN_REPLACE_LAST_TYPE
};


/*==--------------- Utility Function ----------------==*/
std::string type_to_mpi_type(TL::Type type);
std::string type_to_ctype(TL::Type type);
REDUCTION_T op_to_op_type(std::string op);
std::string reduction_op_init_value(REDUCTION_T rt);
std::string op_to_mpi_op(REDUCTION_T rt);
std::string int_to_string(int num);
std::string source_to_kernel_str(TL::Source src);
std::string get_1d_reference(std::string var_name, unsigned dim_num);
TL::Source show_cl_source_in_comment(TL::Source src);
TL::Source create_run_only_root_stmt(TL::Source src);

#define DEFAULT_MPI_COMM "_get_mpi_comm_world()"  //"MPI_COMM_WORLD"
#define DEFAULT_MPI_ROOT "0"
#define DEFAULT_MPI_STATUS "0" //"NULL"

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
TL::Source create_mpi_bcast(std::string buf_name,
                            std::string cnt,
                            std::string mpi_type,
                            unsigned dim_num = 0,
                            std::string root = DEFAULT_MPI_ROOT,
                            std::string comm = DEFAULT_MPI_COMM);
TL::Source create_mpi_gatherv(std::string send_buf_name,
                              std::string send_cnt,
                              std::string send_mpi_type,
                              std::string recv_buf_name,
                              std::string recv_cnt,
                              std::string disps,
                              std::string recv_mpi_type,
                              unsigned dim_num = 0,
                              std::string root = DEFAULT_MPI_ROOT,
                              std::string comm = DEFAULT_MPI_COMM);
TL::Source create_mpi_iprobe(std::string src,
                             std::string tag,
                             std::string comm,
                             std::string flag_name,
                             std::string status);
TL::Source create_mpi_irecv(std::string buf_name,
                            std::string cnt,
                            std::string mpi_type,
                            std::string src,
                            std::string tag,
                            std::string comm,
                            std::string handle);
TL::Source create_mpi_isend(std::string buf_name,
                            std::string cnt,
                            std::string mpi_type,
                            std::string dest,
                            std::string tag,
                            std::string comm,
                            std::string handle);
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
TL::Source create_mpi_send(std::string buf_name,
                           std::string cnt,
                           std::string mpi_type,
                           std::string dest,
                           std::string tag,
                           std::string comm = DEFAULT_MPI_COMM);
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


TL::Source create_cl_create_context();
TL::Source create_cl_command_queue(std::string context,
                                   std::string device,
                                   std::string properties,
                                   std::string status = "_gfn_status");
TL::Source create_cl_create_buffer(std::string context,
                                   std::string flags,
                                   std::string size,
                                   std::string host_ptr,
                                   std::string status = "_gfn_status");
TL::Source create_cl_release_mem_object(std::string buffer);
TL::Source create_cl_enqueue_nd_range_kernel(std::string cmd_queue,
                                             std::string kernel,
                                             std::string work_dim,
                                             std::string global_work_offset,
                                             std::string global_work_size,
                                             std::string local_work_size,
                                             std::string num_event_wait_list = "0",
                                             std::string event_wait_list = "0",
                                             std::string event = "0");
TL::Source create_cl_enqueue_write_buffer(std::string cmd_queue,
                                          std::string buffer,
                                          bool is_block,
                                          std::string offset,
                                          std::string size,
                                          std::string var_ptr,
                                          std::string num_event_wait_list = "0",
                                          std::string event_wait_list = "0",
                                          std::string event = "0");
TL::Source create_cl_enqueue_read_buffer(std::string cmd_queue,
                                         std::string buffer,
                                         bool is_block,
                                         std::string offset,
                                         std::string size,
                                         std::string var_ptr,
                                         std::string num_event_wait_list = "0",
                                         std::string event_wait_list = "0",
                                         std::string event = "0");
TL::Source create_cl_set_kernel_arg(std::string kernel,
                                    int arg_no,
                                    std::string type,
                                    std::string buffer);
TL::Source create_cl_flush(std::string cmd_queue, std::string status = "");
TL::Source create_cl_finish(std::string cmd_queue, std::string status = "");
TL::Source create_cl_ext_pragma();

TL::Source create_cl_help_barrier();
TL::Source create_cl_help_atomic_add_int();
TL::Source create_cl_help_atomic_add_float();
TL::Source create_cl_help_atomic_add_double();
TL::Source create_cl_help_atomic_call(std::string global_var_name,
                                      std::string local_var_name,
                                      REDUCTION_T reduction_type,
                                      TL::Type var_type);

TL::Source create_gfn_check_cl_status(std::string status_var,
                                      std::string phase_name);


TL::Source create_gfn_q_bcast_scalar(std::string var_name, 
                                     std::string mpi_type);
TL::Source create_gfn_f_bcast_scalar();
TL::Source create_gfn_q_reduce_scalar(std::string var_name, 
                                      std::string mpi_type,
                                      std::string op_type);
TL::Source create_gfn_f_reduce_scalar();
TL::Source create_gfn_malloc_nd(std::string var_name,
                                std::string var_cl_name,
                                std::string var_unique_id_name,
                                std::string mpi_type,
                                int dim_num, std::string *dim_size,
                                std::string cl_mem_flags,
                                std::string level1_cond,
                                std::string level2_cond);
TL::Source create_gfn_free(std::string var_unique_id_name,
                           std::string level1_cond,
                           std::string level2_cond);
TL::Source create_gfn_q_bcast_nd(std::string var_name,
                                 std::string var_cl_name,
                                 std::string mpi_type,
                                 int dim_num, std::string *dim_size,
                                 std::string level1_cond,
                                 std::string level2_cond);
TL::Source create_gfn_q_scatter_nd(std::string var_name,
                                   std::string var_cl_name,
                                   std::string mpi_type,
                                   int dim_num, std::string *dim_size,
                                   int partitioned_dim,
                                   std::string cl_mem_flags,
                                   std::string pattern_array,
                                   std::string pattern_type,
                                   std::string level1_cond,
                                   std::string level2_cond);
TL::Source create_gfn_f_dist_array();
TL::Source create_gfn_q_gather_nd(std::string var_name,
                                  std::string var_cl_name,
                                  std::string mpi_type,
                                  int dim_num, std::string *dim_size,
                                  int partitioned_dim,
                                  std::string cl_mem_flags,
                                  std::string pattern_array,
                                  std::string pattern_type,
                                  std::string level1_cond,
                                  std::string level2_cond);
TL::Source create_gfn_f_gather_array();

#endif // GFN_COMMON_HPP
