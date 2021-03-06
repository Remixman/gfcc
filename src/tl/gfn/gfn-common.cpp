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
        mpi_type_str = type_to_mpi_type(type.points_to());

    /* Reference : https://computing.llnl.gov/tutorials/mpi/ */
    else if (type.is_char())
        mpi_type_str = "_GFN_TYPE_CHAR()"; //"MPI_CHAR";
    //else if (type.is_wchar_t())
        //mpi_type_str = "MPI_WCHAR";
    else if (type.is_signed_short_int())
        mpi_type_str = "_GFN_TYPE_SHORT()"; //"MPI_SHORT";
    else if (type.is_signed_int())
        mpi_type_str = "_GFN_TYPE_INT()"; //"MPI_INT";
    else if (type.is_signed_long_int())
        mpi_type_str = "_GFN_TYPE_LONG()"; //"MPI_LONG";
    else if (type.is_signed_long_long_int())
        mpi_type_str = "_GFN_TYPE_LONG_LONG_INT()"; //"MPI_LONG_LONG_INT";
    /* MPI_LONG_LONG */
    else if (type.is_signed_char())
        mpi_type_str = "_GFN_TYPE_CHAR()";
    else if (type.is_unsigned_char())
        mpi_type_str = "_GFN_TYPE_UNSIGNED_CHAR()"; //"MPI_UNSIGNED_CHAR";
    else if (type.is_unsigned_short_int())
        mpi_type_str = "_GFN_TYPE_UNSIGEND_SHORT()"; //"MPI_UNSIGNED_SHORT";
    else if (type.is_unsigned_int())
        mpi_type_str = "_GFN_TYPE_UNSIGNED()"; //"MPI_UNSIGNED";
    else if (type.is_unsigned_long_int())
        mpi_type_str = "_GFN_TYPE_UNSIGNED_LONG()"; //"MPI_UNSIGNED_LONG";
    //else if (type.is_unsigned_long_long_int())
        //mpi_type_str = "MPI_UNSIGNED_LONG_LONG";
    else if (type.is_float())
        mpi_type_str = "_GFN_TYPE_FLOAT()"; //"MPI_FLOAT";
    else if (type.is_double())
        mpi_type_str = "_GFN_TYPE_DOUBLE()"; //"MPI_DOUBLE";
    else if (type.is_long_double())
        mpi_type_str = "_GFN_TYPE_LONG_DOUBLE()"; //"MPI_LONG_DOUBLE";
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
        ctype = type_to_ctype(type.points_to());

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
        mpi_op = "_GFN_OP_MAX()";
    else if (rt == REDUCTION_MIN)
        mpi_op = "_GFN_OP_MIN()";
    else if (rt == REDUCTION_SUM)
        mpi_op = "_GFN_OP_SUM()";
    else if (rt == REDUCTION_PROD)
        mpi_op = "_GFN_OP_PROD()";
    else if (rt == REDUCTION_BAND)
        mpi_op = "_GFN_OP_BAND()";
    else if (rt == REDUCTION_BOR)
        mpi_op = "_GFN_OP_BOR()";
    else if (rt == REDUCTION_BXOR)
        mpi_op = "_GFN_OP_BXOR()";
    else if (rt == REDUCTION_LAND)
        mpi_op = "_GFN_OP_LAND()";
    else if (rt == REDUCTION_LOR)
        mpi_op = "_GFN_OP_LOR()";
    else if (rt == REDUCTION_LXOR)
        mpi_op = "_GFN_OP_LXOR()";
    else if (rt == REDUCTION_MAXLOC)
        mpi_op = "_GFN_OP_MAXLOC()";
    else if (rt == REDUCTION_MINLOC)
        mpi_op = "_GFN_OP_MINLOC()";
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
    //std::cout << "src_str = " << src_str << std::endl;
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

    result += "\\n\"";
    return result;
}

std::string get_1d_reference(std::string var_name, unsigned dim_num)
{
    return var_name;
    
    // TODO: use when remove 2D>1D convertion
  
    std::string buf_reference;
    if (dim_num == 0)
    {
        buf_reference = var_name;
    }
    else
    {
        // cast to contiguous 1D array, Ex. &(A[0][0])
        buf_reference = "&(" + var_name;
        for (int i = 0; i < dim_num; ++i)
            buf_reference += "[0]";
        buf_reference += ")";
    }
    return buf_reference;
}

