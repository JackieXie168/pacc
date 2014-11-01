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
 * \file PACC/SVG/Frame.cpp
 * \brief Class methods for the %SVG clipping frame.
 * \author Marc Parizeau and Michel Fortin, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 */

#include "PACC/SVG/Frame.hpp"

using namespace std;
using namespace PACC;

//! Make frame at origin \c inOrigin with size \c inSize, and using style \c inStyle.
SVG::Frame::Frame(const SVG::Point& inOrigin, const SVG::Size& inSize, const SVG::Style& inStyle)
: Group("svg") 
{
	setAttribute("x", inOrigin.x);
	setAttribute("y", inOrigin.y);
	setAttribute("width", inSize.width);
	setAttribute("height", inSize.height);
	*this += inStyle; 
}

//! Make Frame from name \c inName and attribute list \c inAttributes.
//! This constructor is not accessible to users.
SVG::Frame::Frame(const string& inName, const XML::AttributeList& inAttributes)
: Group(inName, inAttributes) 
{
}

//! Return frame origin.
SVG::Point SVG::Frame::getOrigin(void) const 
{
	return Point(String::convertToFloat(getAttribute("x")), 
				 String::convertToFloat(getAttribute("y")));
}	

//! Return frame size.
SVG::Size SVG::Frame::getSize() const 
{
	return Size(String::convertToFloat(getAttribute("width")), 
				String::convertToFloat(getAttribute("height")));
}

//! Set frame origin to point \c inPoint.
void SVG::Frame::setOrigin(const Point& inOrigin) 
{
	setAttribute("x", inOrigin.x);
	setAttribute("y", inOrigin.y);
}
			
//! Set frame origin to coordinates \c inX and \c inY.
void SVG::Frame::setOrigin(double inX, double inY) 
{
	setAttribute("x", inX);
	setAttribute("y", inY);
}
			
//! Set frame size to size \c inSize.
void SVG::Frame::setSize(const Size& inSize) 
{
	setAttribute("width", inSize.width);
	setAttribute("height", inSize.height);
}
			
//! Set frame size to width \c inwidth and height \c inHeight.
void SVG::Frame::setSize(double inWidth, double inHeight) 
{
	setAttribute("width", inWidth);
	setAttribute("height", inHeight);
}	
			
//! Set viewbox of this frame to origin \c inOrigin and size \c inSize.
void SVG::Frame::setViewBox(const Point& inOrigin, const Size& inSize, bool inPreserveAspect) 
{
	setAttribute("viewBox", String::convert(inOrigin.x) + " "
				 + String::convert(inOrigin.y) + " "
				 + String::convert(inSize.width) + " "
				 + String::convert(inSize.height));
	setAttribute("preserveAspectRatio", inPreserveAspect?"xMidYMid":"none");
}
