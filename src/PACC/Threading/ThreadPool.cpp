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
 * \file PACC/Threading/ThreadPool.cpp
 * \brief Class methods for the portable thread pool.
 * \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.8 $
 * $Date: 2007/01/23 21:27:47 $
 */

#include "PACC/Threading/ThreadPool.hpp"
#include "PACC/config.hpp"

using namespace std;
using namespace PACC;

/*! \brief Execute a pending task.

When awakened by its parent thread pool, this method removes the next task from the head of the queue and starts executing it immediately. It also broadcasts a signal to all waiting threads for this task, both prior to task execution and after task completion.
*/
void Threading::SlaveThread::main(void) 
{
	while(!mCancel)
	{
		// wait for available task
		mPool->lock();
		while(mPool->mTasks.empty() && !mCancel) mPool->wait();
		if(!mCancel)
		{
			// dequeu next task
			Task* lTask = mPool->mTasks.front();
			mPool->mTasks.pop();
			mPool->unlock();
			// signal all that task is running
			lTask->lock();
			lTask->mRunning = true;
			lTask->broadcast();
			lTask->unlock();
			// run task
			lTask->main();
			// signal all that task is completed
			lTask->lock();
			lTask->mRunning = false;
			lTask->mCompleted = true;
			lTask->broadcast();
			lTask->unlock();
		}
		else mPool->unlock();
	}
}

//! Construct thread pool by allocating \c inSlaves threads.
Threading::ThreadPool::ThreadPool(unsigned int inSlaves)
{
	// allocate slave threads
	for(unsigned int i = 0; i < inSlaves; ++i) 
	{
		SlaveThread* lThread = new SlaveThread(this);
		push_back(lThread);
	}
}

//! Delete thread pool.
Threading::ThreadPool::~ThreadPool(void)
{
	lock();
	// wait for queue to become empty
	while(!mTasks.empty()) 
	{
		// wait for last task in queue to at least start running
		Task* lTask = mTasks.back();
		lTask->lock();
		unlock();
		lTask->wait(false);
		lTask->unlock();
		lock();
	}
	// now cancel all threads
	for(unsigned int i = 0; i < size(); ++i) (*this)[i]->cancel();
	// signal them to wake up
	broadcast();
	unlock();
	// then delete them (the thread destructor will wait for thread completion)
	for(unsigned int i = 0; i < size(); ++i) delete (*this)[i];
}

/*! \brief Push task \c inTask onto the thread pool queue.

The thread pool maintains a queue of task references that will be executed in FIFO order.
*/
void Threading::ThreadPool::push(Task& inTask)
{
	// reset task flags
	inTask.reset();
	// push task onto queue and signal availability
	lock();
	mTasks.push(&inTask);
	signal();
	unlock();
}
