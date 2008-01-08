/*
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
    
    $Id: tl-acotestransform.cpp 1611 2007-07-10 09:28:44Z drodenas $
*/
#include "tl-acotescompilerphase.h"

#include "tl-acotestransform.h"
#include "tl-finalizerconstruct.h"
#include "tl-initializerconstruct.h"
#include "tl-sharedconstruct.h"
#include "tl-taskconstruct.h"
#include "tl-taskgroupconstruct.h"
#include "tl-userportconstruct.h"

namespace TL { namespace Acotes {

    
    /* ****************************************************************
     * * CompilerPhase management
     * ****************************************************************/    
    
    /**
     * Constructor.
     */
    AcotesCompilerPhase::AcotesCompilerPhase() 
    : PragmaCustomCompilerPhase("acotes")
    , pragmaDispatcher("acotes", on_directive_pre, on_directive_post)
    {
        on_directive_pre["taskgroup"].connect(
            functor(&AcotesCompilerPhase::onPreTaskgroupConstruct, *this)
            );
        on_directive_post["taskgroup"].connect(
            functor(&AcotesCompilerPhase::onPostTaskgroupConstruct, *this)
            );
        register_construct("taskgroup");
        
        on_directive_pre["task"].connect(
            functor(&AcotesCompilerPhase::onPreTaskConstruct, *this)
            );
        on_directive_post["task"].connect(
            functor(&AcotesCompilerPhase::onPostTaskConstruct, *this)
            );
        register_construct("task");
        
        on_directive_pre["initializer"].connect(
            functor(&AcotesCompilerPhase::onPreInitializerConstruct, *this)
            );
        on_directive_post["initializer"].connect(
            functor(&AcotesCompilerPhase::onPostInitializerConstruct, *this)
            );
        register_construct("initializer");
        
        on_directive_pre["finalizer"].connect(
            functor(&AcotesCompilerPhase::onPreFinalizerConstruct, *this)
            );
        on_directive_post["finalizer"].connect(
            functor(&AcotesCompilerPhase::onPostFinalizerConstruct, *this)
            );
        register_construct("finalizer");
        
        on_directive_pre["port"].connect(
            functor(&AcotesCompilerPhase::onPreUserPortConstruct, *this)
            );
        on_directive_post["port"].connect(
            functor(&AcotesCompilerPhase::onPostUserPortConstruct, *this)
            );
        register_construct("port");
         
        on_directive_pre["shared"].connect(
            functor(&AcotesCompilerPhase::onPreSharedConstruct, *this)
            );
        on_directive_post["shared"].connect(
            functor(&AcotesCompilerPhase::onPostSharedConstruct, *this)
            );
        register_construct("shared");
    }
    
    /**
     * Destructor.
     */
    AcotesCompilerPhase::~AcotesCompilerPhase() {
        
    }
    
    /**
     * AcotesCompilerPhase implementation.
     */
    void 
    AcotesCompilerPhase::run(DTO& data_flow)
    {
        std::cout << "AcotesCompilerPhase run"<< std::endl;
        
        // get the translation_unit tree
        AST_t translation_unit = data_flow["translation_unit"];
        // get the scope_link
        ScopeLink scope_link = data_flow["scope_link"];
        // Get the global_scope
        Scope global_scope = scope_link.get_scope(translation_unit);

        // Instantiate a DepthTraverse
        DepthTraverse depth_traverse;

        PredicateAST<LANG_IS_PRAGMA_CUSTOM_DIRECTIVE> pragmaCustomDirectivePred;
        PredicateAST<LANG_IS_PRAGMA_CUSTOM_CONSTRUCT> pragmaCustomConstructPred;
//        PredicateAST<LANG_IS_FUNCTION_CALL> function_call_pred;

        depth_traverse.add_predicate(pragmaCustomDirectivePred, pragmaDispatcher);
        depth_traverse.add_predicate(pragmaCustomConstructPred, pragmaDispatcher);
//        depth_traverse.add_predicate(function_call_pred, *this);

        // Parse everything
        depth_traverse.traverse(translation_unit, scope_link);
        
        // Transform parsed source
        AcotesTransform::transform();
    }


    
    /* ****************************************************************
     * * CompilerPhase events
     * ****************************************************************/
    
