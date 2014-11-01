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
 * \file PACC/Socket/ConnectedUDP.cpp
 * \brief Class methods for the portable connected %UDP client.
 * \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.18 $
 * $Date: 2007/01/23 21:27:46 $
 */

#include "PACC/Socket/ConnectedUDP.hpp"

using namespace std;
using namespace PACC;

/*! \brief Receive string datagram from connected (UDP) server.

This function waits for a datagram from the connected peer socket, or until time out. It returns the received datagram through output parameter \c outDatagram. Any error raises a Socket::Exception. For instance, it throws an exception with code Socket::eTimeOut if the timeout period expires before reception of any datagram. The timeout period can be changed using function Port::setSockOpt with parameter Socket::eRecvTimeOut.
*/
void Socket::ConnectedUDP::receiveDatagram(string& outDatagram)
{
	outDatagram.resize((unsigned int) getSockOpt(eRecvBufSize));
	unsigned int lRecv = receive(&outDatagram[0], outDatagram.size());
	outDatagram.resize(lRecv);
}

/*! \brief Send datagram message to connected (UDP) server.
\attention Maximum datagram size defaults to 1024 bytes.

This function sends datagram string \c inDatagram to the connected peer socket. Any error raises a Socket::Exception. For instance, it throws an exception with code \c Socket::eTimeOut if the datagram cannot be sent before the time out period expires. The time out period can be changed using function \c Port::setSockOpt with parameter \c Socket::eSendTimeOut.
*/
void Socket::ConnectedUDP::sendDatagram(const string& inDatagram)
{
	send(inDatagram.data(), inDatagram.size());
}
