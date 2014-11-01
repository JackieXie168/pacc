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
 * \file PACC/SVG/Canvas.hpp
 * \brief Class definition for the SVG drawing canvas.
 * \author Marc Parizeau and Michel Fortin, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.9 $
 * $Date: 2005/09/17 03:50:09 $
 */

#ifndef PACC_SVG_Canvas_hpp_
#define PACC_SVG_Canvas_hpp_

#include "SVG/Frame.hpp"
#include "Socket/Address.hpp"
#include "Socket/Cafe.hpp"

namespace PACC {
	
	namespace SVG {
		
		using namespace std;
		
		/*!\brief Specialized frame to display on screen graphics.
		* \ingroup SVG
		*
		* A canvas opens a TCP/IP connection with a running viewer 
		* application on the network. By default it connects to the localhost at port 61250.
		*
		* The viewer opens a window showing the canvas. Changes made to the
		* canvas are updated in real-time on the viewer side.
		*/
		class Canvas : public Frame {
			public:
			//! Pop canvas on viewer at address \c inAddress:inPort with title \c inTitle and size \c inSize.
			Canvas(const string &inTitle, const Size &inSize, const string &inAddress="localhost", unsigned int inPort=61250) : Frame(Point(0, 0), inSize), mAddress(inAddress), mPort(inPort), mSocket(0) {
				initCanvas(inTitle);
			}
			//! Pop canvas on viewer at address \c inAddress:inPort with title \c inTitle, size \c inSize, and style \c inStyle.
			Canvas(const string &inTitle, const Size &inSize, const Style &inStyle, const string &inAddress="localhost", unsigned int inPort=61250) : Frame(Point(0, 0), inSize, inStyle), mAddress(inAddress), mPort(inPort), mSocket(0) {
				initCanvas(inTitle);
			}        
			//! Pop canvas on viewer at address \c inAddress:inPort with title \c inTitle and using frame \c inFrame.
			Canvas(const string &inTitle, const Frame &inFrame, const string &inAddress="localhost", unsigned int inPort=61250) : Frame(inFrame), mAddress(inAddress), mPort(inPort), mSocket(0) {
				initCanvas(inTitle);
			}
			//! Delete canvas.
			~Canvas(void) {if(mSocket) delete mSocket;}
			
			//! Assign frame \c inFrame to this canvas.
			Canvas &operator=(const Frame& inFrame);
			//! Insert graphic primitive \c inGraphic into this canvas.
			Canvas &operator<<(const Primitive& inGraphic);
			
			//! Remove all primitives in this Canvas.
			void clear(void);
			
			//! Return title of this canvas.
			string getTitle() const;
			//! Set title of this canvas.
			void setTitle(const string& inTitle);
			
			//! Wait up to \c inMaxDelay seconds for the user to click a mouse button.
			Point waitForClick(int &outButtonClicked, double inMaxDelay=0) const;
			
			private:
			string mAddress; //<! The IP address of the viewer.
			unsigned int mPort; //!< The port number of the viewer.
			Socket::Cafe *mSocket; //<! The network socket of this canvas.
			string mWinID; //<! The window ID of this canvas.
			
			Point getOrigin() const;
			void setOrigin(const Point &);
			void setOrigin(float, float);
			
			//! Initialize canvas with title \c inTitle.
			void initCanvas(const string& inTitle);
			//! Send canvas as serialized %SVG code.
			void updateViewer(void) const;
		};
		
	} // end of namespace SVG
	
} // end of namespace PACC

#endif // PACC_SVG_Canvas_hpp_
