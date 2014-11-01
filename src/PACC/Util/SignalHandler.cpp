/*
 *  Portable Agile C++ Classes (PACC)
 *  Copyright (C) 2004 Marc Parizeau
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
 * \file PACC/Util/SignalHandler.cpp
 * \brief Class methods for the signal handler.
 * \author Marc Dubreuil, Christian Gagn&eacute; and Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.10 $
 * $Date: 2008/04/17 21:16:07 $
 */

#include "PACC/Util/SignalHandler.hpp"
#include "PACC/Util/Assert.hpp"

#include "PACC/config.hpp"
#include <signal.h>
#include <stdexcept>
#include <cstdlib>

using namespace std;
using namespace PACC;

stack<SignalHandler::Action> SignalHandler::smActions[NSIGNALS];

/*!
\throw runtime_error This method throws an exception if called for an unsuported signal. Note that Windows only supports ANSI signals (see SignalHandler::SignalType).
 */
SignalHandler::SignalType SignalHandler::convertFromNativeSignal(int inSignal)
{
	SignalType lType;
	switch(inSignal) {
#ifdef PACC_USE_WINDOWS
		case SIGABRT: lType = eSigAbrt; break;
		case SIGFPE:  lType = eSigFPE; break;
		case SIGILL:  lType = eSigIll; break;
		case SIGINT:  lType = eSigInt; break;
		case SIGSEGV: lType = eSigSegV; break;
		case SIGTERM: lType = eSigTerm; break;
		default:
			throw runtime_error("SignalHandler::convertFromNativeSignal() signal not supported by Windows!");
#else
		case SIGHUP:  lType = eSigHUp; break;
		case SIGINT:  lType = eSigInt; break;
		case SIGQUIT: lType = eSigQuit; break;
		case SIGILL:  lType = eSigIll; break;
		case SIGTRAP: lType = eSigTrap; break;
		case SIGABRT: lType = eSigAbrt; break;
		case SIGFPE:  lType = eSigFPE; break;
		case SIGKILL: lType = eSigKill; break;
		case SIGSEGV: lType = eSigSegV; break;
		case SIGPIPE: lType = eSigPipe; break;
		case SIGALRM: lType = eSigAlrm; break;
		case SIGTERM: lType = eSigTerm; break;
		case SIGSTOP: lType = eSigStop; break;
		case SIGTSTP: lType = eSigTStp; break;
		case SIGCONT: lType = eSigCont; break;
		case SIGCHLD: lType = eSigChld; break;
		case SIGTTIN: lType = eSigTTin; break;
		case SIGTTOU: lType = eSigTTou; break;
		case SIGUSR1: lType = eSigUsr1; break;
		case SIGUSR2: lType = eSigUsr2; break;
		default:
			throw runtime_error("SignalHandler::convertFromNativeSignal() unknown POSIX signal!");
#endif
	}
	return lType;
}

/*!
\throw runtime_error This method throws an exception if called for an unsuported signal. Note that Windows only supports ANSI signals (see SignalHandler::SignalType).
 */
int SignalHandler::convertToNativeSignal(SignalHandler::SignalType inType)
{
	int lSignal;
	switch(inType) {
#ifdef PACC_USE_WINDOWS
		case eSigAbrt: lSignal = SIGABRT; break;
		case eSigFPE:  lSignal = SIGFPE; break;
		case eSigIll:  lSignal = SIGILL; break;
		case eSigInt:  lSignal = SIGINT; break;
		case eSigSegV: lSignal = SIGSEGV; break;
		case eSigTerm: lSignal = SIGTERM; break;
		default:
			throw runtime_error("SignalHandler::convertToNativeSignal() signal not supported by Windows!");
#else
		case eSigHUp:  lSignal = SIGHUP; break;
		case eSigInt:  lSignal = SIGINT; break;
		case eSigQuit: lSignal = SIGQUIT; break;
		case eSigIll:  lSignal = SIGILL; break;
		case eSigTrap: lSignal = SIGTRAP; break;
		case eSigAbrt: lSignal = SIGABRT; break;
		case eSigFPE:  lSignal = SIGFPE; break;
		case eSigKill: lSignal = SIGKILL; break;
		case eSigSegV: lSignal = SIGSEGV; break;
		case eSigPipe: lSignal = SIGPIPE; break;
		case eSigAlrm: lSignal = SIGALRM; break;
		case eSigTerm: lSignal = SIGTERM; break;
		case eSigStop: lSignal = SIGSTOP; break;
		case eSigTStp: lSignal = SIGTSTP; break;
		case eSigCont: lSignal = SIGCONT; break;
		case eSigChld: lSignal = SIGCHLD; break;
		case eSigTTin: lSignal = SIGTTIN; break;
		case eSigTTou: lSignal = SIGTTOU; break;
		case eSigUsr1: lSignal = SIGUSR1; break;
		case eSigUsr2: lSignal = SIGUSR2; break;
		default:
			throw runtime_error("SignalHandler::convertToNativeSignal() unknown POSIX signal!");
#endif
	}
	return lSignal;
}

