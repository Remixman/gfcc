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



#include "gfn-common.hpp"

std::string type_to_mpi_type(TL::Type type)
{
    std::string mpi_type_str;

    /* Reference : https://computing.llnl.gov/tutorials/mpi/ */
    if (type.is_char())
        mpi_type_str = "MPI_CHAR";
    else if (type.is_wchar_t())
        mpi_type_str = "MPI_WCHAR";
    else if (type.is_signed_short_int())
        mpi_type_str = "MPI_SHORT";
    else if (type.is_signed_int())
        mpi_type_str = "MPI_INT";
    else if (type.is_signed_long_int())
        mpi_type_str = "MPI_LONG";
    else if (type.is_signed_long_long_int())
        mpi_type_str = "MPI_LONG_LONG_INT";
    /* MPI_LONG_LONG */
    else if (type.is_signed_char())
        mpi_type_str = "MPI_SIGNED_CHAR";
    else if (type.is_unsigned_char())
        mpi_type_str = "MPI_UNSIGNED_CHAR";
    else if (type.is_unsigned_short_int())
        mpi_type_str = "MPI_UNSIGNED_SHORT";
    else if (type.is_unsigned_int())
        mpi_type_str = "MPI_UNSIGNED";
    else if (type.is_unsigned_long_int())
        mpi_type_str = "MPI_UNSIGNED_LONG";
    else if (type.is_unsigned_long_long_int())
        mpi_type_str = "MPI_UNSIGNED_LONG_LONG";
    else if (type.is_float())
        mpi_type_str = "MPI_FLOAT";
    else if (type.is_double())
        mpi_type_str = "MPI_DOUBLE";
    else if (type.is_long_double())
        mpi_type_str = "MPI_LONG_DOUBLE";
    else {
        std::cerr << "Don't support type : "
                  << type.get_symbol().get_name() << std::endl;
    }

    /*MPI_C_COMPLEX
    MPI_C_FLOAT_COMPLEX
    MPI_C_DOUBLE_COMPLEX
    MPI_C_LONG_DOUBLE_COMPLEX
    MPI_C_BOOL
    MPI_C_LONG_DOUBLE_COMPLEX
    MPI_INT8_T
    MPI_INT16_T
    MPI_INT32_T
    MPI_INT64_T
    MPI_UINT8_T
    MPI_UINT16_T
    MPI_UINT32_T
    MPI_UINT64_T*/
            //MPI_BYTE
            //MPI_PACKED

    return mpi_type_str;
}

TL::Source create_run_only_root_stmt(TL::Source src)
{
    TL::Source result;
    result << "if (_gfn_rank == 0) {" << src << "}";
    return result;
}


TL::Source create_mpi_abort(std::string comm)
{
    TL::Source result;
    result << "MPI_Abort(" << comm << ", -1234);";
    return result;
}

TL::Source create_mpi_allgatherv(std::string send_buf_name, std::string send_cnt,
                                 std::string send_mpi_type, std::string recv_buf_name,
                                 std::string recv_cnts, std::string recv_disp,
                                 std::string recv_mpi_type, std::string comm)
{
    TL::Source result;
    result
        << "MPI_Allgatherv(&" << send_buf_name << ","
        << send_buf_name << "," << send_mpi_type << ",&"
        << recv_buf_name << "," << recv_cnts << ","
        << recv_disp << "," << recv_mpi_type << "," << comm << ");";
    return result;
}

TL::Source create_mpi_allreduce(std::string send_buf_name, std::string recv_buf_name,
                                std::string cnt, std::string mpi_type,
                                std::string mpi_op, std::string comm)
{
    TL::Source result;
    result
        << "MPI_Allreduce(&" << send_buf_name << ",&"
        << recv_buf_name << "," << cnt << ","
        << mpi_type << "," << mpi_op << "," << comm << ");";
    return result;
}

TL::Source create_mpi_alltoallv(std::string send_buf_name, std::string send_cnt,
                                std::string send_mpi_type, std::string recv_buf_name,
                                std::string recv_cnt, std::string recv_mpi_type,
                                std::string comm)
{
    TL::Source result;
    result
        << "MPI_Alltoallv(&" << send_buf_name << ","
        << send_cnt << "," << send_mpi_type << ",&"
        << recv_buf_name << "," << recv_cnt << ","
        << recv_mpi_type << "," << comm << ");";
    return result;
}

TL::Source create_mpi_barrier(std::string comm)
{
    TL::Source result;
    result << "MPI_Barrier(" << comm << ");";
    return result;
}

TL::Source
create_mpi_bcast(std::string buf_name, std::string cnt, std::string mpi_type,
                 std::string root, std::string comm)
{
    TL::Source result;
    result
        << "MPI_Bcast(&" << buf_name << ", " << cnt << ", " << mpi_type << ","
        << root << ", " << comm << ");";
    return result;
}

TL::Source create_mpi_gatherv(std::string send_buf_name, std::string send_cnt,
                              std::string send_mpi_type, std::string recv_buf_name,
                              std::string recv_cnt, std::string disps,
                              std::string recv_mpi_type, std::string root,
                              std::string comm)
{
    TL::Source result;
    return result;
}

TL::Source create_mpi_iprobe()
{
    TL::Source result;
    return result;
}

TL::Source create_mpi_irecv()
{
    TL::Source result;
    return result;
}

TL::Source create_mpi_isend()
{
    TL::Source result;
    return result;
}

TL::Source create_mpi_probe()
{
    TL::Source result;
    return result;
}

TL::Source create_mpi_recv()
{
    TL::Source result;
    return result;
}

TL::Source create_mpi_send()
{
    /* int MPI_Send(void *buf,
    int count,
    MPI_Datatype datatype,
    int dest,
        int tag,
    MPI_Comm comm)*/
    TL::Source result;
    return result;
}

TL::Source create_mpi_reduce(std::string send_buf_name, std::string recv_buf_name,
                             std::string cnt, std::string mpi_type,
                             std::string mpi_op, std::string root,
                             std::string comm)
{
    TL::Source result;
    result
        << "MPI_Reduce(&" << send_buf_name << ",&"
        << recv_buf_name << "," << cnt << ","
        << mpi_type << "," << mpi_op << ","
        << root << "," << comm << ");";
    return result;
}

TL::Source create_mpi_scatterv(std::string send_buf_name, std::string send_cnts,
                               std::string send_disp, std::string send_mpi_type,
                               std::string recv_buf_name, std::string recv_cnt,
                               std::string recv_mpi_type, std::string root,
                               std::string comm)
{
    TL::Source result;
    result
        << "MPI_Scatterv(&" << send_buf_name << ","
        << send_cnts << "," << send_disp << ","
        << send_mpi_type << ",&" << recv_buf_name << ","
        << recv_cnt << "," << recv_mpi_type << ","
        << root << "," << comm << ");";
    return result;
}



