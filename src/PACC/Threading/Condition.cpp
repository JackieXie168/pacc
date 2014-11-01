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
 * \file PACC/Threading/Condition.cpp
 * \brief Class methods for the portable condition.
 * \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.27 $
 * $Date: 2007/01/23 21:27:47 $
 */

#include "PACC/Threading/Condition.hpp"
#include "PACC/config.hpp"

#ifdef PACC_THREADS_WIN32
#define _WIN32_WINNT 0x0400 // for SignalObjectAndWait
#include <windows.h>
typedef struct {
	// This windows implementation uses the SignalObjectAndWait solution from D.C. 
	// Schmidt and Irfan Pyarali. See http://www.cs.wustl.edu/~schmidt/win32-cv-1.html
	int mWaiters;
	bool mBroadcast;
	// Critical section to protect variables
	CRITICAL_SECTION mLock;
	// Semaphore is used to queue up waiting threads
	HANDLE mSemaphore;
	// Auto-reset event to signal last waiting thread
	HANDLE mDone;
} pthread_cond_t;
#else
#include <pthread.h>
#include <sys/errno.h>
#include <sys/time.h>
#include <math.h>
#endif

using namespace PACC;

/*! \brief Allocate native structure and create condition. 

Any error raises a Threading::Exception.
*/
Threading::Condition::Condition(void)
{
	pthread_cond_t* lCondition = new pthread_cond_t;
#ifdef PACC_THREADS_WIN32
	lCondition->mWaiters = 0;
	lCondition->mBroadcast = false;
	::InitializeCriticalSection(&lCondition->mLock);
	lCondition->mSemaphore = ::CreateSemaphore(0, 0, 0x7fffffff, 0);
	lCondition->mDone = ::CreateEvent(0, false, false, 0);
	if(lCondition->mSemaphore == 0 || lCondition->mDone == 0)
#else
		if(::pthread_cond_init(lCondition, 0) != 0)
#endif
			throw Exception(eOtherError, "Threading::Condition() can't create!");
	mCondition = lCondition;
}

//! Destroy condition and free allocated native structure.
Threading::Condition::~Condition(void)
{
	pthread_cond_t* lCondition = (pthread_cond_t*) mCondition;
#ifdef PACC_THREADS_WIN32
	::DeleteCriticalSection(&lCondition->mLock);
	while(::CloseHandle(lCondition->mSemaphore) == 0) broadcast();
	while(::CloseHandle(lCondition->mDone) == 0) broadcast();
#else
	while(::pthread_cond_destroy(lCondition) == EBUSY) broadcast();
#endif
	delete lCondition;
	mCondition = 0;
}

/*! \brief Broadcast a wake up signal to all waiting threads.

This method assumes that the embedded mutex has already been LOCKED by the calling thread (using method Condition::lock). It allows the caller to execute some thread critical operations like setting an external condition prior to broadcasting. If no thread is waiting, the signal is simply ignored. Here is an example of typical usage:
\code
lCondition.lock();
// set some external condition
...
lCondition.broadcast();
lCondition.unlock();
\endcode
WARNING: Calling this method without first locking the mutex may produce unexpected OS dependant results. And don't forget to unlock the mutex afterwards!

Any error raises a Threading::Exception.
*/
void Threading::Condition::broadcast(void) const
{
	pthread_cond_t* lCondition = (pthread_cond_t*) mCondition;
#ifdef PACC_THREADS_WIN32
	EnterCriticalSection(&lCondition->mLock);
	if(lCondition->mWaiters > 0) {
		lCondition->mBroadcast = true;
		// wake up the waiting threads atomically
		if(!::ReleaseSemaphore(lCondition->mSemaphore, lCondition->mWaiters, 0)) {
			unlock();
			throw Exception(eOtherError, "Condition::broadcast() unable to release semaphore!");
		}
		// wait for threads to acquire the counting semaphore
		LeaveCriticalSection(&lCondition->mLock);
		if(::WaitForSingleObject(lCondition->mDone, INFINITE) != WAIT_OBJECT_0) {
			unlock();
			throw Exception(eOtherError, "Condition::broadcast() invalid event!");
		}
		lCondition->mBroadcast = false;
	}
	else LeaveCriticalSection(&lCondition->mLock);
#else
	if(::pthread_cond_broadcast(lCondition) != 0) {
		unlock();
		throw Exception(eOtherError, "Condition::broadcast() invalid condition!");
	}
#endif
}

