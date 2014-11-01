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
 * \file PACC/Socket/Port.cpp
 * \brief Class methods for the portable socket base class.
 * \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.53 $
 * $Date: 2008/10/29 21:52:40 $
 */

#include "PACC/Socket/Port.hpp"
#include "PACC/config.hpp"

#ifdef PACC_SOCKET_WIN32
///////////// specifics for windows /////////////
#include <winsock2.h>
typedef int socklen_t;
#define ErrNo WSAGetLastError() // descriptor of last error
#define SIGPIPE SIGABRT // windows does not generate SIGPIPE

#else
///////////// specifics for unixes /////////////
#define INVALID_SOCKET -1
#define _BSD_SOCKLEN_T_ // needed by darwin
#define ErrNo errno // descriptor of last error
#include <sys/socket.h>
#include <sys/errno.h>
#include <sys/time.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

#include <signal.h>
#include <sstream>
#include <cstring>

using namespace std;
using namespace PACC;

/*!
 */
Socket::Port::Port(int inDescriptor) throw() : mDescriptor(inDescriptor) {}

/*!
 */
Socket::Port::Port(Socket::Protocol inProtocol) : mDescriptor(INVALID_SOCKET) 
{
	open(inProtocol);
}

/*!
 */
Socket::Port::~Port()
{
	close();
}

/*!
Any error raises a Socket::Exception.
 */
int Socket::Port::accept()
{
	if(mDescriptor == INVALID_SOCKET) throw Exception(eBadDescriptor, "Port::accept() invalid socket");
	struct sockaddr_in lSock;
	socklen_t lSize = sizeof(lSock);
	int lDescriptor = ::accept(mDescriptor, (struct sockaddr*) &lSock, &lSize);
	if(lDescriptor < 0) {
		throw Exception(ErrNo, "Port::acept() unable to accept connection");
	}
	return lDescriptor;
}

/*!
Any error raises a Socket::Exception. In particular, this method may fail if 
the port is already binded by another process or thread. It may even 
fail for a few seconds after the connection is released by another process or 
thread (see Option::eReuseAddress).
 */
void Socket::Port::bind(unsigned int inPortNumber)
{
	if(mDescriptor == INVALID_SOCKET) throw Exception(eBadDescriptor, "Port::bind() invalid socket");
	struct sockaddr_in lSock;
	lSock.sin_family = AF_INET;
	lSock.sin_port = htons(inPortNumber);
	lSock.sin_addr.s_addr = htonl(INADDR_ANY);
	memset(&lSock.sin_zero, 0, 8);
	if(::bind(mDescriptor, (struct sockaddr*) &lSock, sizeof(lSock)) != 0) {
		int lCode = ErrNo;
		ostringstream lMessage;
		lMessage << "Port::bind() unable to bind port: " << inPortNumber;
		throw Exception(lCode, lMessage.str());
	}
}

/*!
This function will shutdown the connection and free the resources associated with the socket. It may block (linger) for a while, until the send buffer is empty. The linger delay can be set using function Port::setSockOpt with Socket::Option parameter \c eLinger. Any error raises a Socket::Exception.
 */
void Socket::Port::close()
{
	if(mDescriptor != INVALID_SOCKET) {
#ifdef PACC_SOCKET_WIN32
		::shutdown(mDescriptor, SD_BOTH);
		if(::closesocket(mDescriptor) != 0)
#else
		::shutdown(mDescriptor, SHUT_RDWR);
		if(::close(mDescriptor) != 0)
#endif
		{
			throw Exception(ErrNo, "Port::close() unable to close (or bad) socket descriptor");
		}
	}
	mDescriptor = INVALID_SOCKET;
}

/*!
This function is used to connect the socket to a peer server at address \c inPeer. For TCP sockets, the connection handshake is immediate. When the function returns, the connection has been established and the socket pair is ready for data transmission. For UDP sockets, no handshake is operated before data transmission. The connection is thus virtual. All subsequent data transmission (send or receive) will be with the peer server at the specified address. Once connected, a TCP socket may not be reconnected to a different server, contrary to UDP sockets which may be reconnected any number of times. Any error raises a Socket::Exception.
 */
