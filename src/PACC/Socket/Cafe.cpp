/*
 *  Portable Agile C++ Classes (PACC)
 *  Copyright (C) 2001-2003 by Marc Parizeau and Marc Dubreuil
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
* \file PACC/Socket/Cafe.cpp
 * \brief Class methods for the cafe protocol.
 * \author Marc Parizeau and Marc Dubreuil, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.28 $
 * $Date: 2008/04/17 21:09:08 $
 */

#include "PACC/Socket/Cafe.hpp"
#include "PACC/config.hpp"
#include <iostream>
#include <cstring>

#ifdef PACC_SOCKET_WIN32
///////////// specifics for windows /////////////
#include <winsock2.h>
namespace PACC {
	typedef u_long UInt32;
}

#else
///////////// specifics for unixes /////////////
#include <netinet/in.h>
namespace PACC {
	typedef uint32_t UInt32;
}
#endif

#ifdef PACC_ZLIB
// Set this variable in order to enable message compression
#include <zlib.h>
#endif

using namespace std;
using namespace PACC;

/*!
WARNING: in order to enable message compression, this class needs to be compiled with variable PACC_ZLIB set.
*/
#ifdef PACC_ZLIB
void Socket::Cafe::compress(const std::string& inMessage, std::string& outMessage, unsigned int inCompressionLevel)
{
	if(inCompressionLevel == 0) outMessage = inMessage;
	else
	{
		// size of buffer must be at least 0.1% + 12 bytes larger than the message size
		unsigned long lBufLength = (inMessage.size() + inMessage.size()/1000 + 1) + 12;
		outMessage.resize(lBufLength);
		// compress message
		int lReturn = ::compress2((Bytef *)&outMessage[0], (uLongf*)&lBufLength, (const Bytef *)inMessage.data(), inMessage.size(), inCompressionLevel);
		// check for error
		if(lReturn != Z_OK) 
		{
			outMessage.resize(0);
			throw Exception(eOtherError, "Cafe::compress() unable to compress message!");
		}
		// resize message to size of compressed message
		outMessage.resize(lBufLength);
	}
}
#endif

/*!
This method will wait until the specified amount of bytes in received from the socket. It assumes that buffer \c inBuffer is large enough to accept \c inCount bytes. Any error (e.g. timeouts or broken connection) will throw a Socket::Exception.
*/
void Socket::Cafe::receive(char* inBuffer, unsigned int inCount)
{
	PACC::UInt32 lTotalReceived = 0;
	while(lTotalReceived < inCount) {
		PACC::UInt32 lRecv = Port::receive(inBuffer+lTotalReceived, inCount-lTotalReceived);
		lTotalReceived += lRecv;;
	}
}

/*!
This function waits for a valid message according to the Cafe protocol, or until 
time out. It returns the received message through output parameter \c OutMessage. 
Any error raises a Socket::Exception. For instance, it throws an exception with 
code Socket::eConnectionClosed if the connection is closed by the other party 
before message end, with code Socket::eBadMessage if the protocol signature is 
invalid, or with code Socket::eTimeOut if the timeout period expires before 
reception of a cmoplete message. The timeout period can be changed using function 
Port::setSockOpt with parameter Socket::eRecvTimeOut.
 */
void Socket::Cafe::receiveMessage(string& outMessage)
{
	if(mDescriptor < 0) throw Exception(eBadDescriptor, "Cafe::receiveMessage() invalid socket");
	// check message signature
	PACC::UInt32 lSignature = 0;
	receive((char*)&lSignature, sizeof(lSignature));
	switch(ntohl(lSignature)) 
	{
		case 0xCAFE: // uncompressed Cafe
		{
			// read message size
			PACC::UInt32 lMessageSize = 0;
			receive((char*)&lMessageSize, sizeof(lMessageSize));
			lMessageSize = ntohl(lMessageSize);
			// resize output string
			outMessage.resize(lMessageSize);
			// read message body
			receive(&outMessage[0], lMessageSize);
			break;
		}
		case 0xCCAFE: // compressed Cafe
		{
#ifdef PACC_ZLIB
			// read message size
			PACC::UInt32 lMessageSize = 0;
			receive((char*)&lMessageSize, sizeof(lMessageSize));
			lMessageSize = ntohl(lMessageSize);
			// resize output string
			outMessage.resize(lMessageSize);
			//read uncompressed size
			PACC::UInt32 lUncompressedSize = 0;
			receive((char*)&lUncompressedSize, sizeof(lUncompressedSize));
			lUncompressedSize = ntohl(lUncompressedSize);			
			// read message body
			receive(&outMessage[0], lMessageSize);
			//decompress message 
			uncompress(outMessage, lUncompressedSize);
			break;
#else
			throw Exception(eOtherError, "Cafe::receiveMessage() class needs to be compiled with variable PACC_ZLIB set, in order to enable message decompression");
#endif
		}
		default: // unknown
			throw Exception(eBadMessage, "Cafe::receiveMessage() invalid signature");
	}
}

