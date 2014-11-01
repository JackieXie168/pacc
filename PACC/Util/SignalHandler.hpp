/*
 *  Portable Agile C++ Classes (PACC)
 *  Copyright (C) 2004 by Marc Parizeau
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
 * \file PACC/Util/SignalHandler.hpp
 * \brief Class definition for the portable signal handler.
 * \author Marc Dubreuil and Christian Gagne, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.6 $
 * $Date: 2006/01/16 00:51:44 $
 */

#ifndef PACC_SIGNALHANDLER_HPP
#define PACC_SIGNALHANDLER_HPP

#include <stack>
#include <signal.h>

namespace PACC {
	
	using namespace std;
	
	/*!
	 *  \brief Portable signal handling.
	 *  \author Marc Dubreuil, Christian Gagne and Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
	 *  \ingroup Util
	 *
	 *  This class can be used to handle common signals. To define custom handlers,
	 *  it should be subclassed and virtual method SignalHandler::main should be 
	 *  overloaded. For example, the following code snippet defines a custom 
	 *  handler for signal SignalType::eSigInt (Ctl-C):
\verbatim
class SigIntHandler : public SignalHandler {
 public:
    SigIntHandler(void) {setCustomAction(eSigInt);}

    virtual void main(SignalType inType) {
        switch(inType) {
           case eSigInt: 
             cout << "My custom handler!" << endl;
             exit(-1);
           default:
             cout << "Unsupported signal!" << endl;
             break;
        }
	}
};
\endverbatim
Furthermore, for each signal type, a stack of actions can be managed using methods SignalHandler::pushAction and SignalHandler::popAction.
	 */
	class SignalHandler {
	 public:
		//! Enumeration of supported signal types.
		enum SignalType {
			eSigAbrt=0,  //!< Abort (ANSI).
			eSigFPE,     //!< Floating-point exception (ANSI).
			eSigIll,     //!< Illegal instruction (ANSI).
			eSigInt,     //!< Interrupt (ANSI).
			eSigSegV,    //!< Segmentation violation (ANSI).
			eSigTerm,    //!< Termination (ANSI).
			eSigAlrm,    //!< Alarm clock (POSIX).
			eSigChld,    //!< Child status has changed (POSIX).
			eSigCont,    //!< Continue (POSIX).
			eSigHUp,     //!< Hangup (POSIX).
			eSigKill,    //!< Kill, unblockable (POSIX).
			eSigPipe,    //!< Broken pipe (POSIX).
			eSigQuit,    //!< Quit (POSIX).
			eSigStop,    //!< Stop, unblockable (POSIX).
			eSigTStp,    //!< Keyboard stop (POSIX).
			eSigTrap,    //!< Trace trap (POSIX).
			eSigTTin,    //!< Background read from tty (POSIX).
			eSigTTou,    //!< Background write to tty (POSIX).
			eSigUsr1,    //!< User-defined signal 1 (POSIX).
			eSigUsr2,    //!< User-defined signal 2 (POSIX).
			NSIGNALS
		};
		
		//! Construct this signal handler.
		SignalHandler(void) {}
		
		//! delete this signal handler.
		virtual ~SignalHandler(void) {}
		
		/*! \brief Main function of signal handler.
			
			This pure virtual method must be overloaded in a subclass in order to
			define custom signal actions. Argument \c inType specifies the signal
			type that must be processed for the current pending signal.
		 */
		virtual void main(SignalType inType) = 0;
		
		//! Pop previous action for signals of type \c inType.
		static void popAction(SignalType inType);
		
		//! Push current action for signals of type \c inType.
		static void pushAction(SignalType inType);
		
		//! Make this signal handler process future signals of type \c inType.
		void setCustomAction(SignalType inType);
		
		//! Make the default system action process future signals of type \c inType.
		static void setDefaultAction(SignalType inType);
		
		//! Specifies that future signals of type \c inType should be ignored.
		static void setIgnoreAction(SignalType inType);
		
	 protected:		
		//! Convert native signal number into a portable signal type.
		static SignalType convertFromNativeSignal(int inNativeNumber);
		
		//! Convert a portable signal type into a native signal number.
		static int convertToNativeSignal(SignalType inType);
		
	 private:
		//! Pointer to signal handler function.
		typedef void (*HandlerPointer) (int);			

		//! Action structure (pointer to either internal or external handler).
		struct Action {
			SignalHandler* mHandler;
			HandlerPointer mFunc;
			Action(SignalHandler* inHandler, HandlerPointer inFunc) : mHandler(inHandler), mFunc(inFunc) {}
		};
		
		static stack<Action> smActions[NSIGNALS]; //!< Table of signal handler stacks.
		
		//! Run the current custom action for native signal \c inSignal.
		static void runAction(int inSignal);
	};
	
} // end of PACC namespace

#endif
