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
 * \file PACC/SVG/Styles.hpp
 * \brief Class definition for the SVG element styles.
 * \author Marc Parizeau and Michel Fortin, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 */

/*!
 * \defgroup SVGstyle Style attributes
 * \ingroup SVG
 * \brief Attachable style attributes for primitives and containers.
 */

#ifndef PACC_SVG_Styles_hpp_
#define PACC_SVG_Styles_hpp_

#include "PACC/SVG/Types.hpp"
#include "PACC/XML/Attribute.hpp"
#include "PACC/Util/StringFunc.hpp"
#include "PACC/Util/Assert.hpp"

namespace PACC {
	
	using namespace String;
	
	namespace SVG {
		
		using namespace std;
		
		class Style;
		
		/*!\brief Container for a single style attribute.
		 * \ingroup SVGstyle
		 *
		 * An attribute is a pair of name/value strings. Its only 
		 * constructor is protected in order to restrict its usage. It will be
		 * sub-classed for specific %SVG style attributes.
		 */
		class Attribute : public pair<string, string> {
		 public:
			Attribute(const string& inName, const string& inValue);

			Style operator+(const Attribute& inAttribute) const;
			Style operator+(const Style& inAttrList) const;
		};
		
		/*!\brief List of attributes representing the style of an element.
		 * \ingroup SVGstyle
		 * \see Attribute
		 *
		 * A style object holds a list of style attributes.
		 *
		 */
		class Style : public XML::AttributeList {
		 public:
			
			//! Make an empty style.
			Style() {}
			
			//! Make a style from a single attribute \c inAttribute.
			Style(const Attribute& inAttribute) {insert(inAttribute);}
			
			//! Make a style from an attribute list \c inList.
			Style(const XML::AttributeList& inList) : XML::AttributeList(inList) {}
			
			//! Return concatenation of style list \c inAttrList with this list.
			Style operator+(const Style& inAttrList) const {
				return Style(*this) += inAttrList;
			}
			
			//! Append attribute list \c inAttrList to this list.
			Style& operator+=(const Style& inAttrList) {
				for(Style::const_iterator lPos = inAttrList.begin(); lPos != inAttrList.end(); ++lPos) {
					(*this)[lPos->first] = lPos->second;
				}
				return *this;
			}
		};
		
		// functions to make style attibutes
		const Attribute fillColor(const Color& inColor);
		const Attribute fillOpacity(double inValue);
		const Attribute fillRule(RuleType inType);
		const Attribute fontFamily(const string& inName);
		const Attribute fontStyle(FontType inType);
		const Attribute fontSize(double inValue);
		const Attribute opacity(double inValue);
		const Attribute strokeColor(const Color& inColor);
		const Attribute strokeDash(const string& inDash);
		const Attribute strokeDash(const vector<double>& inDash);
		const Attribute strokeDash(DashType inType);
		const Attribute strokeLineCap(CapType inType);
		const Attribute strokeLineJoin(JoinType inType);
		const Attribute strokeMiterLimit(double inRatio);
		const Attribute strokeOpacity(double inValue);
		const Attribute strokeWidth(double inValue);
		const Attribute textAnchor(AnchorType inType);
		const Attribute textBaseline(BaselineType inType);
		
	} // end of SVG namespace
	
} // end of PACC namespace

// Hack to bypass the C++ syntax deficiencies
// see "C++'s most vexing parse", 
// in book Effective STL, by Scott Meyers, Addison-Wesley, item 6

#endif // PACC_SVG_Styles_hpp_