/*! \brief Signal a single waiting thread to wake up (which waiting thread wakes up is undetermined).

This method assumes that the embedded mutex has already been locked by the calling thread (using method Condition::lock). It allows the caller to execute some thread critical operations, like setting or resetting external conditions. If no thread is waiting, the signal is simply ignored. Here is an example of typical usage:
\code
lCondition.lock();
// set some external condition
...
lCondition.signal();
lCondition.unlock();
\endcode
WARNING: Calling this method without first locking the mutex may produce unexpected OS dependant results. And don't forget to unlock the mutex afterwards!

Any error raises a Threading::Exception.
*/
void Threading::Condition::signal(void) const
{
	pthread_cond_t* lCondition = (pthread_cond_t*) mCondition;
#ifdef PACC_THREADS_WIN32
	EnterCriticalSection(&lCondition->mLock);
	int lWaiters = lCondition->mWaiters;
	LeaveCriticalSection(&lCondition->mLock);
	// wake up a single waiting thread if any waiters
	if(lWaiters > 0 && ::ReleaseSemaphore(lCondition->mSemaphore, 1, 0) == 0)
#else
		if(::pthread_cond_signal(lCondition) != 0)
#endif
		{
			unlock();
			throw Exception(eOtherError, "Condition::signal() invalid condition!");
		}
}

/*! \brief Wait up to \c inMaxTime seconds for the condition to be signaled (or broadcasted). 
\return True if condition was signaled (or broadcasted), false if timed out.

This method assumes that the embedded mutex has already been locked by the calling thread (using method Condition::lock), and will also return with the mutex locked. A negative or null time out (default) means that the method should wait indefinitely.

Here is an example of typical usage:
\code
...
lCondition.lock();
if(external_condition == false) {
    // wait indefinitely...
    lCondition.wait();
    // reset external condition
    ...
}
lCondition.unlock();
...
\endcode
WARNING: Calling this method without first locking the mutex may produce unexpected OS dependant results. And don't forget to unlock the mutex afterwards!

Any error raises a Threading::Exception.
*/
bool Threading::Condition::wait(double inMaxTime) const
{
	bool lReturn;
	pthread_cond_t* lCondition = (pthread_cond_t*) mCondition;
#ifdef PACC_THREADS_WIN32
	EnterCriticalSection(&lCondition->mLock);
	// increment number of waiters
	lCondition->mWaiters += 1;
	LeaveCriticalSection(&lCondition->mLock);
	// wait for the semaphore after atomically unlocking the mutex
	HANDLE* lMutex = (HANDLE*) mMutex;
	DWORD lRes = ::SignalObjectAndWait(*lMutex, lCondition->mSemaphore, (inMaxTime <= 0 ? INFINITE : (DWORD)(inMaxTime*1000)), false);
	if((lReturn = (lRes != WAIT_TIMEOUT)) && lRes != WAIT_OBJECT_0) {
		unlock();
		throw Exception(eOtherError, "Condition::wait() invalid semaphore!");
	}   
	EnterCriticalSection(&lCondition->mLock);
	// we're no longer waiting, decrement number of waiters
	lCondition->mWaiters -= 1;
	bool lLastWaiter = lCondition->mBroadcast && lCondition->mWaiters == 0;
	LeaveCriticalSection(&lCondition->mLock);
	if(lLastWaiter) {
		// wait for the mutex after atomically signaling the broadcaster that all threads have awakened
		if(::SignalObjectAndWait(lCondition->mDone, *lMutex, INFINITE, false) != WAIT_OBJECT_0)
		{
			unlock();
			throw Exception(eOtherError, "Condition::wait() invalid mutex!");
		}
	}
	else if(::WaitForSingleObject(*lMutex, INFINITE) != WAIT_OBJECT_0) {
		unlock();
		throw Exception(eOtherError, "Condition::wait() invalid mutex!");
	}
#else
	int lRes;
	// pthread_cond_wait atomically unlocks the mutex, waits on the condition, and locks the mutex again
	if(inMaxTime <= 0) lRes = ::pthread_cond_wait(lCondition, (pthread_mutex_t*)mMutex);
	else {
		// get time of day and add specified time out
		struct timeval lVal;
		::gettimeofday(&lVal, 0);
		struct timespec lSpec;
		lSpec.tv_sec = lVal.tv_sec + (long) inMaxTime;
		lSpec.tv_nsec = lVal.tv_usec*1000 + (long) ((inMaxTime - floor(inMaxTime)) * 1000000000);
		// check that the number of nanoseconds is less than 1 sec
		if(lSpec.tv_nsec >= 1000000000)
		{
			// otherwise adjust...
			lSpec.tv_nsec -= 1000000000;
			lSpec.tv_sec += 1;
		}
		// pthread_cond_timedwait atomically unlocks the mutex, waits on the condition, and locks the mutex again
		lRes = ::pthread_cond_timedwait(lCondition, (pthread_mutex_t*)mMutex, &lSpec);
	}
	if((lReturn = (lRes != ETIMEDOUT)) && lRes != 0)
	{
		unlock();
		throw Exception(eOtherError, "Condition::wait() invalid condition!");
	}
#endif
	return lReturn;
}