/*!
This function sends a message string using either of the two Cafe protocols. The 
user can specify a compression level using argument \c inCompressionLevel. 
However, this argument is ignored by default and messages are not compressed in 
any way. To enable compression, the class must be compiled with variable 
PACC_ZLIB set, and the executable must be linked with the zlib library 
<http://www.gzip.org/zlib>. 

Compression level must range from 0 to 9. A nul compression level means no 
compression, while a level of 9 specifies maximum compression. The message is 
sent compressed only if smaller than the original.

Any error raises a Socket::Exception. For instance, it throws an exception with 
code Socket::eConnectionClosed if the connection is closed by the other party 
during message transmission, or with code Socket::eTimeOut if the message 
cannot be sent before the time out period expires. The time out period can be 
changed using function Port::setSockOpt with parameter Socket::eSendTimeOut.
 */
void Socket::Cafe::sendMessage(const string& inMessage, unsigned int inCompressionLevel)
{
	if(inCompressionLevel > 9)
	{
		throw Exception(eOtherError, "Cafe::sendMessage() invalid compression level!");
	}
#ifdef PACC_ZLIB
	if(inCompressionLevel > 0)
	{
		// try to compress message
		string lCompressedMessage;
		compress(inMessage, lCompressedMessage, inCompressionLevel);
		if(lCompressedMessage.size() < inMessage.size()) 
		{
			// send compressed message only if smaller
			// assume MTU is 1500 bytes - IP headers
			char lBuffer[1460]; 
			PACC::UInt32* lHeader = (PACC::UInt32*) lBuffer;
			lHeader[0] = htonl(0xCCAFE);
			lHeader[1] = htonl(lCompressedMessage.size());
			lHeader[2] = htonl(inMessage.size());
			// copy start of compressed message to header
			memcpy(lBuffer+12, lCompressedMessage.data(), (lCompressedMessage.size()<sizeof(lBuffer)-12?lCompressedMessage.size():sizeof(lBuffer)-12));
			// write header and compressed message
			send(lBuffer, (lCompressedMessage.size()<sizeof(lBuffer)-12?lCompressedMessage.size()+12:sizeof(lBuffer)));
			if(lCompressedMessage.size() > sizeof(lBuffer)-12) send(lCompressedMessage.data()+sizeof(lBuffer)-12, lCompressedMessage.size()-sizeof(lBuffer)+12);
		} else {
			// send uncompressed message
			// assume MTU is 1500 bytes - IP headers
			char lBuffer[1460]; 
			PACC::UInt32* lHeader = (PACC::UInt32*) lBuffer;
			lHeader[0] = htonl(0xCAFE);
			lHeader[1] = htonl(inMessage.size());
			// copy start of uncompressed message to header
			memcpy(lBuffer+8, inMessage.data(), (inMessage.size()<sizeof(lBuffer)-8?inMessage.size():sizeof(lBuffer)-8));
			// write header and uncompressed message
			send(lBuffer, (inMessage.size()<sizeof(lBuffer)-8?inMessage.size()+8:sizeof(lBuffer)));
			if(inMessage.size() > sizeof(lBuffer)-8) send(inMessage.data()+sizeof(lBuffer)-8, inMessage.size()-sizeof(lBuffer)+8);
		}
	} else
#endif
	{
		// send uncompressed message (compression is disabled)
		// assume MTU is 1500 bytes - IP headers
		char lBuffer[1460]; 
		PACC::UInt32* lHeader = (PACC::UInt32*) lBuffer;
		lHeader[0] = htonl(0xCAFE);
		lHeader[1] = htonl(inMessage.size());
		// copy start of message to header
		memcpy(lBuffer+8, inMessage.data(), (inMessage.size()<sizeof(lBuffer)-8?inMessage.size():sizeof(lBuffer)-8));
		// write header and message
		send(lBuffer, (inMessage.size()<sizeof(lBuffer)-8?inMessage.size()+8:sizeof(lBuffer)));
		if(inMessage.size() > sizeof(lBuffer)-8) send(inMessage.data()+sizeof(lBuffer)-8, inMessage.size()-sizeof(lBuffer)+8);
	}
}


/*!
WARNING: in order to enable message compression/uncompression, this class needs to be compiled with variable PACC_ZLIB set.
*/
#ifdef PACC_ZLIB
void Socket::Cafe::uncompress(std::string& ioMessage, unsigned long inUncompressedSize)
{
	string lUncompressedMessage;
	lUncompressedMessage.resize(inUncompressedSize);
	int lReturn = ::uncompress((Bytef*)&lUncompressedMessage[0], (uLong*)&inUncompressedSize, (const Bytef*)ioMessage.data(), ioMessage.size());
	if(lReturn != Z_OK) {
		throw Exception(eOtherError, "Cafe::uncompress() unable to uncompress message!");
	}
	ioMessage = lUncompressedMessage;
}
#endif
