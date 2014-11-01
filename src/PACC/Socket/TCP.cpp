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
 * \file PACC/Socket/TCP.cpp
 * \brief Class methods for the portable %TCP client.
 * \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.22 $
 * $Date: 2007/01/23 21:27:46 $
 */

#include "PACC/Socket/TCP.hpp"

using namespace std;
using namespace PACC;

/*! \brief Receive string message from connected (TCP) server.

This function waits for a message from the connected peer socket, or until time out. It returns the received message through output parameter \c OutMessage. Any error raises a Socket::Exception. For instance, it throws an exception with code Socket::eConnectionClosed if the connection is closed by the other party before message end, or with code Socket::eTimeOut if the timeout period expires before reception of any message. The timeout period can be changed using function Port::setSockOpt with parameter Socket::eRecvTimeOut.
*/
void Socket::TCP::receiveMessage(string& outMessage)
{
	// reserve adequate buffer space (if needed)
	unsigned int lRecvBufSize = (unsigned int) getSockOpt(eRecvBufSize);
	if(outMessage.size() < lRecvBufSize) outMessage.resize(lRecvBufSize);
	// receive message
	unsigned int lRecv = receive(&outMessage[0], outMessage.size());
	// resize output string
	outMessage.resize(lRecv);
}

/*! \brief Send string message to connected (TCP) server.

This function sends message string \c inMessage to the connected peer socket. Any error raises a Socket::Exception. For instance, it throws an exception with code Socket::eConnectionClosed if the connection is closed by the other party during message transmission, or with code Socket::eTimeOut if the message cannot be sent before the time out period expires. The time out period can be changed using function Port::setSockOpt with parameter Socket::eSendTimeOut.
*/
void Socket::TCP::sendMessage(const string& inMessage)
{
	send(inMessage.data(), inMessage.size());
}
