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
 * \file PACC/Threading/Thread.cpp
 * \brief Class methods for the portable thread.
 * \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.46 $
 * $Date: 2008/04/17 21:09:32 $
 */

#include "PACC/Threading/Thread.hpp"
#include "PACC/Util/Assert.hpp"
#include "PACC/config.hpp"
#include <cstdlib>

#ifdef PACC_THREADS_WIN32
#include <windows.h>
#define ErrNo WSAGetLastError() // descriptor of last error
struct ThreadStruct {
	HANDLE mHandle;
	DWORD mId;
};

#else // Unix...
#include <pthread.h>
#include <unistd.h>
#include <sys/errno.h>
#define ErrNo errno // descriptor of last error
typedef pthread_t ThreadStruct;
#endif

using namespace PACC;

/*! \brief Create thread.

Note that method Thread::run must be called in order to start the execution of Thread::main.
*/
Threading::Thread::Thread(void) : mThread(0), mCancel(false), mRunning(false) {}

/*! \brief Delete thread.

Assuming that the caller has canceled the thread and waited for its termination, this method detaches the thread and deletes its native structure. In debug mode, if the thread is found still running, then the method aborts the application with a descriptive fatal error message.

A good strategy is to always define a destructor in the derived class, and to make this destructor wait for thread termination. For instance:
\code
class MyThread : public Threading::Thread {
    ...
    ~MyThread(void) {wait();}
    ...
};
\endcode

The calling thread can also use method Thread::cancel to request early termination for this thread. But thread cancellation is asynchronous, it will be deferred to the next cancellation point. From within Thread::main, in order to allow rapid thread cancellation, method Thread::makeCancellationPoint should be called as often as possible whenever it is safe to cancel the thread. A running thread that omits cancellation points and never terminates will block the calling thread of this method indefinitely.

\attention If the destructor in the derived thread class (e.g. MyThread above) does not wait for thread termination, the potential hazardous situation is that the runtime system will have deleted all of its members before calling this destructor (in C++, class destructors are called in reversed sequence). Thus, the still running thread could access deleted data members with unpredictable and unexpected results. So beware!
*/
Threading::Thread::~Thread(void)
{
	lock();
	if(mThread) {
		// make sure that thread is no longer running
		PACC_AssertM(!mRunning, "Destructor called without first cancelling the thread and waiting for its termination. Please correct the situation because it is potentially very hazardous!");
		// detach thread
		ThreadStruct* lThread = (ThreadStruct*) mThread;
#ifdef PACC_THREADS_WIN32
		if(::WaitForSingleObject(lThread->mHandle, INFINITE) == WAIT_FAILED)
#else // Unix...
		if(::pthread_join(*lThread, 0) != 0)
#endif
		{
			throw Exception(ErrNo, "Thread::~Thread() unable to join thread");
		}
		// delete native structure
		delete lThread;
		mThread = 0;
	}
	unlock();
}

/*! \brief Cancel execution of thread.

The cancellation process requested by this function runs asynchronously with the calling thread. The latter can call function Thread::wait in order to wait for thread termination. The cancelled thread will not terminate before it reaches its next cancellation point, or before it terminates by itself. Before deleting a cancelled thread, the caller should always wait for thread termination. 

The caller should always lock the thread (using the lock method) prior to asking for cancellation. If it wishes to wait immediately for thread termination, it should keep the thread locked and call the wait method with argument false (see Thread::wait). 
*/
void Threading::Thread::cancel(void)
{
	mCancel = true;
}

/*! \brief Check wheter or not the thread is currently running.

The caller should always lock the thread (using the lock method) prior to checking the running flag.
*/
bool Threading::Thread::isRunning(void) const
{
	return mRunning;
}

//! Return whether the calling thread is the same as this thread.
bool Threading::Thread::isSelf(void) const
{
	ThreadStruct* lThread = (ThreadStruct*) mThread;
#ifdef PACC_THREADS_WIN32
	return lThread->mId == ::GetCurrentThreadId();
#else // Unix...
#ifdef pthread_equal
	// On AIX, pthread_equal is defined as a macro.
	return pthread_equal(*lThread, ::pthread_self());
#else // pthread_equal
	return ::pthread_equal(*lThread, ::pthread_self());
#endif // pthread_equal
#endif
}

