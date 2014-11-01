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
 * \file PACC/Socket/UDPServer.cpp
 * \brief Class methods for the portable %UDP server.
 * \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.15 $
 * $Date: 2007/01/23 21:27:47 $
 */

#include "PACC/Socket/UDPServer.hpp"
#include <iostream>

using namespace std;
using namespace PACC;

//! Process incomming connections by receiving datagrams and calling virtual function UDPServer::main.
void Socket::UDPServer::acceptDatagrams()
{
	bool lHalt = false;
	mHalt = false;
	while(!lHalt && !mHalt)
	{
		try {
			string lDatagram;
			Address lPeer;
			receiveDatagram(lDatagram, lPeer);
			lHalt = main(lDatagram, lPeer);
		} catch(const Exception& inError) {
			// report any error and ignore
			cerr << inError.getMessage() << endl;
		}
	}
}

/*! \brief Set default socket options.

Default options are:
<ul>
<li>eReuseAddress = true</li>
<li>eRecvBufSize = 65535 bytes</li>
<li>eSendBufSize = 1024 bytes</li>
<li>eRecvTimeOut = 10 seconds</li>
<li>eSendTimeOut = 10 seconds</li>
</ul>
Any error raises a Socket::Exception.
*/
void Socket::UDPServer::setDefaultOptions()
{
	setSockOpt(eReuseAddress, true);
	setSockOpt(eRecvBufSize, 65535);
	setSockOpt(eSendBufSize, 1024);
	setSockOpt(eRecvTimeOut, 10);
	setSockOpt(eSendTimeOut, 10);
}
