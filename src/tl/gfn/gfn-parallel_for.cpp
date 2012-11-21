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



#include "gfn-parallel_for.hpp"
#include "gfn-config.hpp"
#include <sstream>

using namespace TL::GFN;

ParallelFor TL::GFN::parallel_for(PragmaCustomConstruct construct, 
                                  TL::ForStatement for_stmt, 
                                  KernelInfo *kernel_info)
{
    return ParallelFor(construct, for_stmt, kernel_info);
}


TL::Source ParallelFor::get_source()
{
    // Nothing at this moment
    return do_parallel_for();
}

ParallelFor::ParallelFor(PragmaCustomConstruct construct, 
                         ForStatement for_stmt, 
                         KernelInfo *kernel_info)
     : _construct(construct), _for_stmt(for_stmt), 
       _function_def(0), _kernel_info(kernel_info)
{
     ;
}

TL::Source ParallelFor::do_kernel_config(Expression &lower_bound,
                                         Expression &upper_bound,
                                         Expression &step)
{
    // calculate thread num [ ast-node.cxx : SetThreadNum() ]
    TL::Source range, thread_num, block_num, thread_p_block_num;
    {
        // range = (upper - lower)
        bool lower_bound_is_const = false;
        int lower_bound_val = lower_bound.evaluate_constant_int_expression(lower_bound_is_const);
        if (lower_bound_is_const && (lower_bound_val == 0)) {
            range
                << "( " << upper_bound.prettyprint() << " )";
        } else {
            range
                << "( " << upper_bound.prettyprint() << " - " << lower_bound.prettyprint() << " )";
        }

        // thread_num = (range / step) + 1
        bool step_is_const = false;
        int step_val = step.evaluate_constant_int_expression(step_is_const);
        if (step_is_const && (step_val == 1)) {
            thread_num
                << "( " << range << " + 1 )";
        } else {
            thread_num
                << "( ( " << range << " / " << step.prettyprint() << " ) + 1 )";
        }

        // TODO: reduce expr?
        bool valid = true;
        /*if (!expr.is_constant())
        {
            throw HLTException(expr, "factor clause argument should be a constant expression");
        }
        thread_num.parse_expression(_for_stmt.get_ast(), _for_stmt.get_scope_link()).get_expression();
        unroll_factor = expr.evaluate_constant_int_expression(valid);
        if (!valid)
        {
            throw HLTException(expr, "factor clause argument expression could not be evaluated");
        }*/
        
        
        if (0 /* thread_num is integer literal */) 
        {
            ;
        }
        else
        {
            // thread_p_block = 512
            thread_p_block_num
                << "512";
            // block_num = 
            block_num
                << "( ( " << thread_num << " - 1 ) + 512.0) / 512";
            // TODO: reduce expr??
        }
    }
    TL::Source new_config;
    new_config << "<<<" << block_num << "," << thread_p_block_num << ">>>" ;
    return new_config;
}

/* int MPI_Send(void *buf,
int count,
MPI_Datatype datatype,
int dest,
    int tag,
MPI_Comm comm)*/
static TL::Source createMpiSend() {
    TL::Source send_fn;
    return send_fn;
}

/*int MPI_Recv(void *buf, int count, MPI_Datatype datatype,
    int source, int tag, MPI_Comm comm, MPI_Status *status) */
static TL::Source createMpiRecv() {
    TL::Source recv_fn;
    return recv_fn;
}

/*int MPI_Reduce(void *sendbuf, void *recvbuf, int count,
    MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm)*/
static TL::Source createMpiReduce() {
    TL::Source reduce_fn;
    return reduce_fn;
}

/*int MPI_Allreduce(void *sendbuf, void *recvbuf, int count,
    MPI_Datatype datatype, MPI_Op op, MPI_Comm comm)*/
static TL::Source createMpiAllReduce() {
    TL::Source allreduce_fn;

    /*allreduce_fn
        << "MPI_Allreduce(" << sendbuf << ", " << recvbuf << ", "
        << count << ", " <<*/

    return allreduce_fn;
}

static TL::Source createMpiBcast(std::string buffer, std::string count,
                                 std::string type, std::string root,
                                 std::string comm) {
    TL::Source bcast_fn;

    bcast_fn
        << "MPI_Bcast(" << buffer << ", " << count << ", " << type << ","
        << root << ", " << comm << ");";

    return bcast_fn;
}

