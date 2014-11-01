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
 * \file PACC/Threading/Exception.cpp
 * \brief Class methods for the threading exception.
 * \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.15 $
 * $Date: 1970/05/08 18:27:21 $
 */

#include "PACC/Threading/Exception.hpp"
#include "PACC/config.hpp"

#ifdef PACC_SOCKET_WIN32
///////////// specifics for windows /////////////
#include <winsock2.h>

#else
///////////// specifics for unixes /////////////
#include <sys/errno.h>
#endif

#include <sstream>

using namespace std;
using namespace PACC;

//! Convert native OS error \c inError to Socket::Error.
Threading::Error Threading::Exception::convertNativeError(int inError)
{
	Error lCode;
	switch(inError)
	{
#ifdef PACC_THREADS_WIN32
		case ERROR_NOT_OWNER: lCode = eMutexNotOwned; break;
#else
		case EPERM: lCode = eMutexNotOwned; break;
		case EDEADLK: lCode = eWouldDeadLock; break;
#endif
		default: lCode = eOtherError;
	}
	return lCode;
}

//! Return error message.
string Threading::Exception::getMessage() const throw()
{
	ostringstream lMessage;
	switch(mCode)
	{
		case eMutexNotOwned: lMessage << "MutexNotOwned"; break;
		case eWouldDeadLock: lMessage << "WouldDeadLock"; break;
		case eRunning: lMessage << "Running"; break;
		default: lMessage << "OtherError"; break;
	}
	if(mNativeCode) lMessage << " (" << mNativeCode << "): ";
	else lMessage << ": ";
	return string("Threading: ")+lMessage.str()+what();
}
