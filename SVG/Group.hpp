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
 * \file PACC/SVG/Group.hpp
 * \brief Class definition for the SVG primitive group.
 * \author Marc Parizeau and Michel Fortin, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.5 $
 * $Date: 2005/11/29 22:31:51 $
 */

#ifndef PACC_SVG_Group_hpp_
#define PACC_SVG_Group_hpp_

#include "SVG/Primitives.hpp"
#include "XML/Iterator.hpp"
#include "Socket/Address.hpp"
#include "Socket/Cafe.hpp"

namespace PACC {
	
	namespace SVG {
		
		using namespace std;
		
		/*!\brief Basic graphic primitive container.
		* \ingroup SVG
		*
		* A group holds an ordered list of graphic primitives with default
		* style and transform. The group style and transform applies to all embedded
		* elements that do not define their own. Groups can also be nested.
		*/
		class Group : public Primitive {
		 public:
			//! Make group with style \c inStyle.
			explicit Group(const Style &inStyle = Style()) : Primitive("g", inStyle) {}
			
			/*!\brief Add primitive \c inPrimitive to this group.
			
			If primitive \c inPrimitive defines an \c id attribute, then it will replace the first primitive in this group with matching id. Otherwise, a new primitive is added at the end of the group. 
			*/
			Group& operator<<(const Primitive& inPrimitive) {
				if(inPrimitive.isDefined("id")) {
					// search for first instance of same id
					XML::Iterator lPos = getFirstChild();
					while(lPos && lPos->getAttribute("id") != inPrimitive.getAttribute("id")) ++lPos;
					if(lPos) *lPos = inPrimitive;
					else insertAsLastChild(new Node(inPrimitive));
				} else insertAsLastChild(new Node(inPrimitive));
				return *this;
			}
			
			//! Return the number of primitives in this group.
			unsigned count() const {return getChildCount();}
			//! Remove all primitives in this group.
			void clear() {eraseChildren();}
			
		 protected:
			//! Make group from name \c inName and attribute list \c inAttributes.
			Group(const string& inName, const XML::AttributeList& inAttributes = XML::AttributeList()): Primitive(inName, inAttributes) {}
			
		};
		
	} // end of namespace SVG
	
} // end of namespace PACC

#endif //PACC_SVG_Group_hpp_
