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
 * $Revision: 1.29 $
 * $Date: 2004/11/19 01:51:49 $
 */

#ifndef PACC_Socket_Port_hpp_
#define PACC_Socket_Port_hpp_

#include "Socket/Address.hpp"
#include "Socket/Exception.hpp"

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
      eReuseAddress, //!< Allow reuse of address
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
   class Port
   {
    public:
      //! Return socket descriptor
      int getDescriptor() const {return mDescriptor;}
      
      Address getPeerAddress(void) const;
      Protocol getProtocol(void) const;
      Address getSockAddress(void) const;
      double getSockOpt(Option inName) const;
      void setSockOpt(Option inName, double inValue);

    protected:
      int mDescriptor; //!< socket descriptor

      explicit Port(int inDescriptor) throw();
      explicit Port(Protocol inProtocol=eTCP);

      ~Port();
      
      int  accept(void);
      void bind(unsigned int inPortNumber);
      void close(void);
      void connect(const Address& inPeer);
      int convertToNativeOption(Option inName) const;
      void listen(unsigned int inMaxConnections);
      void open(Protocol = eTCP);
      unsigned int receive(char* outBuffer, unsigned inMaxCount);
      unsigned int receiveFrom(char* outBuffer, unsigned inMaxCount, Address& outPeer);
      void send(const char* inBuffer, unsigned int inCount);
      void sendTo(const char* inBuffer, unsigned int inCount, const Address& inPeer);
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



