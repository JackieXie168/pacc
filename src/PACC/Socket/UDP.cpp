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
 * \file PACC/Socket/UDP.cpp
 * \brief Class methods for the portable %UDP client.
 * \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.18 $
 * $Date: 2007/01/23 21:27:47 $
 */

#include "PACC/Socket/UDP.hpp"

using namespace std;
using namespace PACC;

/*! \brief Receive string datagram from unconnected server.

This function waits for a datagram, or until time out. It returns the received datagram through output parameter \c outDatagram. It also returns the peer address through parameter \c outPeer. Any error raises a Socket::Exception. For instance, it throws an exception with code Socket::eTimeOut if the timeout period expires before reception of any datagram. The timeout period can be changed using function Port::setSockOpt with parameter Socket::eRecvTimeOut.
*/
void Socket::UDP::receiveDatagram(string& outDatagram, Socket::Address& outPeer)
{
	// reserve adequate buffer space (if needed)
	unsigned int lRecvBufSize = (unsigned int) getSockOpt(eRecvBufSize);
	if(outDatagram.size() < lRecvBufSize) outDatagram.resize(lRecvBufSize);
	// receive message
	unsigned int lRecv = receiveFrom(&outDatagram[0], outDatagram.size(), outPeer);
	// resize output string
	outDatagram.resize(lRecv);
}

/*! \brief Send datagram message to unconnected server.
\attention Maximum datagram size defaults to 1024 bytes.

This function sends datagram string \c inDatagram to peer socket at address \c inPeer. Any error raises a Socket::Exception. For instance, it throws an exception with code Socket::eTimeOut if the datagram cannot be sent before the time out period expires. The time out period can be changed using function Port::setSockOpt with parameter Socket::eSendTimeOut.
*/
void Socket::UDP::sendDatagram(const string& inDatagram, const Socket::Address& inPeer)
{
	sendTo(inDatagram.data(), inDatagram.size(), inPeer);
}
