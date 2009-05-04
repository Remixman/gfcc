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
// 
// File:   tl-tasktransform.h
// Author: drodenas
//
// Created on 20 / desembre / 2007, 11:49
//

#ifndef _TL_TASKTRANSFORM_H
#define	_TL_TASKTRANSFORM_H

#include <string>
#include <tl-langconstruct.hpp>

namespace TL { namespace Acotes {
    
    class Task;
    
    class TaskTransform {
    // -- Constructor
    public:
        TaskTransform(const std::string& driver);
    protected:
        const std::string driver;
        
    // -- Transform
    public:
        virtual void transform(Task* task);
    private:
        virtual void transformChildren(Task* task);
        virtual void transformAddOutline(Task* task);
        virtual void transformReplacePeek(Task* task);
        virtual void transformReplaceVariable(Task* task);
        virtual void transformReplaceUserPort(Task* task);
        virtual void transformReplaceSharedCheck(Task* task);
        virtual void transformReplaceSharedUpdate(Task* task);
        virtual void transformReplaceTeamReplicate(Task* task);
        virtual void transformReplaceConstruct(Task* task);
        
    // -- Outline generation
    private:
        virtual Source generateOutline(Task* task);
        virtual Source generateForReplicate(Task* task);
        virtual Source generateVariable(Task* task);
        virtual Source generateInitializer(Task* task);
        virtual Source generateFinalizer(Task* task);
        virtual Source generateCopyInAcquire(Task* task);
        virtual Source generateCopyOutAcquire(Task* task);
        virtual Source generateSharedAcquire(Task* task);
        virtual Source generateBody(Task* task);
        virtual Source generateControlAcquire(Task* task);
        virtual Source generateControlSharedCheck(Task* task);
        virtual Source generateControlInputPeek(Task* task);
        virtual Source generateControlOutputPeek(Task* task);
        virtual bool hasInput (Task * task);
        virtual Source generateControlInputBufferAccess(Task* task);
        virtual Source generateControlOutputBufferAccess(Task* task);
        virtual Source generateControlPop(Task* task);
        virtual Source generateControlPush(Task* task);
        virtual Source generateReplicatePeek(Task* task);
        virtual Source generateReplicateBody(Task* task);
        virtual Source generateReplicatePop(Task* task);
        virtual Source generateReplicateAcquire(Task* task);
        virtual Source generateCommitPorts(Task* task);

    // -- Replacement generation
    private:
        virtual Source generateReplacement(Task* task);
        virtual Source generateArtificialPush(Task* task);
        virtual Source generateArtificialPop(Task* task);
        
    // -- Taskgroup replacement generation support
    public:
        virtual Source generateInit(Task* task);
        virtual Source generatePorts(Task* task);
        virtual Source generateBufferPorts(Task* task);
        virtual Source generateNelemsBufferPorts(Task* task);
        virtual Source generate_task_allopen_condition(Task* task);
        virtual Source generateShareds(Task* task);
        virtual Source generateStart(Task* task);
        virtual Source generateWait(Task* task);
        
    };
    
    
} /* end namespace Acotes */ } /* end namespace TL */


#endif	/* _TL_TASKTRANSFORM_H */
