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
 * \file PACC/Socket/Address.cpp
 * \brief Class methods for the portable network address.
 * \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.15 $
 * $Date: 2005/04/19 18:19:39 $
 */

#include "Socket/Address.hpp"
#include "Socket/Exception.hpp"

#ifdef WIN32
///////////// specifics for windows /////////////
#include <winsock2.h>

#else
///////////// specifics for unixes /////////////
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#endif

using namespace std;
using namespace PACC;

/*! \brief Construct a peer address.

 Determines the IP address of a host name host using port number \c inPort and host name \c inName. Host name can be either an IP address (e.g. 198.137.240.92) or an internet name (e.g. whitehouse.gov). Any error raises a Socket::exception.
 */
Socket::Address::Address(unsigned int inPort, const string& inName) : mPortNumber(inPort)
{
#if defined(WIN32)
   WSADATA wsdata;
   if (WSAStartup(MAKEWORD(2,2), &wsdata) != 0) {
      throw Exception(eOtherError, "Address::address() failed to load WinSock2");
   }
#endif
   struct hostent* lHost;
   unsigned lIP;
   if((lIP = inet_addr(inName.c_str())) != INADDR_NONE) {
		// name is an IP address
		mIPAddress = inName;
		// get corresponding host name
      lHost = ::gethostbyaddr((char*)&lIP, sizeof(lIP), AF_INET);
      if(lHost != 0) mHostName = lHost->h_name;
		else mHostName = inName;
   } else {
		// name is a host; retrieve IP address
		mHostName = inName;
      lHost = ::gethostbyname(inName.c_str());
      if(lHost == 0) throw Exception(eOtherError, string("Address::Address() unable to lookup host name for IP ")+inName);
		mIPAddress = inet_ntoa(*((struct in_addr*)lHost->h_addr));
   }
}
