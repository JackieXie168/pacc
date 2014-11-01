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
 * \file PACC/Socket/TCPServer.hpp
 * \brief Class definition for the portable multithreaded %TCP server.
 * \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.38 $
 * $Date: 2007/01/23 21:27:47 $
 */

#ifndef PACC_Socket_TCPServer_hpp_
#define PACC_Socket_TCPServer_hpp_

#include "PACC/Socket/TCP.hpp"
#include "PACC/Threading/Thread.hpp"
#include <vector>

namespace PACC { 
	
	namespace Socket {
		
		class TCPServer;
		
		/*! \brief Portable server thread.
		\author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
		\ingroup Socket
		
		This class defines a specialized server thread that accepts connections and process them with calls to the TCPServer::main method of its parent server. The server thread runs immediately after object initialization; there is no need to call method Thread::Run.
		
		The user should not be considered with this class.
		*/
		class ServerThread : public Threading::Thread {
		 public:
			//! Construct thread and link to server \c inServer.
			ServerThread(Socket::TCPServer* inServer, double inMaxHaltDelay) : mServer(inServer), mMaxHaltDelay(inMaxHaltDelay) {run();}
			//! delete thread.
			~ServerThread(void) {wait();}
			
			bool shouldTerminate(void) const;
			
		 protected:
			Socket::TCPServer* mServer; //!< Pointer to parent server
			double mMaxHaltDelay; //!< Maximum delay for honoring halt requests
			
			void main(void);
		};
		
		/*! \brief Portable multithreaded %TCP server.
			\author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
			\ingroup Socket
			
			This class defines an abstract multithreaded %TCP server that can bind and listen to a given port using a queue of pending connection, and process these connections using a pool of pre-allocated threads. Its \c main method needs to be overloaded in order to specify the server's function. Method \c TCPServer::run is used to launch and initialize the thread pool which will process incomming connection through calls to method \c TCPServer::main. A running server may be halted through a call to method \c TCPServer::halt. 
			
			Any error during initialization raises a Socket::Exception. Exceptions during connections are first reported through std::cerr, and then ignored.
			*/
		class TCPServer : protected TCP, private Threading::Mutex
		{
		 public:
			//! Construct an uninitialized default server.
			TCPServer(void);
			
			//! Construct a server that binds to port \c inPortNumber with a queue of \c inMinPending connections.
			TCPServer(unsigned int inPortNumber, unsigned int inMinPending=10);
			
			//! Delete the server thread pool.
			virtual ~TCPServer(void);
			
			//! Set default server options.
			void setDefaultOptions(void);
			
			//! Bind server to port number \c inPortNumber.
			void bind(unsigned int inPortNumber) {Port::bind(inPortNumber);}
			
			//! Close the server socket.
			void close(void) {Port::close();}
			
			//! Listen for at least \c inMinPending pending connections.
			void listen(unsigned int inMinPending) {Port::listen(inMinPending);}
			
			//! Stop accepting incomming connections.
			void halt(void);
			
			//! Open a new socket.
			void open(void) {Port::open();}
			
			//! Start accepting incomming connections.
			void run(unsigned int inThreads, double inMaxHaltDelay=1);
			
			//! Wait for server termination.
			void wait(void);
			
		 protected:
			vector<ServerThread*> mThreadPool; //!< Pool of threads pointers

			/*! \brief Main function of server.
				
				Method should start by constructing an adequate protocol for socket descriptor \c inDescriptor. For instance, to make an echo server for the TCP protocol:
\code
using namespace std;
using namespace PACC;
class EchoTCP : public Socket::TCPServer {
 public:
	EchoTCP(unsigned int inPort, unsigned int inMinPending) 
	 : Socket::TCPServer(inPort, inMinPending) {}
	~EchoTCP(void) {wait();}
				
	void main(int inDescriptor, const ServerThread* inThread) {
		try {
			Socket::TCP lSocket(inDescriptor);
			lSocket.setSockOpt(eRecvTimeOut, 10);
			lSocket.setSockOpt(eSendTimeOut, 1.5);
			// loop until thread receives a cancellation request
			while(!inThread->shouldTerminate()) {
				string lMessage;
				// receive message from client
				lSocket.receiveMessage(lMessage);
				// echo message to client
				lSocket.sendMessage(lMessage);
			}
		} catch(const Socket::Exception& inError) {
			// output error message and exit
			cerr << inError.getMessage() << endl;
		}
	}
};
\endcode
				In this example, the server constructs a TCP socket, sets time outs of respectivelly 10 and 1.5 seconds for receives and sends, and enters a loop where every received message is echoed to the client. This loop will exit either when method ServerThread::shouldTerminate returns true (typically after the server receives a halt request), when the client closes the connection (exception Socket::eConnectionClosed will be raised), or when a time out expires (exception Socket::eTimeOut will be raised). Note that the socket for this connection will be closed automatically by its destructor when control exits the try-clause.
				*/
			virtual void main(int inDescriptor, const ServerThread* inThread) = 0;
			
			friend class ServerThread;
			
		};
		
	} // end of Socket namespace
	
} // end of PACC namespace

#endif  // PACC_Socket_TCPServer_hpp_
