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
* \file PACC/Threading/ThreadPool.hpp
 * \brief Class definition for the portable thread pool.
 * \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.10 $
 * $Date: 2007/01/23 21:27:47 $
 */

#ifndef PACC_Threading_ThreadPool_hpp_
#define PACC_Threading_ThreadPool_hpp_

#include "PACC/Threading/Thread.hpp"
#include "PACC/Threading/Task.hpp"
#include <queue>
#include <vector>

namespace PACC {
	
	using namespace std;
	
	namespace Threading {
		
		class ThreadPool;
		
		/*! \brief Slave thread for the portable thread pool.
		\author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
		\ingroup Threading
		
		This class defines a specialized thread for executing thread pool tasks. A slave thread simply sleeps until awakened by its parent thread pool when the later needs to execute a pending Task (see SlaveThread::main for more details). 
		*/
		class SlaveThread : public Thread {
			public:
			//! Construct a slave thread for a thread pool.
			SlaveThread(ThreadPool* inPool) : mPool(inPool) {run();}
			//! Delete slave thread; wait for thread termination.
			~SlaveThread(void) {wait(true);}
			
			protected:
			ThreadPool* mPool; //!< Pointer to parent thread pool
			
			void main(void);
		};
		
		/*! \brief Portable thread pool of slaves.
			\author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
			\ingroup Threading
			
			This class implements a thread pool of slave threads that process a FIFO queue of tasks derived from class Threading::Task. A task is simply an object with a Task::main method. Here is a simple usage example:
			\code
#include "Threading/Task.hpp"

class MyTask : public Threading::Task {
 public:
	void main(void) {
		cout << "Hello world task!" << endl;
	}
};
		
#include "Threading/ThreadPool.hpp"
		
int void main(int argc, char** argv)
{
	ThreadPool lPool(10);
	MyTask ltask;
	lPool.pushTask(lTask);
	...
	lTask.wait();
	return 0;
}
		\endcode
			*/
		class ThreadPool : public vector<SlaveThread*>, public Condition {      
			public:
			ThreadPool(unsigned int inSlaves);
			~ThreadPool(void);
			
			void push(Task& inTask);
			
			protected:
			queue<Task*> mTasks; //!< Queue of tasks.
			
			friend class SlaveThread;
		};
		
	} // end of Threading namespace
	
} // end of PACC namespace

#endif // PACC_Threading_ThreadPool_hpp_