/*! \brief Create cancellation point.

This function should be called by Thread::main as often as possible in order to enable thread cancellation. A Thread::cancel request can only be honored at a future cancellation point.
*/
void Threading::Thread::makeCancellationPoint(void)
{
	lock();
	// exit thread if cancellation was requested
	if(mCancel) {
		// signal all that thread has terminated
		mRunning = false;
		broadcast();
		unlock();
		// now exit thread
#ifdef PACC_THREADS_WIN32
		::ExitThread(0);
#else // Unix...
		::pthread_exit(0);
#endif
	}
	unlock();
}

/*! \brief Create and startup thread.

This function allocates memory for the native thread structure, and then creates the thread which starts up asynchronously. The calling thread will block until this thread as actually started to run. 

Any error will raise a Threading:Exception. In particular, this method should not be called if the thread is already running.
*/
void Threading::Thread::run(void)
{
	lock();
	// check that thread is not already running
	if(mRunning) {
		unlock();
		throw Exception(eRunning, "Thread::run() thread is already running!");
	}
	mCancel = false;
	// allocate native structure
	if(!mThread) mThread = new ThreadStruct;
	ThreadStruct* lThread = (ThreadStruct*) mThread;
	// create thread
#ifdef PACC_THREADS_WIN32
	if((lThread->mHandle = ::CreateThread(0, 0, (LPTHREAD_START_ROUTINE)startup, this, 0, &lThread->mId)) == 0)
#else // Unix
	if(::pthread_create(lThread, 0, startup, this) != 0)  
#endif
	{
		unlock();
		throw Exception(eOtherError, "Thread::run() can't create thread!");
	}
	// wait for thread to start up
	if(!mRunning) Condition::wait();
	unlock();
}

/*! \brief Sleep calling thread for \c inSeconds seconds. 

A negative value will throw a Threading::Exception.
*/
void Threading::Thread::sleep(double inSeconds)
{
	if(inSeconds < 0) throw Exception(eOtherError, "Thread::sleep() invalid time");
#ifdef PACC_THREADS_WIN32
	// argument must be in milliseconds for windows
	::Sleep((DWORD)(1000*inSeconds));
#else // Unix...
	if(inSeconds < 1000) {
		// use microsecond sleep
		if(::usleep((unsigned int)(1000000*inSeconds)) != 0) {
			throw Exception(eOtherError, "Thread::sleep() can't sleep");
		}
	} else {
		// use regular sleep
		if(::sleep((unsigned int)(inSeconds)) != 0) {
			throw Exception(eOtherError, "Thread::sleep() can't sleep");
		}
	}
#endif
}

/*! \brief Startup thread.

This function is first called when the thread starts up and immediately calls function Thread::main. After main returns (or is cancelled), it cleans up by signalling any waiting thread, and then terminates.
*/
void* Threading::Thread::startup(void* inThread)
{
	Thread* lThread = (Thread*) inThread;
	// signal parent thread that this thread is starting to run
	lThread->lock();
	lThread->mRunning = true;
	lThread->signal();
	lThread->unlock();
	// execute main
	lThread->main();
	// signal all that thread as terminated
	lThread->lock();
	lThread->mRunning = false;
	lThread->broadcast();
	lThread->unlock();
	return 0;
}

/*! \brief %Wait for thread termination.

This method will wait for thread termination. If argument \c inLock is true (default), it will first lock the thread mutex before checking wheter or not the thread is still running. If the thread has already terminated, it will promptly return after unlocking the mutex. Otherwise, it will wait for the thread to terminate by itself. 

If argument \c inLock is false, this method assumes that the caller has already locked the mutex (using Thread::lock) and is responsible for unlocking it.
*/
void Threading::Thread::wait(bool inLock)
{
	if(inLock) lock();
	if(mRunning) Condition::wait();
	if(inLock) unlock();
}