/* int MPI_Scatter( void *sendbuf,
 *                  int sendcount,
 *                  MPI_Datatype sendtype,
 *                  void *recvbuf,
 *                  int recvcount,
 *                  MPI_Datatype recvtype,
 *                  int root,
 *                  MPI_Comm comm)
 */
static TL::Source createMpiScatter() {

    TL::Source scatter_fn;
    return scatter_fn;
}

TL::Source ParallelFor::do_parallel_for()
{
    if (!_for_stmt.regular_loop())
    {
        throw GFNException(_for_stmt, "support only simple for loop");
    }

    // Get parts of the loop
    IdExpression induction_var = _for_stmt.get_induction_variable();
    Expression lower_bound = _for_stmt.get_lower_bound();
    Expression upper_bound = _for_stmt.get_upper_bound();
    Expression step = _for_stmt.get_step();
    //TL::Source operator_bound = _for_stmt.get_bound_operator();
    
    Statement loop_body = _for_stmt.get_loop_body();

    // XXX: _function_def is function that call for_stmt
    _function_def = new FunctionDefinition(_for_stmt.get_enclosing_function());

    /*==------------------- Cluster CG section ---------------------==*/
    if (Conf_Trans_flags & GFN_TRANS_MPI) {
        TL::Source new_for_stmt, new_init, new_cond, new_step;

        TL::Statement first_expr_in_func = _function_def->get_function_body()
                .get_first_expr_statement();
        TL::Statement last_decl_in_func = _function_def->get_function_body()
                .get_last_decl_statement();

        /*  Declare MPI rank and proc_num in function
            Exmaple :   int _gfn_proc_num, _gfn_rank; */
        TL::Source mpi_decl;
        mpi_decl
            << "int " << GFN_PROC_NUM_VAR << ", "
            << GFN_RANK_VAR << ";"
            << "MPI_Comm " << GFN_COMM << " = MPI_COMM_WORLD;";

        TL::AST_t mpi_decl_tree = mpi_decl.parse_declaration(
                _function_def->get_point_of_declaration(),
                _function_def->get_scope_link());
        last_decl_in_func.get_ast().append(mpi_decl_tree);

        /*  Initialize MPI
            Exmaple :   MPI_Comm_size(_gfn_comm, &_gfn_proc_num);
                        MPI_Comm_rank(_gfn_comm, &_gfn_rank); */
        TL::Source mpi_init;
        mpi_init
            << "MPI_Comm_size(" << GFN_COMM << ", &" << GFN_PROC_NUM_VAR << ");"
            << "MPI_Comm_rank(" << GFN_COMM << ", &" << GFN_RANK_VAR << ");";

        TL::AST_t mpi_init_tree = mpi_init.parse_statement(
                _function_def->get_point_of_declaration(),
                _function_def->get_scope_link());
        first_expr_in_func.get_ast().prepend(mpi_init_tree);

        /* Data send from rank 0 to all */
        for (ObjectList<DataReference>::iterator it = _kernel_info->_use_list.begin();
             it != _kernel_info->_use_list.end();
             ++it)
        {
            /*if (!contain(private_list, *it))
            {
                std::string var_name = it->get_base_symbol().get_name();
                std::string size = _kernel_info->_dim_size_list[var_name]._dim1_size;

                memcpy_h2d
                    << do_gfn_memcpy_h2d(var_name, size);
            }*/
        }

        /* Data send back to master */

        new_init
            << induction_var.prettyprint() << " = _gfn_rank";
        // TODO: <=, >= ??
        new_cond
            << induction_var.prettyprint() << " <= "
            << upper_bound.prettyprint();
        // TODO: +=, -= ??
        new_step
            << induction_var.prettyprint() << " += ("
            << step.prettyprint() << " * " << GFN_PROC_NUM << ")";
        new_for_stmt
            << "for(" << new_init << ";" << new_cond << ";" << new_step << ")"
            << loop_body;
        return new_for_stmt;
    }


    /*==--------------------- GPU CG section -----------------------==*/
    TL::Source result, device_var_decl, memcpy_h2d, memcpy_d2h;
    TL::Source new_param_list;
    std::string kernel_name = get_new_kernel_name();
    
    // declare thread id variable
    TL::Source thread_id_decl = do_thread_id_declaration();
    TL::Source loop_index_decl = do_loop_index_declaration(induction_var.get_symbol(),
                                                           step,
                                                           lower_bound);
    TL::Source private_var_decl;
    
    TL::Source kernel_def, kernel_param, loop_body_replace;
    kernel_def
        << "__global__ void "
        << kernel_name << "("
        << kernel_param
        << ") { "
        << thread_id_decl
        << loop_index_decl
        << private_var_decl
        << "if (" << induction_var.prettyprint() << "<=" << GFN_UPPER_BOUND << ")"
            << "{" << loop_body_replace << "}"
        << "}";
    
    ObjectList<DataReference> private_list = _kernel_info->get_private_list();
    
    // XXX: get all variable in for loop
    _var_list = loop_body.all_symbol_occurrences(TL::Statement::ONLY_VARIABLES);
    ObjectList<std::string> formal_param_list;
    for (ObjectList<IdExpression>::iterator it = _var_list.begin();
         it != _var_list.end();
         it++)
    {
        DataReference data_ref = it->get_expression();
        if (it->prettyprint() != induction_var.prettyprint() &&
            !contain(private_list, data_ref))
        {
            formal_param_list.push_back(GFN_DEVICE_PREFIX + (it->prettyprint()));
            kernel_param.append_with_separator(
                get_declaration(_construct, data_ref), ",");

            // XXX: declare device variable
            if (!is_already_declare(_for_stmt.get_scope(), it->prettyprint()))
            {
                device_var_decl
                    << get_declaration(_construct, data_ref, GFN_DEVICE_PREFIX)
                    << ";";
                add_to_scope(_for_stmt.get_scope(), it->prettyprint());
            }
        }
    }
    formal_param_list.push_back(upper_bound.prettyprint());
    kernel_param.append_with_separator("int " GFN_UPPER_BOUND, ",");

    // XXX: memcpy host to device
    for (ObjectList<DataReference>::iterator it = _kernel_info->_use_list.begin();
         it != _kernel_info->_use_list.end();
         ++it)
    {
        if (!contain(private_list, *it))
        {
            std::string var_name = it->get_base_symbol().get_name();
            std::string size = _kernel_info->_dim_size_list[var_name]._dim1_size;
                /* + " * " + _kernel_info->_dim_size_list[var_name]._dim2_size */

            memcpy_h2d
                << do_gfn_memcpy_h2d(var_name, size);
        }
    }

    // XXX: memcpy device to host
    for (ObjectList<DataReference>::iterator it = _kernel_info->_def_list.begin();
         it != _kernel_info->_def_list.end();
         ++it)
    {
        if (!contain(private_list, *it))
        {
            std::string var_name = it->get_base_symbol().get_name();
            std::string size = _kernel_info->_dim_size_list[var_name]._dim1_size;
                /* + " * " + _kernel_info->_dim_size_list[var_name]._dim2_size */

            memcpy_d2h
                << do_gfn_memcpy_d2h(var_name, size);
        }
    }

    // XXX: declare private variable
    private_var_decl
        << get_declarations_inline(_construct, private_list, "private");

    
    // XXX: create kernel body
    ObjectList<Statement> statement_list = loop_body.get_inner_statements();
    for (ObjectList<Statement>::iterator it = statement_list.begin();
         it != statement_list.end();
         it++)
    {
        loop_body_replace << *it;
    }
    
    // XXX: create kernel define tree
    TL::AST_t kernel_def_tree = kernel_def.parse_declaration(
            _function_def->get_point_of_declaration(),
            _function_def->get_scope_link());
    
    // XXX: create Function Def object
    //extract_define_device_function(loop_body);

    // XXX: prepend kernel
    _function_def->get_ast().prepend_sibling_function(kernel_def_tree);

    /* Add device variable declaration to top of function */
    TL::AST_t device_var_decl_tree = device_var_decl.parse_declaration(
            _function_def->get_point_of_declaration(),
            _function_def->get_scope_link());
    _function_def->get_function_body().get_inner_statements()
            .front().get_ast().prepend(device_var_decl_tree);

    result
        << memcpy_h2d
        << kernel_name
        << do_kernel_config(lower_bound, upper_bound, step) /// <<< block, thread_p_block >>>
        << "(" << new_param_list << ");"
        << memcpy_d2h;
    
    for (ObjectList<std::string>::iterator it = formal_param_list.begin();
         it != formal_param_list.end();
         it++)
    {
        new_param_list.append_with_separator(*it, ",");
    }
    
    return result;
}
#if 0
void ParallelFor::extract_define_device_function(Statement compound_stmt) {
    Statement function_body = gpu_function.get_enclosing_statement();
    
    ObjectList<IdExpression> symbol_list = function_body.all_symbol_occurrences(TL::Statement::ONLY_FUNCTIONS);
    OjjectList<TL::AST_t> device_list;
    for (ObjectList<IdExpression>::iterator it = symbol_list.begin();
         it != symbol_list.end();
         it++)
    {
        if (0/* prefix is GFN_GEN_PREFIX or have definition in gpu eg. sin cos tan*/)
        {
            // DO NOTHING
        }
        else if (1/*can find definition*/)
        {
            FunctionDefinition func_def = it->get_enclosing_function();
            std::cout << func_def.prettyprint() << std::endl;
            // TODO: define as __device__
            TL::Source device_func_src;
            //device_func_src
                //<< "__device__"
                //<< 
            // TODO: change function name to gfn generate in function call and definition
            // TODO: check that it is not recirsive
            
            TL::AST_t device_func_tree = device_func_src.parse_declaration(
                gpu_function->get_point_of_declaration(),
                gpu_function->get_scope_link());
            
            gpu_function.prepend_sibling_function(device_func_tree);
            device_list.push_back(device_func_tree);
        }
        else
        {
            throw GFNException("Cannot found definition of '%s' function.", it->prettyprint());
        }
    }
    
    FunctionDefinition (AST_t ref, ScopeLink scope_link)
    
    // do it recursively
    for (ObjectList<TL::AST_t>::iterator it = device_list.begin();
         it != device_list.end();
         it++)
    {
        extract_define_device_function(*it);
    }
}
#endif

