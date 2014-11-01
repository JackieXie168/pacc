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
 * \file PACC/Socket/TCP.hpp
 * \brief Class definition for the portable %TCP client.
 * \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.25 $
 * $Date: 2007/01/23 21:27:46 $
 */

#ifndef PACC_Socket_TCP_hpp_
#define PACC_Socket_TCP_hpp_

#include "PACC/Socket/Port.hpp"

namespace PACC { 
	
	namespace Socket {
		
		/*!
		\brief Portable %TCP socket client
		 \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
		 \ingroup Socket
		 
		 This class defines a simple %TCP socket client that connects to a peer server. Any error raises a Socket::Exception.
		 */
		class TCP : public Port {
		 public:
			//! Construct unconnected socket.
			explicit TCP(void) : Port(eTCP) {}
			//! Construct using existing socket descriptor \c inDescriptor.
			explicit TCP(int inDescriptor) throw() : Port(inDescriptor) {}
			//! Construct socket connected to peer \c inPeer.
			explicit TCP(const Address& inPeer) : Port(eTCP) {Port::connect(inPeer);}
			
			//! Close connection.
			void close(void) {Port::close();}
			//! Connect to server \c inPeer.
			void connect(const Address& inPeer) {close(); open(eTCP); Port::connect(inPeer);}
			
			void receiveMessage(string& outMessage);
			void sendMessage(const string& inMessage);
			
		};
		
	} // end of Socket namespace
	
} // end of PACC namespace

#endif  // PACC_Socket_TCP_hpp_
