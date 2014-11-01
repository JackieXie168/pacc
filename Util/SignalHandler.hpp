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
 * $Revision: 1.3 $
 * $Date: 2005/06/02 06:59:17 $
 */

#ifndef PACC_SIGNALHANDLER_HPP
#define PACC_SIGNALHANDLER_HPP

#include <vector>
#include <signal.h>

namespace PACC {
   
   using namespace std;

   /*!
    *  \class SignalHandler PACC/Util/SignalHandler.hpp "PACC/Util/SignalHandler.hpp"
    *  \brief Portable signal handling.
    *  \author Marc Dubreuil and Christian Gagne, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
    *  \ingroup Util
    *
    *  This class handle common signals (usually SIGINT and SIGTERM) to cleanly
    *  end a program.  This class should be public inherited.  To specify
    *  which method should be used when receiving a signal, see setHandler.
    *  To ignore some kind of signal, use ignoreSignal.  When specifying
    *  a signal (with setHandler), as long as the root process
    *  is alive, the signals will be changed to the new value.  If it is not desired,
    *  use restoreHandler to its old value.
    */
   class SignalHandler {
public:
#ifdef WIN32
      //negative numbers does not work under Windows.  Only ANSI code.
      enum SignalNumber {
         eSigHUp  = -1,      /* Hangup (POSIX).                  */
         eSigInt  = SIGINT,  /* Interrupt (ANSI).                */
         eSigQuit = -3,      /* Quit (POSIX).                    */
         eSigIll  = SIGILL,  /* Illegal instruction (ANSI).      */
         eSigTrap = -5,      /* Trace trap (POSIX).              */
         eSigAbrt = SIGABRT, /* Abort (ANSI).                    */
         eSigFPE  = SIGFPE,  /* Floating-point exception (ANSI). */
         eSigKill = -9,      /* Kill, unblockable (POSIX).       */
         eSigUsr1 = -10,     /* User-defined signal 1 (POSIX).   */
         eSigSegV = SIGSEGV, /* Segmentation violation (ANSI).   */
         eSigUsr2 = -12,     /* User-defined signal 2 (POSIX).   */
         eSigPipe = -13,     /* Broken pipe (POSIX).             */
         eSigAlrm = -14,     /* Alarm clock (POSIX).             */
         eSigTerm = SIGTERM, /* Termination (ANSI).              */
         eSigChld = -17,     /* Child status has changed (POSIX).*/
         eSigCont = -18,     /* Continue (POSIX).                */
         eSigStop = -19,     /* Stop, unblockable (POSIX).       */
         eSigTStp = -20,     /* Keyboard stop (POSIX).           */
         eSigTTin = -21,     /* Background read from tty (POSIX).*/
         eSigTTou = -22      /* Background write to tty (POSIX). */
      };
#else
      enum SignalNumber{
         eSigHUp  = SIGHUP,   /* Hangup (POSIX).                  */
         eSigInt  = SIGINT,   /* Interrupt (ANSI).                */
         eSigQuit = SIGQUIT,  /* Quit (POSIX).                    */
         eSigIll  = SIGILL,   /* Illegal instruction (ANSI).      */
         eSigTrap = SIGTRAP,  /* Trace trap (POSIX).              */
         eSigAbrt = SIGABRT,  /* Abort (ANSI).                    */
         eSigFPE  = SIGFPE,   /* Floating-point exception (ANSI). */
         eSigKill = SIGKILL,  /* Kill, unblockable (POSIX).       */
         eSigUsr1 = SIGUSR1,  /* User-defined signal 1 (POSIX).   */
         eSigSegV = SIGSEGV,  /* Segmentation violation (ANSI).   */
         eSigUsr2 = SIGUSR2,  /* User-defined signal 2 (POSIX).   */
         eSigPipe = SIGPIPE,  /* Broken pipe (POSIX).             */
         eSigAlrm = SIGALRM,  /* Alarm clock (POSIX).             */
         eSigTerm = SIGTERM,  /* Termination (ANSI).              */
         eSigChld = SIGCHLD,  /* Child status has changed (POSIX).*/
         eSigCont = SIGCONT,  /* Continue (POSIX).                */
         eSigStop = SIGSTOP,  /* Stop, unblockable (POSIX).       */
         eSigTStp = SIGTSTP,  /* Keyboard stop (POSIX).           */
         eSigTTin = SIGTTIN,  /* Background read from tty (POSIX).*/
         eSigTTou = SIGTTOU   /* Background write to tty (POSIX). */
      };
#endif

      //! Construct default signal handler.
      SignalHandler() {}
      //! delete signal handler.
      virtual ~SignalHandler() {}
      
      static SignalHandler* getHandler(int inSignalNumber);
      virtual void handleSignal(int inSignalNumber) = 0;
      static void ignoreSignal(int inSignalNumber);
      static bool isSignalIgnored(int inSignalNumber);
      static void reactivateSignal(int inSignalNumber);
      static SignalHandler* restoreHandler(int inSignalNumber);
      SignalHandler* setAsHandler(int inSignalNumber);
      static SignalHandler* setHandler(int inSignalNumber, SignalHandler*);
      
protected:
      static void sighandle(int);
      
private:
      static SignalHandler* smHandlers[32];
      
      static void* smSigAction[32]; //!< static (Old) signal action.
      static void* smOldSigIgn[32]; //!< static (Old) signal ignore.
   };
   
} // end of PACC namespace

#endif
