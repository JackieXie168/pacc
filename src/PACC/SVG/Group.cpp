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
 * \file PACC/SVG/Group.cpp
 * \brief Class methods for the SVG primitive group.
 * \author Marc Parizeau and Michel Fortin, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 */

#include "PACC/SVG/Group.hpp"

using namespace PACC;

//! Make group with style \c inStyle.
SVG::Group::Group(const Style& inStyle) : Primitive("g", inStyle) 
{
}

//! Make group from name \c inName and attribute list \c inAttributes.
//! This constructor is not accessible to users.
SVG::Group::Group(const string& inName, const XML::AttributeList& inAttributes)
: Primitive(inName, inAttributes) 
{
}

/*!\brief Add primitive \c inPrimitive to this group.
 *
 * If primitive \c inPrimitive defines an \c id attribute, then it will replace 
 * the first primitive in this group with matching id. Otherwise, a new 
 * primitive is added at the end of the group. 
 */
SVG::Group& SVG::Group::operator<<(const Primitive& inPrimitive) 
{
	if(inPrimitive.isDefined("id")) {
		// search for first instance of same id
		XML::Iterator lPos = getFirstChild();
		while(lPos && lPos->getAttribute("id") != inPrimitive.getAttribute("id")) ++lPos;
		if(lPos) *lPos = inPrimitive;
		else insertAsLastChild(new Node(inPrimitive));
	} else insertAsLastChild(new Node(inPrimitive));
	return *this;
}

//! Remove all primitives in this group.
void SVG::Group::clear() {eraseChildren();}

//! Return the number of primitives in this group.
unsigned SVG::Group::count() const {return getChildCount();}