TL::Source show_cl_source_in_comment(TL::Source src)
{
    TL::Source result;
    std::string buffer = "";
    std::string src_str = (std::string)src;

    for (int i = 0; i < src_str.size(); ++i)
    {
        if (src_str[i] == '\n')
        {
            //std::cout << "Find \\n at " << i << "\n";
            result << TL::comment(buffer);
            buffer = "";
        }
        else
        {
            buffer += src_str[i];
        }
    }
    result << TL::comment(buffer);

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

std::string get_function_declaration_from_definition(std::string func_def)
{
    size_t size = func_def.find("{", 0)-1;
    std::string func_decl = func_def.substr(0,size) + ";";
    return func_decl;
}

void print_to_kernel_decl_file(TL::ScopeLink &scope_link, TL::AST_t &translation_unit,
                               FILE *kerdecl_fptr, TL::Source &src)
{
    TL::AST_t src_tree = src.parse_declaration(translation_unit, scope_link);
    fprintf(kerdecl_fptr, "%s\n\n", src_tree.prettyprint_external().c_str());
}

TL::Source create_send_var_id_msg(std::string var_name,
                                  int dim_num)
{
    TL::Source result, subscript_to_1d;
    for (int i = 0; i < dim_num; ++i) subscript_to_1d << "[0]";
    result
        << "_SendConstInputMsg((long long)&(" << var_name << subscript_to_1d << "));";
    return result;
}

TL::Source create_master_lock_var(std::string var_name, int dim_num)
{
    TL::Source result, subscript_to_1d;
    for (int i = 0; i < dim_num; ++i) subscript_to_1d << "[0]";
    result
        << "_GfnLockTransfer((long long)&(" << var_name << subscript_to_1d << "));";
    return result;
}

TL::Source create_master_unlock_var(std::string var_name, int dim_num)
{
    TL::Source result, subscript_to_1d;
    for (int i = 0; i < dim_num; ++i) subscript_to_1d << "[0]";
    result
        << "_GfnUnlockTransfer((long long)&(" << var_name << subscript_to_1d << "));";
    return result;
}

TL::Source create_send_input_nd_msg(std::string var_name,
                                    std::string type_id,
                                    std::string loop_start,
                                    std::string loop_end,
                                    std::string loop_step,
                                    int partitioned_dim,
                                    int pattern_type,
                                    int dim_num,
                                    int pattern_num,
                                    TL::ObjectList<TL::Expression> dim_size,
                                    TL::ObjectList<std::string> &pattern_array)
{
    TL::Source result, subscript_to_1d;
    for (int i = 0; i < dim_num; ++i) subscript_to_1d << "[0]";
    result
        << "_SendInputNDMsg(&(" << var_name << subscript_to_1d << ")," << type_id << ","
        << loop_start << "," << loop_end << "," << loop_step << ","
        << partitioned_dim << "," << pattern_type << ","
        << dim_num << "," << pattern_num;
    for (int i = 0; i < dim_num; ++i) result << "," << dim_size[i];
    for (int i = 0; i < pattern_num; ++i) result << "," << pattern_array[i];
    result
        << ");";
    return result;
}

TL::Source create_recv_output_nd_msg(std::string var_name,
                                     std::string type_id,
                                     std::string loop_start,
                                     std::string loop_end,
                                     std::string loop_step,
                                     int partitioned_dim,
                                     int pattern_type,
                                     int dim_num,
                                     int pattern_num,
                                     TL::ObjectList<TL::Expression> dim_size,
                                     TL::ObjectList<std::string> &pattern_array)
{
    TL::Source result, subscript_to_1d;
    for (int i = 0; i < dim_num; ++i) subscript_to_1d << "[0]";
    result
        << "_RecvOutputNDMsg(&(" << var_name << subscript_to_1d << ")," << type_id << ","
        << loop_start << "," << loop_end << "," << loop_step << ","
        << partitioned_dim << "," << pattern_type << ","
        << dim_num << "," << pattern_num;
    for (int i = 0; i < dim_num; ++i) result << "," << dim_size[i];
    for (int i = 0; i < pattern_num; ++i) result << "," << pattern_array[i];
    result
        << ");";
    return result;
}

TL::Source create_cl_set_kernel_arg(std::string kernel, int arg_no,
                                    std::string type, std::string buffer)
{
    std::string size_str, offset_str;
    TL::Source phase_name;
    
    if (buffer == "0")
    {
        size_str = "sizeof(" + type + ") * _work_group_item_num";
        offset_str = "0";
    }
    else
    {
        size_str = "sizeof(" + type + ")";
        offset_str = "(void*)&" + buffer;
    }

    TL::Source result;
    result
        << "_GfnSetKernelArg(" << kernel << "," << arg_no << ","
        << size_str << "," << offset_str << ");";
    return result;
}

TL::Source create_cl_help_barrier()
{
    TL::Source result;
    result
        << "void _GfnBarrier() {" << CL_EOL
        << "    barrier(CLK_LOCAL_MEM_FENCE | CLK_GLOBAL_MEM_FENCE);" << CL_EOL
        << "}" << CL_EOL;
    return result;
}

TL::Source create_cl_help_atomic_add_int()
{
    TL::Source result;
    result
        << "int _GfnAtomicAddInt(__global int* const address, const int value) {" << CL_EOL
        << "    return atomic_add(address, value);" << CL_EOL
        << "}" << CL_EOL;
    return result;
}

TL::Source create_cl_help_atomic_add_float()
{
    TL::Source result;
    result
        << "float _GfnAtomicAddFloat(__global float* const address, const float value) {" << CL_EOL
        << "    uint oldval, newval, readback;" << CL_EOL
        << "    *(float*)&oldval = *address;" << CL_EOL
        << "    *(float*)&newval = (*(float*)&oldval + value);" << CL_EOL
        << "    while ((readback = atom_cmpxchg((__global uint*)address, oldval, newval)) != oldval) {" << CL_EOL
        << "        oldval = readback;" << CL_EOL
        << "        *(float*)&newval = (*(float*)&oldval + value);" << CL_EOL
        << "    }" << CL_EOL
        << "    return *(float*)&oldval;" << CL_EOL
        << "}" << CL_EOL;
    return result;
}

TL::Source create_cl_help_atomic_add_double()
{
    TL::Source result;
    result
        << "double _GfnAtomicAddDouble(__global double* const address, const double value) {" << CL_EOL
        << "    long oldval, newval, readback;" << CL_EOL
        << "    *(double*)&oldval = *address;" << CL_EOL
        << "    *(double*)&newval = (*(double*)&oldval + value);" << CL_EOL
        << "    while ((readback = atom_cmpxchg((__global long*)address, oldval, newval)) != oldval) {" << CL_EOL
        << "        oldval = readback;" << CL_EOL
        << "        *(double*)&newval = (*(double*)&oldval + value);" << CL_EOL
        << "    }" << CL_EOL
        << "    return *(double*)&oldval;" << CL_EOL
        << "}" << CL_EOL;
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
        << local_var_name << "[0]);\n";
    return result;
}


TL::Source create_gfn_check_cl_status(std::string status_var,
                                      std::string phase_name)
{
    TL::Source result;
    result << "_GfnCheckCLStatus(" << status_var << ",\"" << phase_name << "\");";
    return result;
}

TL::Source create_gfn_malloc_reduce(std::string var_name,
                                    std::string var_cl_name,
                                    std::string type_id,
                                    std::string group_num,
                                    std::string level1_cond,
                                    std::string level2_cond)
{
    TL::Source result;
    result
        << "_GfnMallocReduceScalar(&" << var_name << ",&" << var_cl_name << ","
        << type_id << "," << group_num << "," << level1_cond << "," << level2_cond <<");";
    return result;
}

TL::Source create_gfn_free_reduce(std::string var_cl_name,
                                  std::string level1_cond,
                                  std::string level2_cond)
{
    TL::Source result;
    result
        << "_GfnFreeReduceScalar(" << var_cl_name << ","
        << level1_cond << "," << level2_cond << ");";
    return result; 
}

TL::Source create_gfn_q_bcast_scalar(std::string var_name, 
                                     std::string mpi_type)
{
    TL::Source result;
    result << "_GfnEnqueueBroadcastScalar(&" << var_name << "," << mpi_type << ");";
    return result;
}

TL::Source create_gfn_f_bcast_scalar()
{
    TL::Source result;
    result << "_GfnFinishBroadcastScalar();";
    return result;
}

TL::Source create_gfn_q_reduce_scalar(std::string var_name,
                                      std::string var_cl_name,
                                      std::string mpi_type,
                                      std::string op_type,
                                      std::string group_num,
                                      std::string level1_cond,
                                      std::string level2_cond)
{
    TL::Source result;
    result 
        << "_GfnEnqueueReduceScalar(&" << var_name << "," << var_cl_name << "," << mpi_type << "," 
        << op_type << "," << group_num << "," << level1_cond << "," << level2_cond << ");";
    return result;
}

TL::Source create_gfn_f_reduce_scalar()
{
    TL::Source result;
    result << "_GfnFinishReduceScalar();";
    return result;
}

TL::Source create_gfn_malloc_nd(std::string var_name,
                                std::string var_cl_name,
                                std::string var_unique_id_name,
                                std::string mpi_type,
                                int dim_num, TL::ObjectList<TL::Expression> dim_size,
                                std::string cl_mem_flags,
                                std::string level1_cond,
                                std::string level2_cond)
{
    TL::Source result, func_name, size_params, stars;
    
    // For unknown information variable
    if (dim_size.size() == 0)
    {
        dim_num = 0;
        return result; // TODO: ok to return empty
    }
    
    stars << "*";
    func_name << "_GfnMalloc" << dim_num << "D";
    for (int i = 0; i < dim_num; ++i)
    {
        if (i != 0) size_params << ",";
        size_params << dim_size[i];
        stars << "*";
    }

    result
        << func_name << "((void" << stars << ")&" << var_name << ",&" << var_cl_name << ","
        << var_unique_id_name << "," << mpi_type << "," << size_params << ","
        << cl_mem_flags << "," << level1_cond << "," << level2_cond << ");";
    
    return result;
}

TL::Source create_gfn_free(std::string var_unique_id_name,
                           std::string level1_cond,
                           std::string level2_cond)
{
    TL::Source result;
    
    result
        << "_GfnFree(" << var_unique_id_name << "," 
        << level1_cond << "," << level2_cond << ");";
        
    return result;
}

TL::Source create_gfn_q_bcast_nd(std::string var_name,
                                 std::string var_cl_name,
                                 std::string var_unique_id_name,
                                 std::string mpi_type,
                                 int dim_num, TL::ObjectList<TL::Expression> dim_size,
                                 std::string level1_cond,
                                 std::string level2_cond)
{
    TL::Source result, size_params, subscript_to_1d;
   
    // For unknown information variable
    if (dim_size.size() == 0)
    {
        dim_num = 0;
    }
    
    for (int i = 0; i < dim_num; ++i)
    {
        if (i != 0) {
            size_params << ",";
            subscript_to_1d << "[0]";
        }
        size_params << dim_size[i];
    }
    
    result
        << "_GfnEnqueueBroadcastND((void*)" << var_name << subscript_to_1d << "," 
        << var_cl_name << "," << var_unique_id_name << "," << mpi_type << "," 
        << level1_cond << "," << level2_cond << ","
        << dim_num << ((dim_num==0)?"":",") << size_params << ");";
        
    return result;
}

TL::Source create_gfn_q_scatter_nd(std::string var_name,
                                   std::string var_cl_name,
                                   std::string var_unique_id_name,
                                   std::string mpi_type,
                                   std::string loop_start,
                                   std::string loop_end,
                                   std::string loop_step,
                                   int dim_num, TL::ObjectList<TL::Expression> dim_size,
                                   int partitioned_dim,
                                   std::string cl_mem_flags,
                                   TL::ObjectList<std::string> &pattern_array,
                                   int pattern_array_size,
                                   int pattern_type,
                                   std::string level1_cond,
                                   std::string level2_cond)
{
    TL::Source result, size_params, pattern_params, subscript_to_1d;
     
    // For unknown information variable
    if (dim_size.size() == 0)
    {
        dim_num = 0;
    }
    
    for (int i = 0; i < dim_num; ++i)
    {
        if (i != 0) {
            size_params << ",";
            subscript_to_1d << "[0]";
        }
        size_params << dim_size[i];
    }
    
    for (int i = 0; i < pattern_array_size; ++i)
    {
        pattern_params << "," << pattern_array[i];
    }
    
    result
        << "_GfnEnqueueScatterND((void*)" << var_name << subscript_to_1d << "," 
        << var_cl_name << "," << var_unique_id_name << ","
        << mpi_type << "," << cl_mem_flags << ","
        << loop_start << "," << loop_end << "," << loop_step << "," 
        << partitioned_dim << "," << pattern_type << "," 
        << level1_cond << "," << level2_cond << ","
        << dim_num << "," << pattern_array_size 
        << ((dim_num+pattern_array_size==0)?"":",") << size_params;
        
    if (pattern_array_size > 0)
        result << pattern_params;
    
    result
        << ");";
    
    return result;
}

TL::Source create_gfn_q_stream_scatter_nd(std::string kernel_id,
                                          std::string var_name,
                                          std::string var_cl_name,
                                          std::string var_unique_id_name,
                                          std::string mpi_type,
                                          std::string loop_start,
                                          std::string loop_end,
                                          std::string loop_step,
                                          int dim_num, TL::ObjectList<TL::Expression> dim_size,
                                          int partitioned_dim,
                                          std::string cl_mem_flags,
                                          TL::ObjectList<std::string> &pattern_array,
                                          int pattern_array_size,
                                          int pattern_type,
                                          std::string level1_cond,
                                          std::string level2_cond)
{
    TL::Source result, size_params, pattern_params, subscript_to_1d;
     
    // For unknown information variable
    if (dim_size.size() == 0)
    {
        dim_num = 0;
    }
    
    for (int i = 0; i < dim_num; ++i)
    {
        if (i != 0) {
            size_params << ",";
            subscript_to_1d << "[0]";
        }
        size_params << dim_size[i];
    }
    
    for (int i = 0; i < pattern_array_size; ++i)
    {
        pattern_params << "," << pattern_array[i];
    }
    
    result
        << "_GfnStreamSeqEnqueueScatterND(" << kernel_id 
        << ",(void*)" << var_name << subscript_to_1d << ","
        << var_cl_name << "," << var_unique_id_name << ","
        << mpi_type << "," << cl_mem_flags << ","
        << loop_start << "," << loop_end << "," << loop_step << "," 
        << partitioned_dim << "," << pattern_type << "," 
        << level1_cond << "," << level2_cond << ","
        << dim_num << "," << pattern_array_size 
        << ((dim_num+pattern_array_size==0)?"":",") << size_params;
        
    if (pattern_array_size > 0)
        result << pattern_params;
    
    result
        << ");";
    
    return result;
}

TL::Source create_gfn_f_dist_array()
{
    TL::Source result;
    result << "_GfnFinishDistributeArray();";
    return result;
}

TL::Source create_gfn_f_stream_dist_array()
{
    TL::Source result;
    result << "_GfnFinishStreamSeqDistributeArray();";
    return result;
}

TL::Source create_gfn_q_gather_nd(std::string var_name,
                                  std::string var_cl_name,
                                  std::string var_unique_id_name,
                                  std::string mpi_type,
                                  std::string loop_start,
                                  std::string loop_end,
                                  std::string loop_step,
                                  int dim_num, TL::ObjectList<TL::Expression> dim_size,
                                  int partitioned_dim,
                                  std::string cl_mem_flags,
                                  TL::ObjectList<std::string> &pattern_array,
                                  int pattern_array_size,
                                  int pattern_type,
                                  std::string level1_cond,
                                  std::string level2_cond)
{   
    TL::Source result, size_params, pattern_params, subscript_to_1d;
     
    // For unknown information variable
    if (dim_size.size() == 0)
    {
        dim_num = 0;
    }
    
    for (int i = 0; i < dim_num; ++i)
    {
        if (i != 0) {
            size_params << ",";
            subscript_to_1d << "[0]";
        }
        size_params << dim_size[i];
    }
    
    for (int i = 0; i < pattern_array_size; ++i)
    {
        pattern_params << "," << pattern_array[i];
    }

    result
        << "_GfnEnqueueGatherND((void*)" << var_name << subscript_to_1d << "," 
        << var_cl_name << "," << var_unique_id_name << ","
        << mpi_type << "," << cl_mem_flags << ","
        << loop_start << "," << loop_end << "," << loop_step << "," 
        << partitioned_dim << "," << pattern_type << ","
        << level1_cond << "," << level2_cond << ","
        << dim_num << "," << pattern_array_size 
        << ((dim_num+pattern_array_size==0)?"":",") << size_params;
        
    if (pattern_array_size > 0)
        result << pattern_params;
    
    result
        << ");";
    
    return result;
}

TL::Source create_gfn_q_stream_gather_nd(std::string kernel_id,
                                         std::string var_name,
                                         std::string var_cl_name,
                                         std::string var_unique_id_name,
                                         std::string mpi_type,
                                         std::string loop_start,
                                         std::string loop_end,
                                         std::string loop_step,
                                         int dim_num, TL::ObjectList<TL::Expression> dim_size,
                                         int partitioned_dim,
                                         std::string cl_mem_flags,
                                         TL::ObjectList<std::string> &pattern_array,
                                         int pattern_array_size,
                                         int pattern_type,
                                         std::string level1_cond,
                                         std::string level2_cond)
{
    TL::Source result, size_params, pattern_params, subscript_to_1d;
     
    // For unknown information variable
    if (dim_size.size() == 0)
    {
        dim_num = 0;
    }
    
    for (int i = 0; i < dim_num; ++i)
    {
        if (i != 0) {
            size_params << ",";
            subscript_to_1d << "[0]";
        }
        size_params << dim_size[i];
    }
    
    for (int i = 0; i < pattern_array_size; ++i)
    {
        pattern_params << "," << pattern_array[i];
    }

    result
        << "_GfnStreamSeqEnqueueGatherND(" << kernel_id 
        << ",(void*)" << var_name << subscript_to_1d << ","
        << var_cl_name << "," << var_unique_id_name << ","
        << mpi_type << "," << cl_mem_flags << ","
        << loop_start << "," << loop_end << "," << loop_step << "," 
        << partitioned_dim << "," << pattern_type << "," 
        << level1_cond << "," << level2_cond << ","
        << dim_num << "," << pattern_array_size 
        << ((dim_num+pattern_array_size==0)?"":",") << size_params;
        
    if (pattern_array_size > 0)
        result << pattern_params;
    
    result
        << ");";
    
    return result;
}

TL::Source create_gfn_f_stream_gather_nd(std::string kernel_id,
                                         std::string var_name,
                                         std::string var_cl_name,
                                         std::string var_unique_id_name,
                                         std::string mpi_type,
                                         std::string loop_start,
                                         std::string loop_end,
                                         std::string loop_step,
                                         int dim_num, TL::ObjectList<TL::Expression> dim_size,
                                         int partitioned_dim,
                                         std::string cl_mem_flags,
                                         TL::ObjectList<std::string> &pattern_array,
                                         int pattern_array_size,
                                         int pattern_type,
                                         std::string level1_cond,
                                         std::string level2_cond)
{
    TL::Source result, size_params, pattern_params, subscript_to_1d;
     
    // For unknown information variable
    if (dim_size.size() == 0)
    {
        dim_num = 0;
    }
    
    for (int i = 0; i < dim_num; ++i)
    {
        if (i != 0) {
            size_params << ",";
            subscript_to_1d << "[0]";
        }
        size_params << dim_size[i];
    }
    
    for (int i = 0; i < pattern_array_size; ++i)
    {
        pattern_params << "," << pattern_array[i];
    }

    result
        << "_GfnStreamSeqFinishGatherND(" << kernel_id 
        << ",(void*)" << var_name << subscript_to_1d << ","
        << var_cl_name << "," << var_unique_id_name << ","
        << mpi_type << "," << cl_mem_flags << ","
        << loop_start << "," << loop_end << "," << loop_step << "," 
        << partitioned_dim << "," << pattern_type << "," 
        << level1_cond << "," << level2_cond << ","
        << dim_num << "," << pattern_array_size 
        << ((dim_num+pattern_array_size==0)?"":",") << size_params;
        
    if (pattern_array_size > 0)
        result << pattern_params;
    
    result
        << ");";
    
    return result;
}

TL::Source create_gfn_f_gather_array()
{
    TL::Source result;
    result << "_GfnFinishGatherArray();";
    return result;
}

TL::Source create_gfn_f_stream_gather_array()
{
    TL::Source result;
    result << "_GfnFinishStreamSeqGatherArray();";
    return result;
}