/*!
This method pops the top element of the action stack and resets to the previous action. It is an error to call this method when the stack is empty. Note that, for any given signal, the first call to one of the set methods (SignalHandler::setCustomAction, SignalHandler::setDefaultAction, or SignalHandler::setIgnoreAction) executes an implicit push of the previous action. A call to this method in that particular situation will thus restore this previous action (usually the default system action).

 \throw runtime_error This method throws an exception if called for an unsuported signal. Note that Windows only supports ANSI signals (see SignalHandler::SignalType).
 */
void SignalHandler::popAction(SignalHandler::SignalType inType)
{
	int lSignal = convertToNativeSignal(inType);
	PACC_AssertM(!smActions[inType].empty(), "popAction() stack is empty!");
	smActions[inType].pop();
	PACC_AssertM(!smActions[inType].empty(), "popAction() stack is empty!");
	HandlerPointer lFunc;
	if(smActions[inType].top().mHandler == 0) {
		// action must be external function
		lFunc = ::signal(lSignal, smActions[inType].top().mFunc);
	} else {
		// action is custom
		lFunc = ::signal(lSignal, runAction);
	}
	PACC_AssertM(lFunc != SIG_ERR, "popAction() internal error: invalid signal") 
}

/*!
This method pushes a copy of the current action onto the action stack. It is an error to call this method prior to setting a first action using either SignalHandler::setCustomAction, SignalHandler::SetDefaultAction, or SignalHandler::setIgnoreAction.
 */
void SignalHandler::pushAction(SignalHandler::SignalType inType)
{
	PACC_AssertM(!smActions[inType].empty(), "pushAction() stack is empty, set an action first!");
	smActions[inType].push(smActions[inType].top());
}

/*!
This method is the low level custom signal handler. It should not be called directly!
 */
void SignalHandler::runAction(int inSignal)
{
	SignalType lType = convertFromNativeSignal(inSignal);
	PACC_AssertM(!smActions[lType].empty(), "runAction() internal error, stack is empty!");
	PACC_AssertM(smActions[lType].top().mHandler != 0, "runAction() internal error: no handler specified!");
	smActions[lType].top().mHandler->main(lType);
}

/*!
When called with an empty action stack, the later is initialized by making an implicit push of the previous action. Otherwise, it does not affect the action stack.
 
\throw runtime_error This method throws an exception if called for an unsuported signal. Note that Windows only supports ANSI signals (see SignalHandler::SignalType).
*/
void SignalHandler::setCustomAction(SignalHandler::SignalType inType)
{
	// set new action
	int lSignal = convertToNativeSignal(inType);
	HandlerPointer lFunc = ::signal(lSignal, runAction);
	PACC_AssertM(lFunc != SIG_ERR, "setCustomAction() internal error: invalid signal");
	// update action stack
	Action lCustom(this, 0);
	if(smActions[inType].empty()) {
		// initialize the action stack
		smActions[inType].push(Action(0, lFunc));
		smActions[inType].push(lCustom);
	} else {
		// overwrite the top action
		smActions[inType].top() = lCustom;
	}
}

/*!
When called with an empty action stack, the later is initialized by making an implicit push of the previous action. Otherwise, it does not affect the action stack.
 
\throw runtime_error This method throws an exception if called for an unsuported signal. Note that Windows only supports ANSI signals (see SignalHandler::SignalType).
 */
void SignalHandler::setDefaultAction(SignalHandler::SignalType inType)
{
	int lSignal = convertToNativeSignal(inType);
	HandlerPointer lFunc = ::signal(lSignal, SIG_DFL);
	PACC_AssertM(lFunc != SIG_ERR, "setDefaultAction() internal error: invalid signal");
	Action lCustom(0, SIG_DFL);
	if(smActions[inType].empty()) {
		// initialize the action stack
		smActions[inType].push(Action(0, lFunc));
		smActions[inType].push(lCustom);
	} else {
		// overwrite the top action
		smActions[inType].top() = lCustom;
	}
}

/*!
When called with an empty action stack, the later is initialized by making an implicit push of the previous action. Otherwise, it does not affect the action stack.
 
\throw runtime_error This method throws an exception if called for an unsuported signal. Note that Windows only supports ANSI signals (see SignalHandler::SignalType).
 */
void SignalHandler::setIgnoreAction(SignalHandler::SignalType inType)
{
	int lSignal = convertToNativeSignal(inType);
	HandlerPointer lFunc = ::signal(lSignal, SIG_IGN);
	PACC_AssertM(lFunc != SIG_ERR, "setIgnoreAction() internal error: invalid signal");
	Action lCustom(0, SIG_IGN);
	if(smActions[inType].empty()) {
		// initialize the action stack
		smActions[inType].push(Action(0, lFunc));
		smActions[inType].push(lCustom);
	} else {
		// overwrite the top action
		smActions[inType].top() = lCustom;
	}
}
