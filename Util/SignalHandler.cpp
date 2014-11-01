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
 * \author Marc Dubreuil and Christian Gagn&eacute;, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.5 $
 * $Date: 2005/06/02 06:59:17 $
 */

#include "Util/SignalHandler.hpp"
#include <signal.h>
#include <stdexcept>

// define __sighandler_t on MacOSX
#ifndef __sighandler_t
#define __sighandler_t sig_t
#endif

using namespace std;
using namespace PACC;

SignalHandler* SignalHandler::smHandlers[32] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
void* SignalHandler::smSigAction[32] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
void* SignalHandler::smOldSigIgn[32] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

/*!
 *  \brief Call the virtual function of the derived class.
 *  \param inSignalNumber signal number (usually SIGINT, SIGTERM)
 *  \throw runtime_error Invalid signal number or no Handler specified.
 */
void SignalHandler::sighandle(int inSignalNumber)
{
  if(inSignalNumber > 31) throw runtime_error("SignalHandler::sighandle: invalid signal number");
  if(inSignalNumber < 1) return;
  if(smHandlers[inSignalNumber] != NULL) {
    smHandlers[inSignalNumber]->handleSignal(inSignalNumber);
  }
  else throw runtime_error("SignalHandler::sighandle: no Handler specified.");
}


/*!
 *  \brief Get the Handler of the signal
 *  \param inSignalNumber signal number (usually SIGINT, SIGTERM)
 *  \return Signal handler for a signal
 *  \throw runtime_error Invalid signal number.
 */
SignalHandler* SignalHandler::getHandler(int inSignalNumber)
{
  if(inSignalNumber > 31) throw runtime_error("SignalHandler::getHandler: invalid signal number");
  if(inSignalNumber < 1) return NULL;
  return smHandlers[inSignalNumber];
}

/*!
 *  \brief Ignore the receiving signal.
 *  \param inSignalNumber signal number (usually SIGPIPE on POSIX *nix)
 *  \throw runtime_error Invalid signal number.
 */
void SignalHandler::ignoreSignal(int inSignalNumber)
{
  if(inSignalNumber > 31) throw runtime_error("SignalHandler::ignoreSignal: invalid signal number");
  //the signal is not defined under Windows, do nothing
  if(inSignalNumber < 1) return;
  if(smOldSigIgn[inSignalNumber] != NULL) return;
  smOldSigIgn[inSignalNumber] = (void*)signal(inSignalNumber, SIG_IGN);
}

/*!
 *  \brief If the signal is ignored (SIG_IGN)
 *  \param inSignalNumber signal number
 *  \return true if the signal is ignored, or false otherwise.
 *  \throw runtime_error Invalid signal number.
 */
bool SignalHandler::isSignalIgnored(int inSignalNumber)
{
  if(inSignalNumber > 31) throw runtime_error("SignalHandler::isSignalIgnored: invalid signal number");
  if(inSignalNumber < 1) return false;
  if(smOldSigIgn[inSignalNumber] != NULL) return true;
  else return false;
}


/*!
 *  \brief Reactivate the old signal value before ignoreSignal was called.
 *  \param inSignalNumber signal number (usually SIGINT, SIGTERM)
 *  \throw runtime_error Invalid signal number.
 */
void SignalHandler::reactivateSignal(int inSignalNumber)
{
  if(inSignalNumber > 31) throw runtime_error("SignalHandler::reactivateSignal: invalid signal number");
  //the signal is not defined under Windows, do nothing
  if(inSignalNumber < 1) return;

  if(smOldSigIgn[inSignalNumber] != NULL)
  {
#ifdef WIN32
    signal(inSignalNumber, (void(__cdecl*)(int))smOldSigIgn[inSignalNumber]);
#else
    signal(inSignalNumber, (__sighandler_t)smOldSigIgn[inSignalNumber]);
#endif
  }
  else signal(inSignalNumber, SIG_DFL);
  smOldSigIgn[inSignalNumber] = NULL;
}


/*!
 *  \brief Restore the signal old value before handleSignal or ignoreSignal was called.
 *  \param inSignalNumber signal number (usually SIGINT, SIGTERM)
 *  \return the signal handler that was used to catch the signal
 *  \throw runtime_error Invalid signal number.
 */
SignalHandler* SignalHandler::restoreHandler(int inSignalNumber)
{
  if(inSignalNumber > 31) throw runtime_error("SignalHandler::restoreHandler: invalid signal number");
  SignalHandler* outHandler = NULL;
  //the signal is not defined under Windows, do nothing
  if(inSignalNumber < 1) return outHandler;

  if(smSigAction[inSignalNumber] != NULL)
  {
#ifdef WIN32
    signal(inSignalNumber, (void(__cdecl*)(int))smSigAction[inSignalNumber]);
#else
    signal(inSignalNumber, (__sighandler_t)smSigAction[inSignalNumber]);

#endif
    outHandler = smHandlers[inSignalNumber];
    smSigAction[inSignalNumber] = NULL;
  }
  return outHandler;
}


/*!
 *  \brief set the Handle of a signal
 *  \param inSignalNumber signal number (usually SIGINT, SIGTERM)
 *  \return The old signal handler
 *  \throw runtime_error Invalid signal number.
 */
SignalHandler* SignalHandler::setAsHandler(int inSignalNumber)
{
  if(inSignalNumber > 31) throw runtime_error("SignalHandler::setAsHandler: invalid signal number");
  if(inSignalNumber < 1) return NULL;
  return SignalHandler::setHandler(inSignalNumber, this);
}

/*!
 *  \brief Handle a signal with the specified method
 *  \param inSignalNumber signal number (usually SIGINT, SIGTERM)
 *  \param inSigHandler pointer function to the handling function.
 *  \return The old signal handler
 *  \throw runtime_error Invalid signal number.
 */
SignalHandler* SignalHandler::setHandler(int inSignalNumber, SignalHandler* inSigHandler)
{
  if(inSignalNumber > 31) throw runtime_error("SignalHandler::setHandler: invalid signal number");
  //return the old signal handler if present
  SignalHandler* outOldSignalHandler = NULL;

  //the signal is not defined under Windows, do nothing
  if(inSignalNumber < 1)
    return outOldSignalHandler;
  if(inSigHandler == NULL)
    return restoreHandler(inSignalNumber);

  bool lBackupOldSignal = true;
  if(smHandlers[inSignalNumber] != NULL)
  {
    lBackupOldSignal = false;
    outOldSignalHandler = smHandlers[inSignalNumber];
  }
  smHandlers[inSignalNumber] = inSigHandler;
  smSigAction[inSignalNumber] = (void*)signal(inSignalNumber, SignalHandler::sighandle);
  return outOldSignalHandler;
}


