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

/*!\file PACC/SVG/Frame.hpp
 * \brief Class definition for the %SVG clipping frame.
 * \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.1 $
 * $Date: 2005/06/08 18:46:50 $
 */

#ifndef PACC_SVG_Frame_hpp_
#define PACC_SVG_Frame_hpp_

#include "SVG/Group.hpp"

namespace PACC {
	
	namespace SVG {
   
		using namespace std;
		
		/*!\brief Graphic primitive container with a clipping rectangle.
		 * \ingroup SVG
		 *
		 * The frame is a group that implements a clipping rectangle for its elements. 
		 * Coordinates for elements inside the frame are relative to the frame
		 * origin.
		 */
		class Frame : public Group {
		 public:
			//! Make frame at origin \c inOrigin with size \c inSize, and using style \c inStyle.
			Frame(const Point &inOrigin, const Size &inSize, const Style &inStyle = Style()) : Group("svg") {
				setAttribute("x", inOrigin.x);
				setAttribute("y", inOrigin.y);
				setAttribute("width", inSize.width);
				setAttribute("height", inSize.height);
				*this += inStyle; 
			}
						
			//! Return frame origin.
			Point getOrigin(void) const {
				return Point(String::convertToFloat(getAttribute("x")), String::convertToFloat(getAttribute("y")));
			}			
			//! Set frame origin to point \c inPoint.
			void setOrigin(const Point& inOrigin) {
				setAttribute("x", inOrigin.x);
				setAttribute("y", inOrigin.y);
			}
			//! Set frame origin to coordinates \c inX and \c inY.
			void setOrigin(float inX, float inY) {
				setAttribute("x", inX);
				setAttribute("y", inY);
			}
			//! Return frame size.
			Size getSize() const {
				return Size(String::convertToFloat(getAttribute("width")), String::convertToFloat(getAttribute("height")));
			}
			//! Set frame size to size \c inSize.
			void setSize(const Size& inSize) {
				setAttribute("width", inSize.width);
				setAttribute("height", inSize.height);
			}
			//! Set frame size to width \c inwidth and height \c inHeight.
			void setSize(float inWidth, float inHeight) {
				setAttribute("width", inWidth);
				setAttribute("height", inHeight);
			}			
			
		};
		
	} // end of namespace SVG
	
} // end of namespace PACC

#endif //PACC_SVG_Frame_hpp_
