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
 * \file PACC/Socket/UDPServer.hpp
 * \brief Class definition for the portable %UDP server.
 * \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.22 $
 * $Date: 2007/01/23 21:27:47 $
 */

#ifndef PACC_Socket_UDPServer_hpp_
#define PACC_Socket_UDPServer_hpp_

#include "PACC/Socket/UDP.hpp"

namespace PACC { 
	
	namespace Socket {
		
		/*!
		\brief Portable %UDP server.
		 \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
		 \ingroup Socket
		 
		 This class defines an abstract %UDP server that waits for datagrams on a specified port. Its \c main method needs to be overloaded in order to specify the server's function. Method \c acceptDatagrams enters an infinite loop and calls \c main for each received datagram. This infinite loop may be halted by calling method \c haltServer. Any error raises a Socket::Exception.
		 */
		class UDPServer : public UDP {
		 public:
			//! Construct unconnected (%UDP) server bound to port number \c inPortNumber.
			UDPServer(unsigned int inPortNumber) : mHalt(false) {setDefaultOptions(); bind(inPortNumber);}
			//! Destroy server.
			virtual ~UDPServer(void) {}
			
			void setDefaultOptions(void);
			
			void acceptDatagrams(void);
			//! Halt server after completion of current connection
			void haltServer() throw() {mHalt = true;}
			
			/*! \brief main function of server.
				\return Wheter server should stop accepting connections.
				
				For instance, to make an echo server:
				\code
				class Echo : public UDPServer {...}
			bool Echo::main(const std::string& inDatagram, const Address& inPeer)
			{
				try
			{
				UDP lSocket;
				lSocket.sendDatagram(inDatagram, inPeer);
			}
				catch(Exception inExcep) {}
				return false; // don't halt the server
			}
			\endcode
				*/
			virtual bool main(const string& inDatagram, const Address& inPeer) = 0;
			
		 protected:
			bool mHalt; //!< stop accepting connections
			
		};
		
	} // end of Socket namespace
	
} // end of PACC namespace

#endif  // PACC_Socket_UDPServer_hpp_
