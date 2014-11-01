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
 * \file PACC/Socket/Exception.cpp
 * \brief Class methods for the portable socket exception.
 * \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.14 $
 * $Date: 1970/05/08 18:26:05 $
 */

#include "PACC/Socket/Exception.hpp"
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
Socket::Error Socket::Exception::convertNativeError(int inError)
{
	Error lCode;
	switch(inError)
	{
#ifdef PACC_SOCKET_WIN32
		case WSAEACCES: lCode = ePrivilegedPort; break;
		case WSAEADDRINUSE: lCode = eAddressInUse; break;
		case WSAEADDRNOTAVAIL: lCode = eAddressNotAvailable; break;
		case WSAECONNREFUSED: lCode = eConnectionRefused; break;
		case WSAECONNRESET: lCode = eConnectionClosed; break;
		case WSAEHOSTDOWN: lCode = eConnectionRefused; break;
		case WSAEHOSTUNREACH: lCode = eConnectionRefused; break;
		case WSAEINTR: lCode = eConnectionClosed; break;
		case WSAEISCONN: lCode = eIsConnected; break;
		case WSAEMSGSIZE: lCode = eDatagramTooLong; break;
		case WSAENETRESET: lCode = eConnectionClosed; break;
		case WSAENOPROTOOPT: lCode = eInvalidOption; break;
		case WSAENOTCONN: lCode = eNotConnected; break;
		case WSANOTINITIALISED: lCode = eBadDescriptor; break;
		case WSAENOTSOCK: lCode = eBadDescriptor; break;
		case WSAEOPNOTSUPP: lCode = eOpNotSupported; break;
		case WSAESHUTDOWN: lCode = eBadDescriptor; break;
		case WSAETIMEDOUT: lCode = eTimeOut; break;
#else
		case EACCES: lCode = ePrivilegedPort; break;
		case EADDRINUSE: lCode = eAddressInUse; break;
		case EADDRNOTAVAIL: lCode = eAddressNotAvailable; break;
		case EAGAIN: lCode = eTimeOut; break;
		case EBADF: lCode = eBadDescriptor; break;
		case ECONNREFUSED: lCode = eConnectionRefused; break;
		case ECONNRESET: lCode = eConnectionClosed; break;
		case EINTR: lCode = eConnectionClosed; break;
		case EISCONN: lCode = eIsConnected; break;
		case EMSGSIZE: lCode = eDatagramTooLong; break;
		case ENOPROTOOPT: lCode = eInvalidOption; break;
		case ENOTCONN: lCode = eNotConnected; break;
		case ENOTSOCK: lCode = eBadDescriptor; break;
		case EPIPE: lCode =eConnectionClosed; break;
		case ETIMEDOUT: lCode = eTimeOut; break;
#endif
		default: lCode = eOtherError;
	}
	return lCode;
}

string Socket::Exception::getMessage() const throw()
{
	ostringstream lMessage;
	switch(mCode)
	{
		case eAddressInUse: lMessage << "address in use"; break;
		case eAddressNotAvailable: lMessage << "address not available"; break;
		case eBadDescriptor: lMessage << "bad descriptor"; break;
		case eBadMessage: lMessage << "bad message"; break;
		case eConnectionClosed: lMessage << "connection closed"; break;
		case eConnectionRefused: lMessage << "connection refused"; break;
		case eDatagramTooLong: lMessage << "datagram too long"; break;
		case eInvalidOption: lMessage << "invalid option"; break;
		case eIsConnected: lMessage << "is connected"; break;
		case eNotConnected: lMessage << "not connected"; break;
		case eOpNotSupported: lMessage << "operation not supported"; break;
		case ePrivilegedPort: lMessage << "privileged port"; break;
		case eTimeOut: lMessage << "time out"; break;
		default: lMessage << "other error"; break;
	}
	if(mNativeCode) lMessage << " (" << mNativeCode << ")";
	return string("Socket ")+lMessage.str()+":\n"+what();
}