    void AcotesCompilerPhase::onPreTaskConstruct(PragmaCustomConstruct construct)
    {
        TaskConstruct task(construct);
        task.onPre();
    }

    void AcotesCompilerPhase::onPostTaskConstruct(PragmaCustomConstruct construct)
    {
        TaskConstruct task(construct);
        task.onPost();
    }
    
    void AcotesCompilerPhase::onPreTaskgroupConstruct(PragmaCustomConstruct construct)
    {
        TaskgroupConstruct taskgroup(construct);
        taskgroup.onPre();
    }

    void AcotesCompilerPhase::onPostTaskgroupConstruct(PragmaCustomConstruct construct)
    {
        TaskgroupConstruct taskgroup(construct);
        taskgroup.onPost();
    }
    
    void AcotesCompilerPhase::onPreInitializerConstruct(PragmaCustomConstruct construct)
    {
        InitializerConstruct initializer(construct);
        initializer.onPre();
    }

    void AcotesCompilerPhase::onPostInitializerConstruct(PragmaCustomConstruct construct)
    {
        InitializerConstruct initializer(construct);
        initializer.onPost();
    }
    
    void AcotesCompilerPhase::onPreFinalizerConstruct(PragmaCustomConstruct construct)
    {
        FinalizerConstruct finalizer(construct);
        finalizer.onPre();
    }
    
    void AcotesCompilerPhase::onPostFinalizerConstruct(PragmaCustomConstruct construct)
    {
        FinalizerConstruct finalizer(construct);
        finalizer.onPost();
    }
 
    void AcotesCompilerPhase::onPreUserPortConstruct(PragmaCustomConstruct construct)
    {
        UserPortConstruct userPort(construct);
        userPort.onPre();
    }
    
    void AcotesCompilerPhase::onPostUserPortConstruct(PragmaCustomConstruct construct)
    {
        UserPortConstruct userPort(construct);
        userPort.onPost();
    }

    void AcotesCompilerPhase::onPreSharedConstruct(PragmaCustomConstruct construct)
    {
        SharedConstruct shared(construct);
        shared.onPre();
    }
    
    void AcotesCompilerPhase::onPostSharedConstruct(PragmaCustomConstruct construct)
    {
        SharedConstruct shared(construct);
        shared.onPost();
    }
 
    
    
    /* ****************************************************************
     * * TraverseFunction management
     * ****************************************************************/    

    /**
     * Travese functor implementation.
     */
    void 
    AcotesCompilerPhase::preorder(Context ctx, AST_t node)
    {
//        AcotesTaskCall acotes_task_call(node, ctx.scope_link);
//        Task* task= acotes_task_call.get_task();
//        if (task) 
//        {
//            if (TaskStack::top()) {
//                on_task_call_pre(acotes_task_call);
//            } else {
//                ErrorReport::I()->warning(node)
//                    << "found function call to task "
//                    << "'" << task->get_name() << "'"
//                    << " outside to task or taskgroup, will not be streamized"
//                    << std::endl;
//            }
//        }
    }

    /**
     * TraverseFunctor implementation.
     */
    void 
    AcotesCompilerPhase::postorder(Context ctx, AST_t node)
    {
//        AcotesTaskCall acotes_task_call(node, ctx.scope_link);
//        Task* task= acotes_task_call.get_task();
//        if (task)
//        {
//            if (TaskStack::top()) {
//                on_task_call_post(acotes_task_call);
//            } 
//        }
    }

    
    
} /* end namespace Acotes */ } /* end namespace TL */

EXPORT_PHASE(TL::Acotes::AcotesCompilerPhase);