/*
 *  Portable Agile C++ Classes (PACC)
 *  Copyright (C) 2001-2003 by Marc Parizeau
 *  http://manitou.gel.ulaval.ca/~parizeau/PACC
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *  Contact:
 *  Laboratoire de Vision et Systemes Numeriques
 *  Departement de genie electrique et de genie informatique
 *  Universite Laval, Quebec, Canada, G1K 7P4
 *  http://vision.gel.ulaval.ca
 *
 */

/*!
 * \file PACC/Threading/Task.hpp
 * \brief Class definition for the abstract task of the portable thread pool.
 * \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.8 $
 * $Date: 2007/01/23 21:27:47 $
 */

#ifndef PACC_Threading_Task_hpp_
#define PACC_Threading_Task_hpp_

#include "PACC/Threading/Condition.hpp"

namespace PACC {
	
	namespace Threading {
		
		/*! \brief %Task for thread pool execution.
		\author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
		\ingroup Threading
		
		This abstract class encapsulates a primitive task that can be executed by a ThreadPool. It must be subclassed in order to implement virtual method Task::main which defines the task's function. Once pushed onto a thread pool using method ThreadPool::push, the task will start executing as soon as a thread becomes available. A task cannot be canceled after it has started to run.
		*/
		class Task : public Condition {
			public: 
			//! Construct default task: initialize to not running and not completed.
			Task(void) : mRunning(false), mCompleted(false) {}
			//! Delete task: wait for task completion.
			virtual ~Task(void) {lock(); while(!mCompleted) wait(); unlock();}
			
			/*! \brief Check wheter task is completed.
			
			The embedded mutex should always be locked (using method Task::lock) prior to calling this method.
			*/
			bool isCompleted(void) const {return mCompleted;}
			
			/*! \brief Check wheter task is running.
			
			The embedded mutex should always be locked (using method Task::lock) prior to calling this method.
			*/
			bool isRunning(void) const {return mRunning;}
			
			/*! \brief Implements main procedure of task.
			
			This virtual method must be overloaded in a derived class in order to implement the main procedure of this task.
			*/
			virtual void main(void) = 0;
			
			//! Reset internal task state to default (not running and not completed).
			void reset(void) {lock(); mRunning = mCompleted = false; unlock();}
			
			/*! \brief Wait for task to complete.
				
				If argument \c inLock is true (default), this method locks the embedded mutex prior to waiting on the condition, and unlocks the mutex prior to return. Otherwise, it assumes that the mutex is already locked and does not modify its state. This method should never be called with argument \c inLock=false without first locking the mutex using method Task::lock.
			*/
			void wait(bool inLock=true) const {
				if(inLock) lock();
				while(!mCompleted) Condition::wait();
				if(inLock) unlock();
			}
			
			protected:
			bool mRunning; //!< is running flag
			bool mCompleted; //!< is completed flag
			
			friend class SlaveThread;
		};
		
	} // end of Threading namespace 
	
} // end of PACC namespace

#endif // PACC_Threading_Task_hpp_
