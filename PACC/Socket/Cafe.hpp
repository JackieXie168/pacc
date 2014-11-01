/*
 *  Portable Agile C++ Classes (PACC)
 *  Copyright (C) 2001-2004 by Marc Parizeau and Marc Dubreuil
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
 * \file PACC/Socket/Cafe.hpp
 * \brief Class definition for the cafe protocol.
 * \author Marc Parizeau and Marc Dubreuil, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.28 $
 * $Date: 2007/01/23 21:27:46 $
 */

#ifndef PACC_Socket_Cafe_hpp_
#define PACC_Socket_Cafe_hpp_

#include "PACC/Socket/TCP.hpp"

namespace PACC { 
	
	using namespace std;
	
	namespace Socket {
		
		/*!\brief %Cafe protocol.
		\author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
		\ingroup Socket
		
		This class implements a custom protocol for socket communication. It 
		encapsulates ASCII string messages over %TCP using a header containing a 
		double word signature plus message length information. There are in fact 
		two protocols. The first uses the \c 0xCAFE signature to encapsulate 
		uncompressed messages. In that case, the next double word (4 bytes) 
		contains the message length in network order. The second protocol uses 
		the \c 0xCCAFE signature to encapsulate compressed messages. In this 
		case, the header contains two double words: the first being the 
		compressed message length (in network order), and the second the 
		uncompressed message length (again in network order).
		
		By default, this class does not allow message compression (compression 
		level must be 0). In order to enable compression (levels 1 to 9), it 
		needs to be compiled with variable PACC_ZLIB set, and the executable 
		needs to be linked with the zlib library <http://www.gzip.org/zlib>. 
		Also note that messages will be sent uncompressed whenever compression 
		would result in longer messages.
		
		Any error raises a Socket::Exception. 
		*/
		class Cafe : public TCP {
		 public:
			//! Construct unconnected socket.
			explicit Cafe(void) throw() {}
			
			//! Construct using existing socket descriptor \c inDescriptor.
			Cafe(int inDescriptor) throw() : TCP(inDescriptor) {}
			
			//! Construct socket connected to peer \c inPeer.
			Cafe(const Address& inPeer) : TCP(inPeer) {}
			
			//! Receive string message from connected server using the 0cafe protocol.
			void receiveMessage(string& outMessage);
			
			//! Send string message \c inMessage to connected server using the cafe protocol.
			void sendMessage(const string& inMessage, unsigned int inCompressionLevel = 0);
			
		 protected:
			//! Compress string \c inMessage using compression level \c inCompressionLevel, and return result through string \c outMessage.
			void compress(const string& inMessage, string& outMessage, unsigned int inCompressionLevel);
			
			//! Uncompress string \c ioMessage knowing that the uncompressed message length is \c inUncompressedSize, and return result through string \c ioMessage.
			void uncompress(string& ioMessage, unsigned long inSize);
			
			//! Receive \c inCount bytes from socket.
			void receive(char* inBuffer, unsigned int inCount);
		};
		
	} // end of Socket namespace
	
} // end of PACC namespace

#endif  // PACC_Socket_Cafe_hpp_