void Socket::Port::connect(const Socket::Address& inPeer)
{
	if(mDescriptor == INVALID_SOCKET) throw Exception(eBadDescriptor, "Port::connect() invalid socket");
	struct sockaddr_in lSock;
	lSock.sin_family = AF_INET;
	lSock.sin_port = htons(inPeer.getPortNumber());
	lSock.sin_addr.s_addr = inet_addr(inPeer.getIPAddress().c_str());
	memset(&lSock.sin_zero, 0, 8);
	if(::connect(mDescriptor, (struct sockaddr*) &lSock, sizeof(lSock)) != 0) {
		int lCode = ErrNo;
		ostringstream lMessage;
		lMessage << "Port::connect() unable to connect to server \"" << inPeer.getHostName();
		lMessage << "\" at port " << inPeer.getPortNumber();
		throw Exception(lCode, lMessage.str());
	}
}

/*!
Any error raises a Socket::Exception.
 */
int Socket::Port::convertToNativeOption(Socket::Option inName) const
{
	int lNativeOpt;
	switch(inName) {
		case eKeepAlive: lNativeOpt = SO_KEEPALIVE; break;
		case eLinger: lNativeOpt = SO_LINGER; break;
		case eNoDelay: lNativeOpt = TCP_NODELAY; break;
		case eProtocolType: lNativeOpt = SO_TYPE; break;
		case eReuseAddress: lNativeOpt = SO_REUSEADDR; break;
		case eRecvBufSize: lNativeOpt = SO_RCVBUF; break;
		case eSendBufSize: lNativeOpt = SO_SNDBUF; break;
		case eRecvTimeOut: lNativeOpt = SO_RCVTIMEO; break;
		case eSendTimeOut: lNativeOpt = SO_SNDTIMEO; break;
		default: throw Exception(eOtherError, "Port::convertToNativeOption() unknown socket option");
	}
	return lNativeOpt;
}

/*!
Any error raises a Socket::Exception.
 */
Socket::Address Socket::Port::getPeerAddress() const
{
	struct sockaddr lSock;
	socklen_t lLength = sizeof(lSock);
	if(::getpeername(mDescriptor, &lSock, &lLength) != 0) {
		throw Exception(eNotConnected, "Port::getPeerAddress() unable to retrieve peer address");
	}
	return Address(ntohs(((sockaddr_in*)&lSock)->sin_port), inet_ntoa(((sockaddr_in*)&lSock)->sin_addr));
}

/*!
Any error raises a Socket::Exception.
 */
Socket::Protocol Socket::Port::getProtocol() const
{
	
	int lProtocol = (int) getSockOpt(eProtocolType);
	if(lProtocol == SOCK_STREAM) return eTCP;
	else if(lProtocol == SOCK_DGRAM) return eUDP;
	else return eOther;
}

/*!
Any error raises a Socket::Exception.
 */
Socket::Address Socket::Port::getSockAddress() const
{
	struct sockaddr lSock;
	socklen_t lLength = sizeof(lSock);
	if(::getsockname(mDescriptor, &lSock, &lLength) != 0) {
		throw Exception(eOtherError, "Port::getSockAddress() unable to retrieve socket address");
	}
	return Address(ntohs(((sockaddr_in*)&lSock)->sin_port), inet_ntoa(((sockaddr_in*)&lSock)->sin_addr));
}

/*!
Valid options are:
<ul>
<li>eKeepAlive: keep connection alive (TCP only)</li>
<li>eLinger: time to linger on close (in seconds, TCP only)</li>
<li>eNoDelay: disable the Nagle algorithm for packet coalescing (TCP only)</li>
<li>eProtocolType: socket protocol type</li>
<li>eReuseAddress: allow reuse of address</li>
<li>eRecvBufSize: size of receive buffer (in bytes)</li>
<li>eSendBufSize: size of send buffer (in bytes)</li>
<li>eRecvTimeOut: time out period for receive operations (in seconds)</li>
<li>eSendTimeOut: time out period for send operations (in seconds)</li>
</ul>
Any error raises a Socket::Exception.
 */
