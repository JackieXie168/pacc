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
 * \file PACC/Socket/Address.hpp
 * \brief Class definition for the portable network address.
 * \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.22 $
 * $Date: 2006/12/07 02:58:14 $
 */

#ifndef PACC_Socket_Address_hpp_
#define PACC_Socket_Address_hpp_

#include <string>

namespace PACC { 
	
	using namespace std;
	
	namespace Socket {
		
/*! \brief Portable network address.
	\author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
	\ingroup Socket
		
This class encapsulates a cross-platform address for a peer host. It should be compatible with any POSIX Unix, and with any version of windows. It has been tested under Linux, MacOS X and Windows 2000/XP.

Here is an example of a short program that reads either a hostname or an IP address on the command line, and outputs both the host name and IP address to the console:
\code
#include "Socket.hpp"
#include <iostream>
		
using namespace std;
using namespace PACC;
		
int main(int argc, char** argv) 
{
	if (argc!=2) {
		cout << "Please specify either a hostname or IP adress" << endl;
		exit(-1);
	}
	try {
		Socket::Address lAddress(0, argv[1]);
		cout << "Hostname:   " << lAddress.getHostName() << endl;
		cout << "IP address: " << lAddress.getIPAddress() << endl;
	} 
	catch (const Socket::Exception& inException) {
		cout << inException.getMessage() << endl;
	}
}
\endcode		
		
Any error raises a Socket::Exception. 
*/
		class Address {
		 public:
			//! Construct a peer address for "host:port" \c inHostPort.
			Address(const string& inHostPort);
			
			//! Construct a peer address for host \c inHost and port \c inPort.
			explicit Address(unsigned int inPort=0, const string& inHost="localhost") : mPortNumber(inPort) {lookupHost(inHost);}
			
			//! Return host name.
			const string& getHostName() const {return mHostName;}
			//! Return IP address.
			const string& getIPAddress() const {return mIPAddress;}
			//! Return port number.
			unsigned int getPortNumber() const {return mPortNumber;}
			
		 protected:
			unsigned int mPortNumber; //!< socket port number
			string mIPAddress; //!< socket IP address
			string mHostName; //!< host name
			
			//! Lookup host name/address \c inHost.
			void lookupHost(const string& inHost);
		};
		
	} // end of Socket namespace
	
} // end of PACC namespace

#endif  // PACC_Socket_Address_hpp_
