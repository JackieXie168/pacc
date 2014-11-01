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
 * \file PACC/Threading/Mutex.cpp
 * \brief Class methods for the portable mutual exclusion lock.
 * \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.23 $
 * $Date: 2007/01/23 21:27:47 $
 */

#include "PACC/Threading/Mutex.hpp"
#include "PACC/config.hpp"

#ifdef PACC_THREADS_WIN32
#include <windows.h>
typedef HANDLE pthread_mutex_t;
#else
#include <pthread.h>
#include <sys/errno.h>
#endif

using namespace PACC;

/*! \brief Allocate native structure and create mutex. 

Any error raised a Threading:Exception.
*/
Threading::Mutex::Mutex(void)
{
	pthread_mutex_t* lMutex = new pthread_mutex_t;
#ifdef PACC_THREADS_WIN32
	if((*lMutex = ::CreateMutex(0, 0, 0)) == 0)
#else
		if(::pthread_mutex_init(lMutex, 0))
#endif
			throw Exception(eOtherError, "Mutex::Mutex() can't create!");
	mMutex = lMutex;
}

//! Destroy mutex and free allocated native structure.
Threading::Mutex::~Mutex(void)
{
	pthread_mutex_t* lMutex = (pthread_mutex_t*) mMutex;
#ifdef PACC_THREADS_WIN32
	while(::CloseHandle(*lMutex) == 0)
#else
		while(::pthread_mutex_destroy(lMutex) == EBUSY)
#endif
		{
			// wait for mutex to unlock
			lock();
			unlock();
		}
			delete lMutex;
	mMutex = 0;
}

/*! \brief Lock the mutex.

A thread should never lock a mutex twice consecutively (without calling method Mutex::unlock). Any error will raise a Threading::Exception.
*/
void Threading::Mutex::lock(void) const
{
	pthread_mutex_t* lMutex = (pthread_mutex_t*) mMutex;
#ifdef PACC_THREADS_WIN32
	if(::WaitForSingleObject(*lMutex, INFINITE) != WAIT_OBJECT_0)
#else
		if(::pthread_mutex_lock(lMutex) != 0) 
#endif
			throw Exception(eWouldDeadLock, "Mutex::lock() can't lock!");
}

/*! \brief Try to lock the mutex without blocking.

Return's true if successful; false otherwise.

Any error will raise a Threading::Exception.
*/
bool Threading::Mutex::tryLock(void) const
{
	pthread_mutex_t* lMutex = (pthread_mutex_t*) mMutex;
#ifdef PACC_THREADS_WIN32
	int lValue = ::WaitForSingleObject(*lMutex, 0);
	if(lValue == WAIT_TIMEOUT) return false;
	if(lValue != WAIT_OBJECT_0)
#else
		int lValue = ::pthread_mutex_trylock(lMutex);
	if(lValue == EBUSY) return false;
	if(lValue != 0) 
#endif
		throw Exception(eOtherError, "Mutex::trylock() can't trylock!");
	return true;
}


/*! \brief Unlock the mutex.

A thread should never unlock a mutex twice consecutively (without calling method Mutex::lock). Any error will raise a Threading::Exception.
*/
void Threading::Mutex::unlock(void) const
{
	pthread_mutex_t* lMutex = (pthread_mutex_t*) mMutex;
#ifdef PACC_THREADS_WIN32
	if(::ReleaseMutex(*lMutex) == 0)
#else
		if(::pthread_mutex_unlock(lMutex) != 0)
#endif
			throw Exception(eMutexNotOwned, "Mutex::unlock() can't unlock!");
}