double Socket::Port::getSockOpt(Socket::Option inName) const
{
	double lValue;
	int lBuffer[2] = {0, 0};
	socklen_t lSize = sizeof(lBuffer);
	if(::getsockopt(mDescriptor, (inName==eNoDelay?IPPROTO_TCP:SOL_SOCKET), convertToNativeOption(inName), (char*)lBuffer, &lSize) != 0)
	{
		throw Exception(ErrNo, "Port::getSockOpt() unable to retrieve socket option");
	}
	switch(inName) {
		case eKeepAlive: case eNoDelay: case eReuseAddress: case eRecvBufSize: case eSendBufSize: case eProtocolType:
			lValue = lBuffer[0];
			break;
		case eLinger:
		{
			// warning: the linger structure is not the same size on windows and unix!
			struct linger* lPtr = (struct linger*) lBuffer;
			if(lPtr->l_onoff) lValue = lPtr->l_linger;
			else lValue = -1;
		}
			break;
		case eRecvTimeOut: case eSendTimeOut:
#ifdef PACC_SOCKET_WIN32
			// on windows, timeout is specified in milliseconds
			lValue = (float)lBuffer[0]/1000.;
#else
			// on unix, timeout is specified with timeval structure
			{
				struct timeval* lPtr = (struct timeval*) lBuffer;
				lValue = (float)lPtr->tv_sec + lPtr->tv_usec/1000000.;
			}
#endif
				break;
		default:
			throw Exception(eOtherError, "Port::getSockOpt() unknown socket option");
	}
	return lValue;
}

/*!
 */
void Socket::Port::listen(unsigned int inMinPending)
{
	if(mDescriptor == INVALID_SOCKET) throw Exception(eBadDescriptor, "Port::listen() invalid socket");
	if(::listen(mDescriptor, inMinPending) != 0) {
		throw Exception(ErrNo, "Port::listen() unable to listen to port");
	}
}

/*!
A socket descriptor is allocated using the protocol determined by parameter \c inProtocol. The TCP protocol is selected with value \c eTCP (default), while the UDP protocol is chosen with value \c eUDP. 

Any error raises a Socket::Exception.
 */
void Socket::Port::open(Socket::Protocol inProtocol)
{
#ifdef PACC_SOCKET_WIN32
	static bool lInitialized = false;
	if(!lInitialized) {
		WSADATA wsdata;
		if (WSAStartup(MAKEWORD(2,2), &wsdata) != 0) {
			throw Exception(eOtherError, "Port::open() failed to load WinSock2");
		}
		lInitialized = true;
	}
#endif
	// first close socket if already open
	if(!INVALID_SOCKET) close();
	// select protocol and create new socket descriptor
	if(inProtocol == eTCP) mDescriptor = ::socket(AF_INET, SOCK_STREAM, 0);
	else if(inProtocol == eUDP) mDescriptor = ::socket(AF_INET, SOCK_DGRAM, 0);
	else throw Exception(eOtherError, "Port::open() unsupported socket protocol");
	if(mDescriptor == INVALID_SOCKET) {
		throw Exception(eBadDescriptor, "Port::open() unable to allocate socket descriptor");
	}
}

/*!
\return Number of received characters.

This function waits until some data is received through the socket, or until the timeout period expires. The received data is returned through output parameter \c outBuffer which must be large enough to contain up to \c inMaxCount characters. Note that this ouput buffer is NOT null terminated. The function returns the effective number of read characters contained in the output buffer. Any error raises a Socket::Exception. For instance, if the other party closes the connection (TCP sockets), an exception is thrown with code Socket::eConnectionClosed, or with code Socket::eTimeOut if nothing is received before the time out period expires. The time out period can be changed using function Port::setSockOpt with parameter Socket::eRecvTimeOut.
 */
unsigned int Socket::Port::receive(char* outBuffer, unsigned inMaxCount)
{
	if(mDescriptor == INVALID_SOCKET) throw Exception(eBadDescriptor, "Port::receive() invalid socket");
	void(*lPipeMethod)(int) = ::signal(SIGPIPE, SIG_IGN);
	int lRecv = ::recv(mDescriptor, outBuffer, inMaxCount, 0);
	::signal(SIGPIPE, lPipeMethod);
	if(lRecv < 0) {
		throw Exception(ErrNo, "Port::receive() operation incomplete");
	} else if(lRecv == 0) {
		close();
		throw Exception(eConnectionClosed, "Port::receive() operation incomplete");
	}
	return lRecv;
}

