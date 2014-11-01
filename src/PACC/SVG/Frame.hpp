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
 * \file PACC/SVG/Frame.hpp
 * \brief Class definition for the %SVG clipping frame.
 * \author Marc Parizeau and Michel Fortin, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 */

#ifndef PACC_SVG_Frame_hpp_
#define PACC_SVG_Frame_hpp_

#include "PACC/SVG/Group.hpp"
#include "PACC/XML/Streamer.hpp"

namespace PACC {
	
	namespace SVG {
		
		using namespace std;
		
		/*!\brief Graphic primitive container with a clipping rectangle.
		* \ingroup SVGdoc
		*
		* The frame is a group that implements a clipping rectangle for its elements. 
		* Coordinates for elements inside the frame are relative to the frame
		* origin.
		*/
		class Frame : public Group {
		 public:
			Frame(const Point& inOrigin, const Size& inSize, const Style& inStyle=Style());
			
			Point getOrigin(void) const;
			Size  getSize() const;
			
			void setOrigin(const Point& inOrigin);			
			void setOrigin(double inX, double inY);
			void setSize(const Size& inSize);
			void setSize(double inWidth, double inHeight);
			
			void setViewBox(const Point& inOrigin, const Size& inSize, bool inPreserveAspect=false);
			
		protected:
			Frame(const string& inName, const XML::AttributeList& inAttributes=XML::AttributeList());
			
		 private:
			// transforms are not allowed in frames
			void clearTransform(void);
			void setTransform(const Transform& inTransform);
			
		};
		
	} // end of namespace SVG
	
} // end of namespace PACC

#endif //PACC_SVG_Frame_hpp_
