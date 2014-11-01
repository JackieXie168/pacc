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
 * \file PACC/Threading/Thread.hpp
 * \brief Class definition for the portable thread.
 * \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.29 $
 * $Date: 2007/01/23 21:27:47 $
 */

#ifndef PACC_Threading_Thread_hpp_
#define PACC_Threading_Thread_hpp_

#include "PACC/Threading/Condition.hpp"

namespace PACC { 
	
	namespace Threading {
		
		/*! \brief Abstract thread.
		\author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
		\ingroup Threading
		
		This class incapsulates an abstract cross-platform thread. It should be subclassed in order to define virtual member function Thread::main which is called soon after thread creation (see Thread::run). The thread terminates when main returns or after a call to Thread::cancel is honored by a subsequent cancellation point. A cancellation point can be created by a call to Thread::makeCancellationPoint.
		
		This class should be compatible with any flavour of Unix that supports POSIX threads. It should also be compatible will any version of Windows that is supported by class Condition (refer to its documentation for more details). It has been tested under Linux, MacOS X, and Windows 2000/XP. 
		*/
		class Thread : public Condition {
		 public:
			Thread(void);
			virtual ~Thread(void);
			
			void cancel(void);
			bool isRunning(void) const;
			bool isSelf(void) const;
			static void sleep(double inSeconds);
			void run(void);
			void wait(bool inLock=true);
			
			protected:
			void* mThread; //!< Opaque structure of native thread.
			bool mCancel; //!< Should be canceled flag.
			bool mRunning; //!< Is running flag
			
			/*! \brief Implements main procedure of thread.
				
				This virtual method must be overloaded in a derived class in order to implement the main procedure of this thread. As much as possible, and whenever it is safe to cancel execution, this method should call Thread::makeCancellationPoint so that this thread can be canceled by other threads (including deleted by the main thread). Otherwise, any thread cancellation (or deletion) will be postponed until after main returns. For instance, if main's implementation requires a loop, a cancellation point should be included at least once for each iteration.
				*/
			virtual void main(void) = 0;
			
			void makeCancellationPoint(void);
			
		 private:
			static void* startup(void* inThread);
			
			//! restrict (disable) copy constructor.
			Thread(const Thread&);
			//! restrict (disable) assignment operator.
			void operator=(const Thread&);
		};
		
	} // end of Threading namespace
	
} // end of PACC namespace

#endif // PACC_Threading_Thread_hpp_
