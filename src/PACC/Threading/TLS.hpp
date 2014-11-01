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
 * \file PACC/Threading/TLS.hpp
 * \brief Class definition for the portable Thread Local Storage (%TLS).
 * \author Guillaume Mayer, ForwardSim Inc.
 * \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.2 $
 * $Date: 2008/10/29 21:54:30 $
 */

#ifndef PACC_Threading_TLS_hpp_
#define PACC_Threading_TLS_hpp_

namespace PACC { 
	
	namespace Threading {
		
		/*! \brief Thread Local Storage.
		\author Guillaume Mayer, ForwardSim Inc.
		\author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
		\ingroup Threading
		
		This class incapsulates the thread local storage mechanisms found in Windows or POSIX pthread.
		*/
		class TLS {
		 public:
			//! Construct local storage.
			TLS(void);
			
			//! Delete local storage.
			~TLS(void);
			
			//! Set local storage to object \c inObject.
			void setValue(void *inObject);
			
			//! Retrieve local storage object.
			void* getValue(void);
			
		 protected:
			void* mIndex; //!< Opaque structure of native index.
			
		 private:
			//! restrict (disable) copy constructor.
			TLS(const TLS&);
			//! restrict (disable) assignment operator.
			void operator=(const TLS&);
		};
		
	} // end of Threading namespace
	
} // end of PACC namespace

#endif // PACC_Threading_TLS_hpp_
