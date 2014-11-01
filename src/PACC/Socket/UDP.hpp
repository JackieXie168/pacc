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
 * \file PACC/Socket/UDP.hpp
 * \brief Class definition for the portable %UDP client.
 * \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.21 $
 * $Date: 2007/01/23 21:27:47 $
 */

#ifndef PACC_Socket_UDP_hpp_
#define PACC_Socket_UDP_hpp_

#include "PACC/Socket/Port.hpp"

namespace PACC { 
	
	namespace Socket {
		
		/*!
		\brief Portable %UDP client
		 \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
		 \ingroup Socket
		 
		 This class defines a simple %UDP socket client. Any error raises a Socket::Exception.
		 */
		class UDP : public Port {
		 public:
			//! Construct unconnected socket (see UDP::setDefaultOptions for default socket options).
			explicit UDP(void) : Port(eUDP) {}
			//! Construct using existing socket descriptor \c indescriptor.
			explicit UDP(int inDescriptor) throw() : Port(inDescriptor) {}

			void receiveDatagram(string& outDatagram, Address& outPeer);
			void sendDatagram(const string& inDatagram, const Address& inPeer);

		};
		
	} // end of Socket namespace
	
} // end of PACC namespace

#endif  // Socket_UDP_hpp_