TL::Source ParallelFor::do_loop_index_declaration(TL::Symbol loop_index,
                                                  TL::Expression loop_increment,
                                                  TL::Expression loop_lowerbound)
{
    TL::Source index_declaration;
    Type type = loop_index.get_type();
    
    // TOOD: simplify expression
    index_declaration
        << comment("loop index entity");
    index_declaration
        << type.get_declaration(_construct.get_scope(), loop_index.get_name())
        << "="
        << "("
        << GFN_THREAD_ID_VAR << "*" << loop_increment
        << ")"
        << "+" << loop_lowerbound
        << ";";

    return index_declaration;
}

TL::Source ParallelFor::do_thread_id_declaration()
{
    TL::Source thread_id_decl;
    
    thread_id_decl 
        << comment("kernel thread id entity");
    
    if (0 /* block_num == 1 */)
    {
        thread_id_decl
            << "int " GFN_THREAD_ID_VAR " = threadIdx.x;";
    }
    else
    {
        thread_id_decl
            << "int " GFN_THREAD_ID_VAR " = blockIdx.x * blockDim.x + threadIdx.x;";
    }
    
    return thread_id_decl;
}

TL::Source ParallelFor::do_gfn_malloc(std::string &identifier,
                                      TL::Source &malloc_size)
{
    TL::Source malloc_call;
    malloc_call
        << "_gfn_malloc("
        << GFN_DEVICE_PREFIX << identifier
        << ","
        << malloc_size
        << ");";

    return malloc_call;
}

