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



#include <sstream>
#include "gfn-common.hpp"

std::string type_to_mpi_type(TL::Type type)
{
    std::string mpi_type_str;

    if (type.is_array())
        mpi_type_str = type_to_mpi_type(type.array_element());
    else if (type.is_pointer())
        mpi_type_str = type_to_mpi_type(type.get_pointer_to());

    /* Reference : https://computing.llnl.gov/tutorials/mpi/ */
    else if (type.is_char())
        mpi_type_str = "_get_mpi_char()"; //"MPI_CHAR";
    //else if (type.is_wchar_t())
        //mpi_type_str = "MPI_WCHAR";
    else if (type.is_signed_short_int())
        mpi_type_str = "_get_mpi_short()"; //"MPI_SHORT";
    else if (type.is_signed_int())
        mpi_type_str = "_get_mpi_int()"; //"MPI_INT";
    else if (type.is_signed_long_int())
        mpi_type_str = "_get_mpi_long()"; //"MPI_LONG";
    else if (type.is_signed_long_long_int())
        mpi_type_str = "_get_mpi_long_long_int()"; //"MPI_LONG_LONG_INT";
    /* MPI_LONG_LONG */
    else if (type.is_signed_char())
        mpi_type_str = "MPI_SIGNED_CHAR";
    else if (type.is_unsigned_char())
        mpi_type_str = "_get_mpi_unsigned_char()"; //"MPI_UNSIGNED_CHAR";
    else if (type.is_unsigned_short_int())
        mpi_type_str = "_get_mpi_unsigned_short()"; //"MPI_UNSIGNED_SHORT";
    else if (type.is_unsigned_int())
        mpi_type_str = "_get_mpi_unsigned()"; //"MPI_UNSIGNED";
    else if (type.is_unsigned_long_int())
        mpi_type_str = "_get_mpi_unsigned_long()"; //"MPI_UNSIGNED_LONG";
    else if (type.is_unsigned_long_long_int())
        mpi_type_str = "MPI_UNSIGNED_LONG_LONG";
    else if (type.is_float())
        mpi_type_str = "_get_mpi_float()"; //"MPI_FLOAT";
    else if (type.is_double())
        mpi_type_str = "_get_mpi_double()"; //"MPI_DOUBLE";
    else if (type.is_long_double())
        mpi_type_str = "_get_mpi_long_double()"; //"MPI_LONG_DOUBLE";
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

std::string type_to_ctype(TL::Type type)
{
    std::string ctype;

    if (type.is_array())
        ctype = type_to_ctype(type.array_element());
    else if (type.is_pointer())
        ctype = type_to_ctype(type.get_pointer_to());

    else if (type.is_char())
        ctype = "char";
    //else if (type.is_wchar_t())
        //mpi_type_str = "MPI_WCHAR";
    else if (type.is_signed_short_int())
        ctype = "short";
    else if (type.is_signed_int())
        ctype = "int";
    else if (type.is_signed_long_int())
        ctype = "long";
    else if (type.is_signed_long_long_int())
        ctype = "long long";
    /* MPI_LONG_LONG */
    //else if (type.is_signed_char())
        //ctype = "MPI_SIGNED_CHAR";
    else if (type.is_unsigned_char())
        ctype = "unsigned char";
    else if (type.is_unsigned_short_int())
        ctype = "unsigned short";
    else if (type.is_unsigned_int())
        ctype = "unsigned";
    else if (type.is_unsigned_long_int())
        ctype = "unsigned long";
    else if (type.is_unsigned_long_long_int())
        ctype = "unsigned long long";
    else if (type.is_float())
        ctype = "float";
    else if (type.is_double())
        ctype = "double";
    else if (type.is_long_double())
        ctype = "long double";
    else {
        std::cerr << "Don't support type : "
                  << type.get_symbol().get_name() << std::endl;
    }

    return ctype;
}

REDUCTION_T op_to_op_type(std::string op)
{
    if (op == "max")
        return REDUCTION_MAX;
    else if (op == "min")
        return REDUCTION_MIN;
    else if (op == "+")
        return REDUCTION_SUM;
    else if (op == "*")
        return REDUCTION_PROD;
    else if (op == "&")
        return REDUCTION_BAND;
    else if (op == "|")
        return REDUCTION_BOR;
    else if (op == "^")
        return REDUCTION_BXOR;
    else if (op == "&&")
        return REDUCTION_LAND;
    else if (op == "||")
        return REDUCTION_LOR;
#if 0
    else if (op == "")
        return REDUCTION_LXOR;
    else if (op == "")
        return REDUCTION_MAXLOC;
    else if (op == "")
        return REDUCTION_MINLOC;
#endif
    else {
        std::cerr << "Don't support operator : " << op << std::endl;
        return REDUCTION_UNKNOWN;
    }
}

std::string reduction_op_init_value(REDUCTION_T rt)
{
    if (rt == REDUCTION_MAX)
        return "";
    else if (rt == REDUCTION_MIN)
        return "";
    else if (rt == REDUCTION_SUM)
        return "0";
    else if (rt == REDUCTION_PROD)
        return "1";
    else if (rt == REDUCTION_BAND)
        return "~0";
    else if (rt == REDUCTION_BOR)
        return "0";
    else if (rt == REDUCTION_BXOR)
        return "0";
    else if (rt == REDUCTION_LAND)
        return "1";
    else if (rt == REDUCTION_LOR)
        return "0";
    else if (rt == REDUCTION_LXOR)
        return "";
    else if (rt == REDUCTION_MAXLOC)
        return "";
    else if (rt == REDUCTION_MINLOC)
        return "";
    else
    {
        std::cerr << "Don't support operator : " << (int)rt << std::endl;
        return "";
    }
}

std::string op_to_mpi_op(REDUCTION_T rt)
{
    std::string mpi_op;

    if (rt == REDUCTION_MAX)
        mpi_op = "_get_mpi_max()";
    else if (rt == REDUCTION_MIN)
        mpi_op = "_get_mpi_min()";
    else if (rt == REDUCTION_SUM)
        mpi_op = "_get_mpi_sum()";
    else if (rt == REDUCTION_PROD)
        mpi_op = "_get_mpi_prod()";
    else if (rt == REDUCTION_BAND)
        mpi_op = "_get_mpi_band()";
    else if (rt == REDUCTION_BOR)
        mpi_op = "_get_mpi_bor()";
    else if (rt == REDUCTION_BXOR)
        mpi_op = "_get_mpi_bxor()";
    else if (rt == REDUCTION_LAND)
        mpi_op = "_get_mpi_land()";
    else if (rt == REDUCTION_LOR)
        mpi_op = "_get_mpi_lor()";
    else if (rt == REDUCTION_LXOR)
        mpi_op = "_get_mpi_lxor()";
    else if (rt == REDUCTION_MAXLOC)
        mpi_op = "_get_mpi_maxloc()";
    else if (rt == REDUCTION_MINLOC)
        mpi_op = "_get_mpi_minloc()";
    else {
        std::cerr << "Don't support operator : " << (int)rt << std::endl;
    }

    return mpi_op;
}

std::string int_to_string(int num)
{
    std::stringstream ss;
    ss << num;
    return ss.str();
}

std::string source_to_kernel_str(TL::Source src)
{
    std::string result = "";
    result += "\"";

    std::string src_str = (std::string)src;
    std::cout << "src_str = " << src_str << std::endl;
    for (int i = 0; i < src_str.size(); ++i)
    {
        if (src_str[i] == '\n')
        {
            result += "\"\n\"";
        }
        else
        {
            result += src_str[i];
        }
    }

    result += "\"";
    return result;
}

TL::Source create_run_only_root_stmt(TL::Source src)
{
    TL::Source result;
    if (src.empty())
    {
        return result;
    }
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
        << "MPI_Allgatherv(" << send_buf_name << ","
        << send_buf_name << "," << send_mpi_type << ","
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

TL::Source create_mpi_bcast(std::string buf_name, std::string cnt,
                            std::string mpi_type, std::string root,
                            std::string comm)
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
    result
        << "MPI_Gatherv(" << send_buf_name << "," << send_cnt << ","
        << send_mpi_type << "," << recv_buf_name << ","
        << recv_cnt << "," << disps << "," << recv_mpi_type << ","
        << root << "," << comm << ");";
    return result;
}

TL::Source create_mpi_iprobe(std::string src, std::string tag,
                             std::string comm, std::string flag_name,
                             std::string status)
{
    TL::Source result;
    result
        << "MPI_Iprobe(" << src << "," << tag << ","
        << comm << ",&" << flag_name << "," << status << ");";
    return result;
}

TL::Source create_mpi_irecv(std::string buf_name, std::string cnt,
                            std::string mpi_type, std::string src,
                            std::string tag, std::string comm,
                            std::string handle)
{
    TL::Source result;
    return result;
}

TL::Source create_mpi_isend(std::string buf_name, std::string cnt,
                            std::string mpi_type, std::string dest,
                            std::string tag, std::string comm,
                            std::string handle)
{
    TL::Source result;
    return result;
}

TL::Source create_mpi_probe(std::string src, std::string tag,
                            std::string comm, std::string status)
{
    TL::Source result;
    result
        << "MPI_Probe(" << src << "," << tag << ","
        << comm << "," << status << ");";
    return result;
}

TL::Source create_mpi_recv(std::string buf_name, std::string cnt,
                           std::string mpi_type, std::string src,
                           std::string tag, std::string comm,
                           std::string status)
{
    TL::Source result;
    result
        << "MPI_Recv(&" << buf_name << "," << cnt << ","
        << mpi_type << "," << src << "," << tag << ","
        << comm << "," << status << ");";
    return result;
}

TL::Source create_mpi_send(std::string buf_name, std::string cnt,
                           std::string mpi_type, std::string dest,
                           std::string tag, std::string comm)
{
    TL::Source result;
    result
        << "MPI_Send(&" << buf_name << "," << cnt << "," << mpi_type << ","
        << dest << "," << tag << "," << comm << ");";
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
        << "MPI_Scatterv(" << send_buf_name << ","
        << send_cnts << "," << send_disp << ","
        << send_mpi_type << "," << recv_buf_name << ","
        << recv_cnt << "," << recv_mpi_type << ","
        << root << "," << comm << ");";
    return result;
}


TL::Source create_cl_create_buffer(std::string context, std::string flags,
                                   std::string size, std::string host_ptr,
                                   std::string status)
{
    TL::Source result;
    result
        << "clCreateBuffer(" << context << "," << flags << ","
        << size << "," << host_ptr << ",&" << status << ");";
    return result;
}

TL::Source create_cl_release_mem_object(std::string buffer)
{
    TL::Source result;
    result << "clReleaseMemObject(" << buffer << ");";
    return result;
}

TL::Source create_cl_enqueue_nd_range_kernel(std::string cmd_queue, std::string kernel,
                                             std::string work_dim, std::string global_work_offset,
                                             std::string global_work_size, std::string local_work_size,
                                             std::string num_event_wait_list,
                                             std::string event_wait_list, std::string event)
{
    TL::Source result;
    result
        << "clEnqueueNDRangeKernel(" << cmd_queue << "," << kernel << ","
        << work_dim << "," << global_work_offset << ","
        << global_work_size << "," << local_work_size << ","
        << num_event_wait_list << "," << event_wait_list << ","
        << event << ");";
    return result;
}

TL::Source create_cl_enqueue_write_buffer(std::string cmd_queue, std::string buffer,
                                          bool is_block, std::string offset,
                                          std::string size, std::string var_ptr,
                                          std::string num_event_wait_list,
                                          std::string event_wait_list, std::string event)
{
    TL::Source result;
    std::string block = ((is_block)? "_get_cl_true()" : "_get_cl_false()");
    result
        << "clEnqueueWriteBuffer(" << cmd_queue << "," << buffer << ","
        << block << "," << offset << "," << size << "," << var_ptr << ","
        << num_event_wait_list << "," << event_wait_list << "," << event << ");";
    return result;
}

TL::Source create_cl_enqueue_read_buffer(std::string cmd_queue, std::string buffer,
                                         bool is_block, std::string offset,
                                         std::string size, std::string var_ptr,
                                         std::string num_event_wait_list,
                                         std::string event_wait_list, std::string event)
{
    TL::Source result;
    std::string block = ((is_block)? "_get_cl_true()" : "_get_cl_false()");
    result
        << "clEnqueueReadBuffer(" << cmd_queue << "," << buffer << ","
        << block << "," << offset << "," << size << "," << var_ptr << ","
        << num_event_wait_list << "," << event_wait_list << "," << event << ");";
    return result;
}

TL::Source create_cl_set_kernel_arg(std::string kernel, int arg_no,
                                    std::string type, std::string buffer)
{
    TL::Source result;
    result
        << "clSetKernelArg(" << kernel << "," << arg_no
        << ",sizeof(" << type << "),";
    if (buffer == "0")
        result << "0);";
    else
        result <<"(void*)&" << buffer << ");";
    return result;
}

TL::Source create_cl_flush(std::string cmd_queue, std::string status)
{
    TL::Source result;
    if (status != "") result << status << " = ";
    result << "clFlush(" << cmd_queue << ");";
    return result;
}

TL::Source create_cl_finish(std::string cmd_queue, std::string status)
{
    TL::Source result;
    if (status != "") result << status << " = ";
    result << "clFinish(" << cmd_queue << ");";
    return result;
}


TL::Source create_cl_help_barrier()
{
    TL::Source result;
    result
        << "void _GfnBarrier() {\n"
        << "    barrier(CLK_LOCAL_MEM_FENCE | CLK_GLOBAL_MEM_FENCE);\n"
        << "}\n";
    return result;
}

TL::Source create_cl_help_atomic_add_int()
{
    TL::Source result;
    result
        << "int _GfnAtomicAddInt(__global int* const address, const int value) {\n"
        << "    return atomic_add(address, value);\n"
        << "}\n";
    return result;
}

TL::Source create_cl_help_atomic_add_float()
{
    /* TODO: is following line is necessary?
     * #pragma OPENCL EXTENSION cl_khr_global_int32_base_atomics : enable */
    TL::Source result;
    result
        << "float _GfnAtomicAddFloat(__global float* const address, const float value) {\n"
        << "    uint oldval, newval, readback;\n"
        << "    *(float*)&oldval = *address;\n"
        << "    *(float*)&newval = (*(float*)&oldval + value);\n"
        << "    while ((readback = atom_cmpxchg((__global uint*)address, oldval, newval)) != oldval) {\n"
        << "        oldval = readback;\n"
        << "        *(float*)&newval = (*(float*)&oldval + value);\n"
        << "    }\n"
        << "    return *(float*)&oldval;\n"
        << "}\n";
    return result;
}

TL::Source create_cl_help_atomic_add_double()
{
    /* TODO: is following line is necessary?
     * #pragma OPENCL EXTENSION cl_khr_fp64 : enable
     * #pragma OPENCL EXTENSION cl_khr_int64_base_atomics : enable */
    TL::Source result;
    result
        << "double _GfnAtomicAddDouble(__global double* const address, const double value) {\n"
        << "    long oldval, newval, readback;\n"
        << "    *(double*)&oldval = *address;\n"
        << "    *(double*)&newval = (*(double*)&oldval + value);\n"
        << "    while ((readback = atom_cmpxchg((__global long*)address, oldval, newval)) != oldval) {\n"
        << "        oldval = readback;\n"
        << "        *(double*)&newval = (*(double*)&oldval + value);\n"
        << "    }\n"
        << "    return *(double*)&oldval;\n"
        << "}\n";
    return result;
}

TL::Source create_cl_help_atomic_call(std::string global_var_name,
                                      std::string local_var_name,
                                      REDUCTION_T reduction_type,
                                      TL::Type var_type)
{
    TL::Source result;
    std::string atomic_name = "_GfnAtomic";//"AddFloat";

    switch (reduction_type) {
    case REDUCTION_MAX:     atomic_name += "Max"; break;
    case REDUCTION_MIN:     atomic_name += "Min"; break;
    case REDUCTION_SUM:     atomic_name += "Add"; break;
    case REDUCTION_PROD:    atomic_name += "Mul"; break;
    case REDUCTION_BAND:    atomic_name += "Band"; break;
    case REDUCTION_BOR:     atomic_name += "Bor"; break;
    case REDUCTION_BXOR:    atomic_name += "Bxor"; break;
    case REDUCTION_LAND:    atomic_name += "Land"; break;
    case REDUCTION_LOR:     atomic_name += "Lor"; break;
    case REDUCTION_LXOR:    atomic_name += "Lxor"; break;
    case REDUCTION_MAXLOC:  atomic_name += "Maxloc"; break;
    case REDUCTION_MINLOC:  atomic_name += "MinLoc"; break;
    default: // TODO: error
        break;
    }

    // TODO: other type
    if (var_type.is_char())        atomic_name += "Char";
    else if (var_type.is_signed_int()) atomic_name += "Int";
    else if (var_type.is_float())  atomic_name += "Float";
    else if (var_type.is_double()) atomic_name += "Double";
    else {
        std::cerr << "Unsupport type " << __FILE__ << ":" << __LINE__ << "\n";
    }

    result
        << atomic_name << "(" << global_var_name << ","
        << local_var_name << ");\n";
    return result;
}

