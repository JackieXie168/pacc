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
 * \file PACC/Socket/Address.hpp
 * \brief Class definition for the portable network address.
 * \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.16 $
 * $Date: 2004/11/30 02:30:35 $
 */

#ifndef PACC_Socket_Address_hpp_
#define PACC_Socket_Address_hpp_

#include <string>

namespace PACC { 
   
   using namespace std;
   
   namespace Socket {

		/*! \brief Portable network address.
		 \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
		 \ingroup Socket
		 
		 This class encapsulates a cross-platform address for a peer host. It should be compatible with any POSIX Unix, and with any version of windows. It has been tested under Linux, MacOS X and Windows 2000/XP.
		 
		 Any error raises a Socket::Exception. 
		 */
		class Address
		{
		 public:
			explicit Address(unsigned int inPort=0, const string& inName="localhost");
			
			//! Return host name.
			const string& getHostName() const {return mHostName;}
			//! Return IP address.
			const string& getIPAddress() const {return mIPAddress;}
			//! Return port number.
			unsigned int getPortNumber() const {return mPortNumber;}
			
		 protected:
			unsigned int mPortNumber; //!< socket port number
			string mIPAddress; //!< socket IP address
			string mHostName; //!< host name
			
		};

	} // end of Socket namespace

} // end of PACC namespace

#endif  // PACC_Socket_Address_hpp_



