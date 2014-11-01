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
 * $Revision: 1.19 $
 * $Date: 2008/04/17 21:26:51 $
 */

#ifndef PACC_SVG_Canvas_hpp_
#define PACC_SVG_Canvas_hpp_

#include "PACC/SVG/Document.hpp"
#include "PACC/Socket/Cafe.hpp"

namespace PACC {
	
	namespace SVG {
		
		using namespace std;
		
		/*!\brief Specialized document to display on screen graphics.
		* \ingroup SVG
		*
		* A canvas opens a TCP/IP connection with a running viewer 
		* application on the network. By default it connects to the localhost at port 61250.
		*
		* The viewer opens a window showing the canvas. Changes made to the
		* canvas are updated in real-time on the viewer side.
		*/
		class Canvas : public Document, protected Socket::Cafe {
		 public:
			//! Pop canvas on viewer at address \c inHostPort with title \c inTitle and size \c inSize.
			Canvas(const string& inTitle, const Size& inSize, const string& inHostPort="localhost:61250") : Document(inTitle, inSize), Socket::Cafe(inHostPort) {
				initCanvas();
			}
			
			//! Pop canvas on viewer at address \c inAddress:inPort with title \c inTitle, size \c inSize, and style \c inStyle.
			Canvas(const string& inTitle, const Size& inSize, const Style& inStyle, const string& inHostPort="localhost:61250") : Document(inTitle, inSize, inStyle), Socket::Cafe(inHostPort) {
				initCanvas();
			}
			
			//! Pop canvas on viewer at address \c inAddress:inPort with title \c inTitle and using frame \c inFrame.
			Canvas(const string& inTitle, const Frame& inFrame, const string& inHostPort="localhost:61250") : Document(inTitle, inFrame), Socket::Cafe(inHostPort) {
				initCanvas();
			}
			
			//! Pop canvas on viewer at address \c inAddress:inPort using document \c inDocument.
			Canvas(const Document& inDocument, const string& inHostPort="localhost:61250") : Document(inDocument), Socket::Cafe(inHostPort) {
				initCanvas();
			}
			
			//! Assign frame \c inFrame to this canvas.
			Canvas& operator=(const Frame& inFrame) {
				Frame::operator=(inFrame);
				updateViewer();
				return *this;
			}
			
			//! Insert graphic primitive \c inGraphic into this canvas.
			Canvas& operator<<(const Primitive& inGraphic) {
				Group::operator<<(inGraphic); 
				updateViewer(); 
				return *this;
			}
			
			//! Erase all drwing elements.
			void clear(void) {Group::clear(); updateViewer();}
			
			//! Set size of canvas to size \c inSize.
			void setSize(const Size& inSize) {
				setSize(inSize.width, inSize.height);
			}
			
			//! Set frame size to width \c inwidth and height \c inHeight.
			void setSize(double inWidth, double inHeight) {
				setAttribute("width", inWidth);
				setAttribute("height", inHeight);
				updateViewer();
			}	
			
			//! Wait up to \c inMaxDelay seconds for the user to click a mouse button.
			Point waitForClick(int &outButtonClicked, double inMaxDelay=0);
			
		 private:
			string mWinID; //<! Window ID of this canvas.
			
			//! Pop canvas window on viewer.
			void initCanvas(void);
			
			//! Send canvas to viewer.
			void updateViewer(void);
			
			//! Disabled methods.
			void setOrigin(const Point& inOrigin);
			void setOrigin(float inX, float inY);
		};
		
	} // end of namespace SVG
	
} // end of namespace PACC

#endif // PACC_SVG_Canvas_hpp_
