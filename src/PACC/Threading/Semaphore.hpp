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
 * \file PACC/Threading/Semaphore.hpp
 * \brief Class definition for the portable counting semaphore.
 * \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.18 $
 * $Date: 2007/01/23 21:27:47 $
 */

#ifndef PACC_Threading_Semaphore_hpp_
#define PACC_Threading_Semaphore_hpp_

#include "PACC/Threading/Mutex.hpp"
#include "PACC/Threading/Condition.hpp"

namespace PACC { 
	
	namespace Threading {
		
		/*! \brief %Semaphore for thread synchronization.
		\author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
		\ingroup Threading
		
		This class incapsulates a counting semaphore with classic Semaphore::post, Semaphore::tryWait, and Semaphore::wait methods. The semaphore is initialized with a count of \c inCount ressources (see constructor). A post increments the count while a wait decrements it. A count of 0 means that no more ressources are available. Subsequent calls to wait will block until future post releases some of the allocated ressources. When several threads have blocked because of exhausted ressources, the order in which they will be released is undetermined (FIFO should not be assumed).
		
		This class does not contain any OS dependant code. However, it is built over classes Condition and Mutex which may not be fully cross-platform (see documentation of these classes for more details). 
		*/
		class Semaphore : public Condition {
			public:
			//! Construct semaphore with initial count \c inCount.
			Semaphore(unsigned int inCount=0) : mCount(inCount), mWaiters(0) {}
			
			//! Return count of semaphore (semaphore should be locked prior to call).
			int getCount(void) const {return mCount;}
			void post(bool inLock=true);
			//! Set count of semaphore (semaphore should be locked prior to call).
			void setCount(unsigned int inCount) {mCount = inCount;}
			bool tryWait(bool inLock=true);
			bool wait(double inMaxTime=0, bool inLock=true);
			
			protected:
			unsigned int mCount; //!< Current count of semaphore
			unsigned int mWaiters; //!< Current number of waiting threads
		};
		
	} // end of Threading namespace
	
} // end of PACC namespace

#endif // PACC_Threading_Semaphore_hpp_
