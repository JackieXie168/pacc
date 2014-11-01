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
 * \file PACC/Threading/Condition.hpp
 * \brief Class definition for the portable condition.
 * \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.19 $
 * $Date: 2007/01/23 21:27:47 $
 */

#ifndef PACC_Threading_Condition_hpp_
#define PACC_Threading_Condition_hpp_

#include "PACC/Threading/Mutex.hpp"

namespace PACC { 
	
	namespace Threading {
		
		/*! \brief %Condition for thread synchronization.
		\author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
		\ingroup Threading
		
		This class incapsulates a cross-platform POSIX condition with classic Condition::broadcast, Condition::signal, and Condition::wait methods. It should be compatible with any flavour of Unix that supports POSIX threads. It is also compatible with any version of Windows that support the SignalObjectAndWait method (introduced with NT4). 
		
		This class has been tested under Linux, MacOS X and Windows 2000/XP.
		*/
		class Condition : public Mutex {
			public:
			Condition(void);
			~Condition(void);
			
			void broadcast(void) const;
			void signal(void) const;
			bool wait(double inMaxTime=0) const;
			
			protected:
			void* mCondition; //!< Opaque structure of native condition
			
			private:
			//! restrict (disable) copy constructor.
			Condition(const Condition&);
			//! restrict (disable) assignment operator.
			void operator=(const Condition&);
		};
		
	} // end of Threading namespace
	
} // end of PACC namespace

#endif // PACC_Threading_Condition_hpp_
