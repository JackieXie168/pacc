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
 * \file PACC/Threading/TLS.cpp
 * \brief Class methods for the portable Thread Local Storage (TLS).
 * \author Guillaume Mayer, ForwardSim Inc.
 * \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.4 $
 * $Date: 2008/10/29 21:54:30 $
 */

#include "PACC/Threading/TLS.hpp"
#include "PACC/Threading/Exception.hpp"
#include "PACC/Util/Assert.hpp"
#include "PACC/config.hpp"

#ifdef PACC_THREADS_WIN32
#include <windows.h>
typedef DWORD TlsIndex;

#else // Unix...
#include <pthread.h>
typedef pthread_key_t TlsIndex;
#endif

using namespace PACC;

/*! 
*/
Threading::TLS::TLS(void) : mIndex(0)
{
	TlsIndex* lIndex = new TlsIndex;
#ifdef PACC_THREADS_WIN32
	if((*lIndex = TlsAlloc()) == TLS_OUT_OF_INDEXES)
#else
	if(pthread_key_create(lIndex, NULL))
#endif
		throw Exception(eOtherError, "TLS::TLS() could not allocate local storage!");
	mIndex = lIndex;
}

/*!
*/
Threading::TLS::~TLS(void)
{
	TlsIndex* lIndex = (TlsIndex*) mIndex;
#ifdef PACC_THREADS_WIN32
	if(TlsFree(*lIndex))
#else
	if(pthread_key_delete(*lIndex))
#endif
		throw Exception(eOtherError, "TLS::~TLS() could not release local storage!");
	delete lIndex;
}

/*! 
*/
void Threading::TLS::setValue(void *inObject)
{
	TlsIndex* lIndex = (TlsIndex*) mIndex;
#ifdef PACC_THREADS_WIN32
	if(!TlsSetValue(*lIndex, inObject))
#else
	if(pthread_setspecific(*lIndex, inObject))
#endif
		throw Exception(eOtherError, "TLS::setValue() could not set value!");
}

/*!
*/
void* Threading::TLS::getValue(void)
{
	TlsIndex* lIndex = (TlsIndex*) mIndex;
	return 
#ifdef PACC_THREADS_WIN32
		TlsGetValue(*lIndex)
#else
		pthread_getspecific(*lIndex)
#endif
		;
}
