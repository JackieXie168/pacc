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
 * \file PACC/Socket/Port.hpp
 * \brief Class definition for the portable socket base class.
 * \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.35 $
 * $Date: 2007/01/23 21:27:46 $
 */

#ifndef PACC_Socket_Port_hpp_
#define PACC_Socket_Port_hpp_

#include "PACC/Socket/Address.hpp"
#include "PACC/Socket/Exception.hpp"

namespace PACC { 
	
	namespace Socket {
		
		/*! 
		\brief Supported socket protocols.
		\author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
		\ingroup Socket
		*/
		enum Protocol {
			eTCP, //!< Transfer Control Protocol
			eUDP, //!< User Datagram Protocol
			eOther //!< Other protocol
		};
		
		/*! 
		\brief Supported socket options.
		\author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
		\ingroup Socket
		*/
		enum Option {
			eKeepAlive, //!< Keep connection alive
			eLinger, //!< Time to linger on close (in seconds)
			eNoDelay, //!< Disable the Nagle algorithm for packet coalescing
			eProtocolType, //!< %Socket protocol type
			eReuseAddress, //!< Allow reuse of a TCP address without delay
			eRecvBufSize, //!< Size of receive buffer (in number of chars)
			eSendBufSize, //!< Size of send buffer (in number of chars)
			eRecvTimeOut, //!< Time out period for receive operations (in seconds)
			eSendTimeOut //!< Time out period for send operations (in seconds)
		};
		
		/*!
			\brief Portable socket base class.
		 \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
		 \ingroup Socket
		 
		 This class encapsulates a cross-platform socket port descriptor (not to be confused with the port number). It can be used with both %TCP and %UDP protocols. It is usually subclassed to implement either a client or server connection. Any error raises a Socket::Exception. 
		 
		 This class should be compatible with any POSIX Unix, and with any version of windows. It has been tested under Linux, MacOS X and Windows 2000/XP.
		 */
		class Port {
		 public:
			//! Return socket descriptor
			int getDescriptor() const {return mDescriptor;}
			
			//! Return address of peer socket host.
			Address getPeerAddress(void) const;
			
			//! Return protocol of socket.
			Protocol getProtocol(void) const;
			
			//! Return address of socket host.
			Address getSockAddress(void) const;
			
			//! Return value of socket option \c inName.
			double getSockOpt(Option inName) const;

			//! Set socket option \c inName to value \c inValue.
			void setSockOpt(Option inName, double inValue);
			
		 protected:
			int mDescriptor; //!< socket descriptor
			
			//! Construct using existing socket descriptor \c inDescriptor.
			explicit Port(int inDescriptor) throw();
			
			//! Construct new socket descriptor using protocol \c inProtocol (default=%TCP).
			explicit Port(Protocol inProtocol=eTCP);
			
			//! Close descriptor and deallocate receive buffer.
			~Port();

			//! Accept connection on bound port.
			int  accept(void);
			
			//! Bind socket to port number \c inPortNumber.
			void bind(unsigned int inPortNumber);
			
			//! Close socket port.
			void close(void);
			
			//! Connect to peer socket \c inPeer.
			void connect(const Address& inPeer);
			
			//! Convert socket option \c inName to native socket option code.
			int convertToNativeOption(Option inName) const;
			
			//! Listen to socket using a queue of at least \c inMinPending pending connections.
			void listen(unsigned int inMaxConnections);
			
			//! Open new socket descriptor.
			void open(Protocol = eTCP);
			
			//! Receive data from connected socket.
			unsigned int receive(char* outBuffer, unsigned inMaxCount);
			
			//! Receive data from unconnected socket.
			unsigned int receiveFrom(char* outBuffer, unsigned inMaxCount, Address& outPeer);
			
			//! Send data to connected socket.
			void send(const char* inBuffer, unsigned int inCount);
			
			//! Send data to unconnected socket.
			void sendTo(const char* inBuffer, unsigned int inCount, const Address& inPeer);
			
			//! Wait for activity.
			bool waitForActivity(double inSeconds);
			
		 private:
			//! restrict (disable) copy constructor.
			Port(const Port&);
			
			//! restrict (disable) assignment operator.
			void operator=(const Port&);

		};
		
	} // end of Socket namespace
	
} // end of PACC namespace

#endif  // PACC_Socket_Port_hpp_
