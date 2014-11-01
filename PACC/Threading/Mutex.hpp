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
 * \file PACC/Threading/Mutex.hpp
 * \brief Class definition for the portable mutual exclusion lock.
 * \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.16 $
 * $Date: 2007/01/23 21:27:47 $
 */

#ifndef PACC_Threading_Mutex_hpp_
#define PACC_Threading_Mutex_hpp_

#include "PACC/Threading/Exception.hpp"

namespace PACC { 
	
	namespace Threading {
		
		/*! \brief Mutual exclusion for thread synchronization.
		\author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
		\ingroup Threading
		
		This class incapsulates a mutex with classic Mutex::lock, Mutex::unlock, and Mutex::tryLock methods. A thread that already has acquired the mutex should not lock it again because this could lead to a deadlock. A thread should only unlock a mutex that is has lock previously (the mutex is not recursive). Otherwise the behavior of the mutex is undetermined (OS specific). 
		
		This class should be compatible with any Unix that supports POSIX threads, as well as any version of Windows. It has been tested under Linux, MacOS X and Windows 2000/XP.
		*/
		class Mutex {
			public:
			Mutex(void);
			~Mutex(void);
			
			void lock(void) const;
			bool tryLock(void) const;
			void unlock(void) const;
			
			protected:
			void* mMutex; //!< Opaque structure of native mutex
			
			private:
			//! restrict (disable) copy constructor.
			Mutex(const Mutex&);
			//! restrict (disable) assignment operator.
			void operator=(const Mutex&);
			
		};
		
	} // end of Threading namespace
	
} // end of PACC namespace

#endif // PACC_Threading_Mutex_hpp_
