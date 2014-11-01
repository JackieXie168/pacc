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
 */

#ifndef PACC_SVG_Canvas_hpp_
#define PACC_SVG_Canvas_hpp_

#include "PACC/SVG/Document.hpp"
#include "PACC/Socket/Cafe.hpp"

namespace PACC {
	
	namespace SVG {
		
		using namespace std;
		
		/*!\brief Specialized document to display on-screen graphics. 
		* \ingroup SVGdoc
		*
		* A canvas opens a TCP/IP connection with a running viewer 
		* user agent on the network. By default it connects to the 
		* localhost at port 61250.
		*
		* The viewer opens a window showing the canvas. Changes made to the
		* canvas are updated in real-time on the viewer side.
		*/
		class Canvas : public Document, protected Socket::Cafe {
		public:
			Canvas(const string& inTitle, const Size& inSize, 
				   const string& inHostPort="localhost:61250");
			Canvas(const string& inTitle, const Size& inSize, const Style& inStyle, 
				   const string& inHostPort="localhost:61250");
			Canvas(const Document& inDocument, const string& inHostPort="localhost:61250");
			
			Canvas& operator=(const Document& inDocument);			
			Canvas& operator<<(const Primitive& inGraphic);
			
			void clear(void);
			void setSize(const Size& inSize);
			void setSize(double inWidth, double inHeight);
			Point waitForClick(int& outButtonClicked, double inMaxDelay=0);
			
		 private:
			string mWinID; //<! Window ID of this canvas.
			
			void initCanvas(void);			
			void updateViewer(void);
			
			//! Disabled methods.
			void setOrigin(const Point& inOrigin);
			void setOrigin(float inX, float inY);
		};
		
	} // end of namespace SVG
	
} // end of namespace PACC

#endif // PACC_SVG_Canvas_hpp_
