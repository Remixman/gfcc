/*
    Mercurium C/C++ Compiler
    Copyright (C) 2006-2007 - Roger Ferrer Ibanez <roger.ferrer@bsc.es>
	Acotes Translation Phase
	Copyright (C) 2007 - David Rodenas Pico <david.rodenas@bsc.es>
    Barcelona Supercomputing Center - Centro Nacional de Supercomputacion
    Universitat Politecnica de Catalunya

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include "tl-transformtaskoutline.hpp"

#include <assert.h>

#include "tl-mintakatransformhelper.hpp"
#include "tl-streamtransformhelper.hpp"
#include "tl-symboltransformhelper.hpp"
#include "tl-tasktransformhelper.hpp"
#include "tl-taskgroupinfo.hpp"


namespace TL
{

// TransformTaskOutline constructor --------------------------------------------
TransformTaskOutline::
TransformTaskOutline
		( const PragmaCustomConstruct& pragma_custom_construct
		, TaskInfo* task_info
		)
		: _pragma_custom_construct(pragma_custom_construct)
		, _task_info(task_info)
{
}

// TransformTaskOutline destructor ---------------------------------------------
TransformTaskOutline::
~TransformTaskOutline
		(
		)
{
}

// transform -------------------------------------------------------------------
void
TransformTaskOutline::
transform
		(
		)
{
	Source declares_src= this->generate_outline();
	
	// Add outline task
	FunctionDefinition function_definition 
		= _pragma_custom_construct.get_enclosing_function();
		
	AST_t function_ast = function_definition.get_ast();
	ScopeLink function_scope_link = function_definition.get_scope_link();
	
	AST_t task_add_tree = declares_src
			.parse_global(function_ast, function_scope_link);
		
	function_definition.get_ast().prepend_sibling_function(task_add_tree);
}

// generate_body ---------------------------------------------------------------
std::string 
TransformTaskOutline::
generate_body
		( void
		)
{
	std::stringstream ss;
	
	// original body
	Statement task_body= _pragma_custom_construct.get_statement();
	
	// replace references
	Statement modified_body= SymbolTransformHelper::
			transform_all_to_reference(_task_info->get_references(), task_body);
	
	
	ss << modified_body.prettyprint();
	
	return ss.str();
}

// generate_closes -------------------------------------------------------------
std::string 
TransformTaskOutline::
generate_closes
		( void
		)
{
	std::stringstream ss;
	
	ss << StreamTransformHelper::
			close_all(_task_info->get_loop_close_ostream_set());
			
	return ss.str();
}

// generate_declares -----------------------------------------------------------
std::string 
TransformTaskOutline::
generate_declares
		( void
		)
{
	std::stringstream ss;
	
	ss << SymbolTransformHelper::declare_all(_task_info->get_privates());
	ss << SymbolTransformHelper::
			declare_reference_all(_task_info->get_references());
	
	return ss.str();
}

// generate_eos ----------------------------------------------------------------
std::string 
TransformTaskOutline::
generate_eos
		( void
		)
{
	std::stringstream ss;
	
	ss		<< "(!("
			<< StreamTransformHelper::
					eos_any(_task_info->get_loop_control_istream_set())
			<< "))"
			;
			
	return ss.str();
}

// generate_outline ------------------------------------------------------------
std::string
TransformTaskOutline::
generate_outline
		(
		)
{
	std::stringstream ss;
	
	ss		<< "static void* "
			<< TaskTransformHelper::outline_name(_task_info)
			<< "(void * arg)"
			<< "{"
			<<   MintakaTransformHelper::initialize_task(_task_info)
			<<   generate_declares()
			<<   "while (" << generate_pops_expression() << ")"
			<<   "{"
			<<     generate_body()
			<<     generate_pushes()
			<<   "}"
			<<   generate_closes()
			<<   MintakaTransformHelper::finalize_task(_task_info)
			<< "}"
			;
	
	return ss.str();
}

// generate_peeks --------------------------------------------------------------
std::string 
TransformTaskOutline::
generate_peeks
		( void
		)
{
	std::stringstream ss;
	
	ss << StreamTransformHelper::
			peek_all(_task_info->get_loop_pop_istream_set());
	
	return ss.str();
}

// generate_pops ---------------------------------------------------------------
std::string 
TransformTaskOutline::
generate_pops
		( void
		)
{
	std::stringstream ss;
	
	ss << StreamTransformHelper::
			pop_all(_task_info->get_loop_pop_istream_set());
	
	return ss.str();
}

// generate_pops_expression ----------------------------------------------------
std::string 
TransformTaskOutline::
generate_pops_expression
		( void
		)
{
	std::stringstream ss;
	
	ss << StreamTransformHelper::
			pop_all_expression(_task_info->get_loop_pop_istream_set());
	
	return ss.str();
}

// generate_pushes -------------------------------------------------------------
std::string 
TransformTaskOutline::
generate_pushes
		( void
		)
{
	std::stringstream ss;
	
	ss << StreamTransformHelper::
			push_all(_task_info->get_loop_push_ostream_set());
	
	return ss.str();
}

// generate_waits --------------------------------------------------------------
std::string 
TransformTaskOutline::
generate_waits
		( void
		)
{
	std::stringstream ss;
	
	ss << StreamTransformHelper::
			wait_istream_all(_task_info->get_loop_pop_istream_set());
	ss << StreamTransformHelper::
			wait_ostream_all(_task_info->get_loop_push_ostream_set());
	
	return ss.str();
}

}
