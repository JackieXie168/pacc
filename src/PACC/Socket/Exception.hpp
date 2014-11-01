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
 * \file PACC/Socket/Exception.hpp
 * \brief Class definition for the portable socket exception.
 * \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.25 $
 * $Date: 2005/11/30 18:29:34 $
 */

#ifndef PACC_Socket_Exception_hpp_
#define PACC_Socket_Exception_hpp_

#include <stdexcept>
#include <string>

namespace PACC { 
	
	using namespace std;
	
	namespace Socket {
		
		/*! 
		\brief Supported socket error codes.
		\author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
		\ingroup Socket
		*/
		enum Error {
			eAddressInUse, //!< %Address is already in use (bind & connect).
			eAddressNotAvailable, //!< %Address not available on machine (bind & connect).
			eBadDescriptor, //!< Invalid socket descriptor (socket).
			eBadMessage, //!< Message signature is invalid.
			eConnectionClosed, //!< Connection was closed (or broken) by other party.
			eConnectionRefused, //!< Connection refused by server.
			eDatagramTooLong, //!< Datagram too long to send atomically.
			eInvalidOption, //!< Invalid option for socket protocol.
			eIsConnected, //!< %Socket is already connected.
			eNotConnected, //!< %Socket is not connected.
			eOpNotSupported, //!< Operation is not supported for this socket.
			ePrivilegedPort, //!< User does not have acces to privileged ports (bind).
			eTimeOut, //!< Time out was reached for operation (receive & send).
			eOtherError //!< Any other OS specific error.
		};
		
		/*!
		\brief %Socket exception.
		 \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
		 \ingroup Socket
		 
		 This class encapsulates most interesting runtime errors that can happen while using sockets.
		 */
		class Exception : public runtime_error {
		 public:
			//! Construct exception with native error code \c inCode and message \c inMessage.
			explicit Exception(int inCode, const string& inMessage)
			: runtime_error(inMessage), mNativeCode(inCode) {mCode = convertNativeError(inCode);}
			
			//! Construct exception with portable error code \c inCode and message \c inMessage.
			explicit Exception(Error inCode, const string& inMessage)
			: runtime_error(inMessage), mCode(inCode) {mNativeCode = 0;}
			
			static Error convertNativeError(int inError);
			
			//! Return error code.
			int getErrorCode(void) const {return mCode;}

			//! Return exception message.
			string getMessage(void) const throw();

		 protected:
			Error mCode; //!< Portable error code
			int mNativeCode; //!< Native error code
			
		};
		
	} // end of Socket namespace
	
} // end of PACC namespace

#endif  // PACC_Socket_Exception_hpp_