/*!
\return Number of received characters .

This function waits until some data is received through the socket, or until the timeout period expires. The received data is returned through output parameter \c outBuffer which must be large enough to contain up to \c inMaxCount characters. Note that this ouput buffer is NOT null terminated. The function returns the effective number of read characters contained in the output buffer. The peer socket address is also returned through output parameter \c outPeer. Any error raises a Socket::Exception. For instance, if the other party closes the connection (TCP sockets), an exception is thrown with code Socket::eConnectionClosed, or with code Socket::eTimeOut if nothing is received before the time out period expires. The time out period can be changed using function Port::setSockOpt with parameter Socket::eRecvTimeOut.
 */
unsigned int Socket::Port::receiveFrom(char* outBuffer, unsigned inMaxCount, Address& outPeer)
{
	if(mDescriptor == INVALID_SOCKET) throw Exception(eBadDescriptor, "Port::receiveFrom() invalid socket");
	struct sockaddr_in lSock;
	socklen_t lSize = sizeof(lSock);
	void(*lPipeMethod)(int) = ::signal(SIGPIPE, SIG_IGN);
	int lRecv = ::recvfrom(mDescriptor, outBuffer, inMaxCount, 0, (struct sockaddr*) &lSock, &lSize);
	::signal(SIGPIPE, lPipeMethod);
	if(lRecv < 0) {
		throw Exception(ErrNo, "Port::receive() operation incomplete");
	} else if(lRecv == 0) {
		throw Exception(eConnectionClosed, "Port::receive() operation incomplete");
	}
	// transfer peer address
	outPeer = Address(ntohs(lSock.sin_port), inet_ntoa(lSock.sin_addr));
	return lRecv;
}

/*! 
This function sends to its peer socket the \c inCount characters of data contained in buffer \c inBuffer (socket is assumed connected). Any error raises a Socket::Exception. For instance, it throws an exception with code Socket::eConnectionClosed if the connection is closed by the other party during message transmission, or with code Socket::eTimeOut if the message cannot be sent before the time out period expires. The time out period can be changed using function Port::setSockOpt with parameter Socket::eSendTimeOut.
*/
void Socket::Port::send(const char* inBuffer, unsigned int inCount)
{
	if(mDescriptor == INVALID_SOCKET) throw Exception(eBadDescriptor, "Port::send() invalid socket");
	unsigned int lTotalSent = 0;
	// send all data
	while(lTotalSent < inCount) {
		void(*lPipeMethod)(int) = ::signal(SIGPIPE, SIG_IGN);
		int lSent = ::send(mDescriptor, inBuffer+lTotalSent, inCount-lTotalSent, 0);
		::signal(SIGPIPE, lPipeMethod);
		if(lSent < 0) {
			throw Exception(ErrNo, "Port::send() operation incomplete");
		} else if(lSent < 1) {
			close();
			throw Exception(eConnectionClosed, "Port::send() operation incomplete");
		}
		lTotalSent += lSent;
	}
}

/*!
This function sends to peer \c inPeer the data contained in buffer \c inBuffer (total of \c inCount characters). Any error raises a Socket::Exception. For instance, it throws an exception with code Socket::eConnectionClosed if the connection is closed by the other party during message transmission, or with code Socket::eTimeOut if the message cannot be sent before the time out period expires. The time out period can be changed using function Port::setSockOpt with parameter Socket::eSendTimeOut.
*/
void Socket::Port::sendTo(const char* inBuffer, unsigned int inCount, const Socket::Address& inPeer)
{
	if(mDescriptor == INVALID_SOCKET) throw Exception(eBadDescriptor, "Port::sendTo() invalid socket");
	struct sockaddr_in lSock;
	lSock.sin_family = AF_INET;
	lSock.sin_port = htons(inPeer.getPortNumber());
	lSock.sin_addr.s_addr = inet_addr(inPeer.getIPAddress().c_str());
	memset(&lSock.sin_zero, 0, 8);
	unsigned int lTotalSent = 0;
	// send all data
	while(lTotalSent < inCount) {
		void(*lPipeMethod)(int) = ::signal(SIGPIPE, SIG_IGN);
		int lSent = ::sendto(mDescriptor, inBuffer+lTotalSent, inCount-lTotalSent, 0, (struct sockaddr*) &lSock, sizeof(lSock));
		::signal(SIGPIPE, lPipeMethod);
		if(lSent < 0) {
			throw Exception(ErrNo, "Port::send() operation incomplete");
		} else if(lSent < 1) {
			close();
			throw Exception(eConnectionClosed, "Port::send() operation incomplete");
		}
		lTotalSent += lSent;
	}
}

