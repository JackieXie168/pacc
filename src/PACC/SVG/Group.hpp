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
 */

#ifndef PACC_SVG_Group_hpp_
#define PACC_SVG_Group_hpp_

#include "PACC/SVG/Primitives.hpp"
#include "PACC/XML/Iterator.hpp"

namespace PACC {
	
	namespace SVG {
		
		using namespace std;
		
		/*!\brief Basic graphic primitive container.
		* \ingroup SVGdoc
		*
		* A group holds an ordered list of graphic primitives with default
		* style and transform. The group style and transform applies to all embedded
		* elements that do not define their own. Groups can also be nested.
		*/
		class Group : public Primitive {
		 public:
			explicit Group(const Style& inStyle = Style());
			
			Group& operator<<(const Primitive& inPrimitive);
			
			void clear();
			unsigned count() const;
			
		 protected:
			Group(const string& inName, const XML::AttributeList& inAttributes=XML::AttributeList());
		};
		
	} // end of namespace SVG
	
} // end of namespace PACC

#endif //PACC_SVG_Group_hpp_
