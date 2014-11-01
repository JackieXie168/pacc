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
 * \file PACC/Socket/TCPServer.cpp
 * \brief Class methods for the portable %TCP server.
 * \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.41 $
 * $Date: 2007/01/23 21:27:47 $
 */

#include "PACC/Socket/TCPServer.hpp"
#include "PACC/Util/Assert.hpp"
#include <iostream>

using namespace std;
using namespace PACC;

//! Process incomming connections.
void Socket::ServerThread::main(void)
{
	// process connections until thread cancellation
	while(!mCancel) {
		// acquire right to accept a connection
		mServer->lock();
		// wait either for cancellation request or pending connection 
		while(!mCancel && !mServer->waitForActivity(mMaxHaltDelay));
		if(!mCancel) {
			// accept pending connection
			int lDescriptor = mServer->accept();
			// release right to accept connection
			mServer->unlock();
			try {
				// call server main in order to process connection
				mServer->main(lDescriptor, this);
			} catch(const Exception& inError) {
				// report any error and ignore
				cerr << inError.getMessage() << endl;
			}
		} else {
			// release right to accept connection
			mServer->unlock();
		}
	}
}

/*! Return whether the server thread should terminated its current connection early.

This method will respond true whenever the running thread has received a cancellation request. Such a request results from a call to method TCPServer::halt. 

\attention This method should never be called with the thread mutex LOCKED BY THE CALLER. Otherwise, it will deadlock!
*/
bool Socket::ServerThread::shouldTerminate(void) const
{
	lock();
	bool lResult = mCancel;
	unlock();
	return lResult;
}

/*!
Upon return, the server is not binded to any port, nor listening to any connections. To initialize such a server the user must first call methods TCPServer::bind and TCPServer::listen before calling TCPServer::run. This is the power user constructor!

\attention If the user runs the server without binding and listening to a port, it will never accept any connection nor raise any error. Also note that this method calls the TCPServer::setDefaultOptions method.
*/
Socket::TCPServer::TCPServer(void)
{
	setDefaultOptions();
}

/*!
Upon return, the server is binded and listening to port \c inPortNumber using a queue of \c inMinPending pending connections. Note that the exact length of this queue may be operating system dependent (experience has shown that it is usually longer than specified on macosx and linux). For the server to start accepting these connections, the user must call method TCPServer::run. 

Any error raises a Socket::Exception (for example, if the requested port number is unavailable).
*/
Socket::TCPServer::TCPServer(unsigned int inPortNumber, unsigned int inMinPending)
{
	setDefaultOptions();
	Port::bind(inPortNumber);
	Port::listen(inMinPending);
}

/*!
Assuming that the caller has halted the server (using TCPServer::halt) and waited for the server threads to complete any pending connections (using TCPServer::wait), this method deletes the allocated thread pool. In debug mode, if any thread is found still running, the method aborts the application with a descriptive fatal error message.

A good strategy is to always define a destructor in the derived server class, and to make this destructor waits for server termination. For instance:
\code
class MyServer : public Socket::TCPServer {
	...
	~MyServer(void) {wait();}
	...
};
\endcode

\attention If the destructor in the derived server class (e.g. MyServer above) does not wait for thread termination, the potential hazardous situation is that the runtime system will have deleted all of its members before calling this destructor (in C++, class destructors are called in reversed sequence). Thus, a still running server thread could access deleted members with unpredictable end unexpected results. Moreover, if a new connection is accepted in the meantime, then the running thread will crash the application with a cryptic "pure virtual method called" message that results from a call to a main method that no longer exists in this object. So beware! 
*/
Socket::TCPServer::~TCPServer(void)
{
	// delete every thread after making sure that they have stopped running
	for(unsigned int i = 0; i < mThreadPool.size(); ++i) {
		PACC_AssertM(!mThreadPool[i]->isRunning(), "Destructor called without first halting the server and waiting for the threads to terminate. Please correct the situation because it is potentially very hazardous!");
		delete mThreadPool[i];
	}
	mThreadPool.clear();
}

/*!
This method requests cancellation for every allocated server thread in order to make them stop accepting new connections. Threads termination is asynchronous; they terminate either after they complete their current connection or after the \c inMaxHaltDelay delay that was specified in the call to TCPServer::run. 

In order to minimize halting delay, whenever it is safe to terminate a connection from within TCPServer::main, the user should make calls to method ServerThread::shouldTerminate to determine whether it should close the connection and terminate execution early.
*/
void Socket::TCPServer::halt(void)
{
	// request cancellation for all threads
	for(unsigned int i = 0; i < mThreadPool.size(); ++i) {
		mThreadPool[i]->lock();
		mThreadPool[i]->cancel();
		mThreadPool[i]->unlock();
	}
}

/*!
Default options are:
<ul>
<li>eReuseAddress = true</li>
<li>eLinger = 10 seconds</li>
</ul>
Any error raises a Socket::Exception.
*/
void Socket::TCPServer::setDefaultOptions()
{
	setSockOpt(eReuseAddress, true);
	setSockOpt(eLinger, 10);
}

/*!
Upon return, this method has added \c inThreads new threads to the server's thread pool. These new threads start accepting incomming connections immediately, and until some thread calls method TCPServer::halt. Incomming connections are processed through calls to virtual function TCPServer::main which needs to be overloaded in a sub-class. Halt requests will be honored at least every \c inMaxHaltDelay seconds (default=1), or after a connection terminates.

This method can be called any number of times to increase the size of the thread pool. 

\attention if the server was constructed using the default constructor, methods TCPServer::bind and TCPServer::listen must be called prior to calling this method. Otherwise, no incomming connection will ever be accepted, nor any error raised. Any error during the initialization of the new threads raises a Socket::Exception.
*/
void Socket::TCPServer::run(unsigned int inThreads, double inMaxHaltDelay)
{
	// allocate new threads
	for(unsigned int i = 0; i < inThreads; ++i) {
		ServerThread* lThread = new ServerThread(this, inMaxHaltDelay);
		mThreadPool.push_back(lThread);
	}
}

/*!
This method will wait for the termination of every server thread. If method TCPServer::halt was previously executed, then the server threads will terminate after the longer of either the time required to complete their current connections or the \c inMaxHaltDelay delay that was specified in the call to TCPServer::run. Otherwise, they will not terminate until another thread calls the TCPServer::halt method.
*/
void Socket::TCPServer::wait(void)
{
	// wait for the termination of every server thread
	for(unsigned int i = 0; i < mThreadPool.size(); ++i) {
		// do not wait for one self!
		if(!mThreadPool[i]->isSelf()) mThreadPool[i]->wait(true);
	}
}