/*!
Options are:
<ul>
<li>eKeepAlive: keep connection alive (TCP only)</li>
<li>eLinger: time to linger on close (in seconds; TCP only)</li>
<li>eNoDelay: Disable the Nagle algorithm for packet coalescing (TCP only)</li>
<li>eReuseAddress: allow reuse of address</li>
<li>eRecvBufSize: size of receive buffer (in bytes)</li>
<li>eSendBufSize: size of send buffer (in bytes)</li>
<li>eRecvTimeOut: time out period for receive operations (in seconds)</li>
<li>eSendTimeOut: time out period for send operations (in seconds)</li>
</ul>
Note that for option \c eLinger, a negative value means don't linger. For options \c eRecvTimeOut and \c eSendTimeOut, a negative or nul value means dont't timeout, and a positive value of less than 1 msec will be equivalent to 1 msec. Any error raises a Socket::Exception.
 */
void Socket::Port::setSockOpt(Option inName, double inValue)
{
	int lBuffer[2] = {0, 0};
	socklen_t lSize;
	switch(inName) {
		case eKeepAlive: case eNoDelay: case eReuseAddress: case eRecvBufSize: case eSendBufSize:
			lBuffer[0] = (int) inValue;
			lSize = sizeof(int);
			break;
		case eLinger:
		{
			// warning: the linger structure is not the same size on windows and unix!
			struct linger* lPtr = (struct linger*) lBuffer;
#ifdef PACC_SOCKET_WIN32
			lPtr->l_onoff = (u_short) (inValue >= 0);
			lPtr->l_linger = (u_short) (inValue >= 0 ? inValue : 0);
#else
			lPtr->l_onoff = (int) (inValue >= 0);
			lPtr->l_linger = (int) (inValue >= 0 ? inValue : 0);
#endif
		}
			lSize = sizeof(struct linger);
			break;
		case eRecvTimeOut: case eSendTimeOut:
			if(inValue < 0) inValue = 0;
			else if(inValue > 0 && inValue < 0.001) inValue = 0.001;
#ifdef PACC_SOCKET_WIN32
				// on windows, timeout must be specified in milliseconds
				lBuffer[0] = (int) (1000*inValue);
			lSize = sizeof(int);
#else
			// on unix, timeout must be specified with timeval structure
			{
				struct timeval* lPtr = (struct timeval*) lBuffer;
				lPtr->tv_sec = (int32_t) inValue;
				lPtr->tv_usec = (int32_t) ((inValue-lPtr->tv_sec)*1000000);
				lSize = sizeof(struct timeval);
			}
#endif
				break;
		default:
			throw Exception(eOtherError, "Port::setSockOpt() unsupported socket option");
	}
	if(::setsockopt(mDescriptor, (inName==eNoDelay?IPPROTO_TCP:SOL_SOCKET), convertToNativeOption(inName), (char*)lBuffer, lSize) != 0)
	{
		throw Exception(ErrNo, "Port::setSockOpt() unable to set socket option");
	}
}

/*!
This function waits for any socket activity for up to \c inSeconds seconds. The three types of activities are: ready for read, ready for write, and pending errors. It returns true if an activity is detected before timeout, and false otherwise.
*/
bool Socket::Port::waitForActivity(double inSeconds)
{
	fd_set lRead, lWrite, lError;
	FD_ZERO(&lRead);
	FD_SET(mDescriptor, &lRead);
	FD_ZERO(&lWrite);
	FD_SET(mDescriptor, &lWrite);
	FD_ZERO(&lError);
	FD_SET(mDescriptor, &lError);
	struct timeval lTime;
	lTime.tv_sec = (long) inSeconds;
	lTime.tv_usec = (long) ((inSeconds-lTime.tv_sec)*1000000);
	return select(FD_SETSIZE, &lRead, &lWrite, &lError, &lTime) == 1;
}
