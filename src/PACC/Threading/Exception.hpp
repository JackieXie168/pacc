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
 * \file PACC/Threading/Exception.hpp
 * \brief Class definition for the threading exception.
 * \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.15 $
 * $Date: 2008/10/29 21:53:09 $
 */

#ifndef PACC_Threading_Exception_hpp_
#define PACC_Threading_Exception_hpp_

#include <stdexcept>
#include <string>

namespace PACC { 
	
	using namespace std;
	
	namespace Threading {
		
		//! Supported Multithread error codes.
		enum Error {
			eMutexNotOwned, //!< Mutex not own by calling thread
			eWouldDeadLock, //!< Operation would produce a dead lock.
			eRunning, //!< Thread is already running
			eOtherError //!< Any other OS specific error.
		};
		
		/*!
		\brief Multithread exception.
		 \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
		 \ingroup Threading
		 
		 This class encapsulates most interesting runtime errors that can happen while using multithreading operations.
		 */
		class Exception : public runtime_error {
			public:
			//! Construct with native error code.
			explicit Exception(int inNativeCode, const string& inMessage)
			: runtime_error(inMessage), mCode(convertNativeError(inNativeCode)), mNativeCode(inNativeCode) {}
			//! Construct with multithreading error code.
			explicit Exception(Error inCode, const string& inMessage) : runtime_error(inMessage), mCode(inCode), mNativeCode(0) {}
			
			static Error convertNativeError(int inError);
			
			//! Return error code.
			int getErrorCode() {return mCode;}
			
			string getMessage() const throw();
			
			protected:
			Error mCode; //!< Portable error code.
			int mNativeCode; //!< Native error code.
			
		};
		
	} // end of Threading namespace
	
} // end of PACC namespace

#endif  // PACC_Threading_Exception_hpp_
