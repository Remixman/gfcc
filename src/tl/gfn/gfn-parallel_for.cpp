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
    
    TL::Source result, device_var_decl, memcpy_h2d, memcpy_d2h;
    TL::Source new_param_list;
    std::string kernel_name = get_new_kernel_name();
    
    // XXX: find use and def variable
    find_use_and_def_list(loop_body);
    
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
 
    // XXX: _function_def is function that call for_stmt
    _function_def = new FunctionDefinition(_for_stmt.get_enclosing_function());
    
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
            formal_param_list.push_back(it->prettyprint());
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
    
    result 
        << device_var_decl
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
void ParallelFor::find_use_and_def_list(TL::Statement compound_stmt)
{
    

    if (!compound_stmt.is_compound_statement())
    {
        std::cerr 
            << "Error at gfn-parallel_for.cpp:find_use_and_def_list\n"; find_use_and_def_list
    }
  
    ObjectList<Statement> statements = compound_stmt.get_inner_statements();
  
    /* FIXME: if def before use eg.
     * A[i] = 6;
     * B[i] = A[i];
     * A is def but also use;
     */
    for (ObjectList<Statement>::iterator it = statements.begin();
         it != statements.end();
         it++)
    {
        Expression expression = it->get_expression();
        if (it->is_compound_statement())
        {
            find_use_and_def_list(*it);
        }
        else if (expression.is_assignment() || expression.is_operation_assignment())
        {
            DataReference def_var_ref(expression.get_first_operand());
            if (!contain(_def_list, def_var_ref))
            {
                _def_list.push_back(def_var_ref);
            }
        }
        else
        {
            /* if pass to function we don't know use or def
             * add to both list.
             */
            // XXX: get all variable in statement
            ObjectList<IdExpression> stmt_var_list = it->all_symbol_occurrences(Statement::ONLY_VARIABLES);
            for (ObjectList<IdExpression>::iterator iit = stmt_var_list.begin();
                 iit != stmt_var_list.end();
                 iit++)
            {
                DataReference data_ref(iit->get_expression());
                if (!contain(_use_list, data_ref))
                {
                    _use_list.push_back(data_ref);
                }
            }
            
        }
    }
}

TL::Source ParallelFor::do_loop_index_declaration(TL::Symbol loop_index,
                                                  TL::Expression loop_increment,
                                                  TL::Expression loop_lowerbound)
{
    TL::Source index_declaration;
    Type type = loop_index.get_type();
    
    index_declaration
        << comment("loop index entity");
    index_declaration
        << type.get_declaration(_construct.get_scope(), loop_index.get_name())
        << "="
        << GFN_THREAD_ID_VAR << "*" << loop_increment
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
        << identifier
        << ","
        << malloc_size
        << ");";

    return malloc_call;
}

TL::Source ParallelFor::do_gfn_mfree(std::string &identifier);
TL::Source ParallelFor::do_gfn_memcpy(std::string &identifier,
                                      TL::Source &copy_size,
                                      CUDA_MEMCPY_KIND memcpy_kind);

TL::Source ParallelFor::do_cuda_malloc(std::string &identifier, Source &malloc_size)
{
    TL::Source malloc_call;
    malloc_call
        << "cudaMalloc((void**)&"
        << identifier
        << ","
        << malloc_size
        << ");";

    return malloc_call;
}

TL::Source ParallelFor::do_cuda_free(std::string &identifier)
{
    TL::Source free_call;
    free_call
        << "cudaFree( " << identifier << " );";

    return free_call;
}

TL::Source ParallelFor::do_cuda_memcpy(std::string &identifier, Source &malloc_size, 
                           CUDA_MEMCPY_KIND memcpy_kind)
{
    // TODO: add src_iden, desc_iden
    TL::Source memcpy_call;
    memcpy_call
        << "cudaMemcpy( &" << identifier << " );";

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
