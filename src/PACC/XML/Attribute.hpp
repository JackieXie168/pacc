/*
 *  Portable Agile C++ Classes (PACC)
 *  Copyright (C) Marc Parizeau
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
 * \file PACC/XML/Attribute.hpp
 * \brief Class definition for the %XML tag attributes.
 * \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.16 $
 * $Date: 2007/01/23 21:28:09 $
 */

#ifndef PACC_XML_Attribute_hpp_
#define PACC_XML_Attribute_hpp_

#include "PACC/Util/StringFunc.hpp"
#include <map>

namespace PACC {
	
	using namespace std;
	
	namespace XML {
				
		/*!\brief %Attribute list for tree nodes.
		\author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
		\ingroup XML
		*/
		class AttributeList : public map<string, string> {
		 public:

			//! Construct an empty attribute list.
			AttributeList() {}

			//! Return concatenation of attribute list \c inAttrList with this list.
			AttributeList operator+(const AttributeList& inAttrList) const {
				return AttributeList(*this) += inAttrList;
			}

			//! Append attribute list \c inAttrList to this list.
			AttributeList& operator+=(const AttributeList& inAttrList) {
				for(AttributeList::const_iterator lPos = inAttrList.begin(); lPos != inAttrList.end(); ++lPos) {
					(*this)[lPos->first] = lPos->second;
				}
				return *this;
			}
			
			//! Return value of attribute \c inName, or empty string if it doesn't exist.
			const string& getAttribute(const string& inName) const {
				static const string lEmpty;
				const_iterator lAttr = find(inName); 
				return (lAttr != end() ? (*lAttr).second : lEmpty);}

			//! Test wheter attribute name \c inName exists in this list.
			bool isDefined(const std::string& inName) const {
				return find(inName) != end();
			}

			//! Remove attribute name \c inName from this attribute list.
			void removeAttribute(const string& inName) {
				erase(inName);
			}
			//! Set attribute \c inName to value \c inValue.
			void setAttribute(const string& inName, const string& inValue) {
				(*this)[inName] = inValue;
			}

			//! Set attribute \c inName to value \c inValue.
			void setAttribute(const string& inName, double inValue) {
				(*this)[inName] = String::convert(inValue);
			}
		};
		
	} // end of XML namespace
	
} // end of PACC namespace

#endif