// TODO: potential_malloc

// TODO: if variable's declaration is in function (not function args)
TL::Source ParallelFor::do_gfn_mfree(std::string &identifier)
{
    TL::Source potential_mfree;
    potential_mfree
        << "_gfn_potential_mfree("
        << GFN_DEVICE_PREFIX << identifier
        << ");";

    return potential_mfree;
}

TL::Source ParallelFor::do_gfn_memcpy_h2d(std::string &identifier,
                                          std::string &copy_size)
{
    TL::Source memcpy_call;
    memcpy_call
        << "_gfn_memcpy_h2d("
        << identifier
        << ","
        << GFN_DEVICE_PREFIX << identifier
        << ","
        << copy_size
        << ");";
    return memcpy_call;
}

TL::Source ParallelFor::do_gfn_memcpy_d2h(std::string &identifier,
                                          std::string &copy_size)
{
    TL::Source memcpy_call;
    memcpy_call
        << "_gfn_memcpy_d2h("
        << GFN_DEVICE_PREFIX << identifier
        << ","
        << identifier
        << ","
        << copy_size
        << ");";
    return memcpy_call;
}

bool ParallelFor::contain(ObjectList<DataReference> &list, DataReference &obj)
{
    for (ObjectList<DataReference>::iterator it = list.begin();
         it != list.end();
         it++)
    {
        if (it->get_base_symbol().get_name() == obj.get_base_symbol().get_name())
        {
            return true;
        }
    }
    return false;
}
