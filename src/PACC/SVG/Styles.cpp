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
 * \file PACC/SVG/Styles.cpp
 * \brief Class methods for %SVG styles.
 * \author Marc Parizeau and Michel Fortin, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 */

#include "PACC/SVG/Styles.hpp"

using namespace std;
using namespace PACC;

//! Construct an attribute style from name \c inName and value \c inValue.
SVG::Attribute::Attribute(const string& inName, const string& inValue) 
: pair<string, string>(inName, inValue) 
{
}

//! Return a style list of attributes by combining attribute \c inAttribute with this attribute.
SVG::Style SVG::Attribute::operator+(const SVG::Attribute& inAttribute) const
{
	return Style(*this) += inAttribute;
}

//! Return a style list of attributes by merging list \c inAttrList with this attribute.
SVG::Style SVG::Attribute::operator+(const SVG::Style& inAttrList) const
{
	return Style(*this) += inAttrList;
}

/*!\brief Make fill color attribute using color \c inColor.
 * \ingroup SVGstyle
 * \see Color to specifiy a named color;
 * \see RGBColor to specify color components;
 * \see Gray for shades of gray.
 *
 * Default fill color is eBlack.
 */
const SVG::Attribute SVG::fillColor(const SVG::Color& inColor) 
{
	return Attribute("fill", inColor);
}

/*!\brief Make fill opacity attribute from value \c inValue (0=transparent).
 * \ingroup SVGstyle
 * \see opacity, strokeOpacity
 *
 * Valid opacity values range from 0 (fully transparent) 
 * to 1 (fully opaque). Default is 1.
 */
const SVG::Attribute SVG::fillOpacity(double inValue) 
{
	PACC_AssertM(inValue >= 0 && inValue <= 1, "Invalid opacity value!");
	return Attribute("fill-opacity", String::convert(inValue));
}

/*!\brief Make fill rule attribute from rule type \c inType.
 * \ingroup SVGstyle
 * \see RuleType
 * \see <a href="http://www.w3.org/TR/SVG/painting.html#FillRuleProperty">
 * %SVG fill-rule specification</a>
 *
 * Overlapping regions can be filled according to two differents rules:
 * non-zero and even-odd. 
 *
 * For the non-zero rule, 
 * the "insideness" of a point on the canvas is determined by 
 * drawing a ray from that point to infinity in any direction and then 
 * examining the places where a segment of the shape crosses the ray. 
 * Starting with a count of zero, add one each time a path segment 
 * crosses the ray from left to right and subtract one each time a 
 * path segment crosses the ray from right to left. After counting the 
 * crossings, if the result is zero then the point is outside the path. 
 * Otherwise, it is inside. 
 *
 * For the even-odd rule, 
 * the "insideness" of a point on the canvas is determined by 
 * drawing a ray from that point to infinity in any direction and 
 * counting the number of path segments from the given shape that the 
 * ray crosses. If this number is odd, the point is inside; if even, 
 * the point is outside.
 *
 * Default rule is eNonZero.
 */
const SVG::Attribute SVG::fillRule(SVG::RuleType inType) 
{
	string lValue;
	switch(inType) {
		case eNonZero: lValue = "nonzero";
		case eEvenOdd: lValue = "evenodd";
	}
	PACC_AssertM(!lValue.empty(), "Invalid fill rule type!");
	return Attribute("fill-rule",lValue);
}

/*!\brief Make font family attribute using name list \c inName.
 * \ingroup SVGstyle
 * \see <a href="http://www.w3.org/TR/SVG/text.html#FontFamilyProperty">%SVG font family specification</a>
 *
 * The name list can contain one or more font family names. If more
 * than one name is provided, the first matching family will be used. 
 * The list must be of the form: "Times, Times New Roman, serif".
 * 
 * Default font family is agent dependent.
 */
const SVG::Attribute SVG::fontFamily(const string& inName) 
{
	return Attribute("font-family", inName);
}

/*!\brief make font size attribute using value \c inValue.
 * \ingroup SVGstyle
 *
 * Font size is expressed in current user coordinates.
 */
const SVG::Attribute SVG::fontSize(double inValue) 
{
	return Attribute("font-size", String::convert(inValue));
}

/*!\brief Make font style attribute using type \c inType.
 * \ingroup SVGstyle
 *
 * Valid font styles are: eBold, eItalic, eOblique, eUnderline,
 * and eStrike. Types eItalic and eOblique are mutually exclusive. 
 * Types eUnderline and eStrike are also mutually exclusive, and may 
 * not be supported by all user agents. 
 */
const SVG::Attribute SVG::fontStyle(SVG::FontType inType)
{
	string lName, lValue;
	switch(inType) {
		case eBold: lName = "font-weight"; lValue = "bold"; break;
		case eItalic: lName = "font-style"; lValue = "italic"; break;
		case eOblique: lName = "font-style"; lValue = "oblique"; break;
		case eUnderline: lName = "text-decoration"; lValue = "underline"; break;
		case eStrike: lName = "text-decoration"; lValue = "line-through"; break;
	}
	PACC_AssertM(!lName.empty() && !lValue.empty(), "Invalid font style type!");
	return Attribute(lName,lValue);
}

/*!\brief Make opacity attribute from value \c inValue (0=transparent).
 * \ingroup SVGstyle
 * \see <a href="http://www.w3.org/TR/SVG/masking.html#OpacityProperty">
 * %SVG opacity property specification</a>
 *
 * This attribute style will set the opacity of the resulting 
 * image of a graphic. 
 *
 * Valid opacity values range from 0 (fully transparent) 
 * to 1 (fully opaque). Default is 1.
 *
 * This is different than setting fillOpacity and strokeOpacity. For
 * example, if you set both stroke and fill opacity to 0.5, you will 
 * see the fill edge behind the stroke. With opacity the whole shape is
 * rendered and then made transparent.
 */
const SVG::Attribute SVG::opacity(double inValue) 
{
	PACC_AssertM(inValue >= 0 && inValue <= 1, "Invalid opacity value!");
	return Attribute("opacity", String::convert(inValue));
}

/*!\brief Make stroke color attribute using color \c inColor.
 * \ingroup SVGstyle
 * \see Color to specify a named color;.
 * \see RGBColor to specify color components;
 * \see Gray for shades of gray.
 *
 * Default stroke color is eNone.
 */
const SVG::Attribute SVG::strokeColor(const SVG::Color& inColor) 
{
	return Attribute("stroke", inColor);
}

/*!\brief Make stroke dash array attribute using string \c inDash.
 * \ingroup SVGstyle
 * \see <a href="http://www.w3.org/TR/SVG/painting.html#StrokeDasharrayProperty">
 * %SVG stroke-dasharray specification</a>
 *
 * The dash array is a list of numbers representing the lengths of each
 * part of the dashed line. The first number represent the first dash 
 * length, the next is the empty space length, the next is the second dash
 * lenght, and so on.
 */
const SVG::Attribute SVG::strokeDash(const string& inDash) 
{
	return Attribute("stroke-dasharray", inDash);
}

/*!\brief Make stroke dash array attribute using vector \c inDash.
 * \ingroup SVGstyle
 * \see <a href="http://www.w3.org/TR/SVG/painting.html#StrokeDasharrayProperty">%SVG stroke-dasharray specification</a>
 *
 * The dash array is a vector of numbers representing the lengths of each
 * part of the dashed line. The first number represent the first dash 
 * length, the next is the empty space length, the next is the second dash
 * lenght, and so on.
 */
const SVG::Attribute SVG::strokeDash(const vector<double>& inDash) 
{
	string lValue;
	for(int i = 0; i < inDash.size(); ++i) {
		lValue += String::convert(inDash[i]) + " ";
	}
	PACC_AssertM(!lValue.empty(), "Invalid dash vector!");
	return strokeDash(lValue);
}

/*!\brief Make stroke dash array attribute using type \c inType.
 * \ingroup SVGstyle
 * \see DashType
 * \see <a href="http://www.w3.org/TR/SVG/painting.html#StrokeDasharrayProperty">
 * %SVG stroke-dasharray specification</a>
 *
 * Supported dash types are: eConstinuous, eDashed, eDotted. Default is 
 * eContinuous.
 */
const SVG::Attribute SVG::strokeDash(SVG::DashType inType) 
{
	string lValue;
	switch(inType) {
		case eContinuous: lValue = "none";
		case eDashed: lValue = "5 4";
		case eDotted: lValue = "1 2";
	}			
	PACC_AssertM(!lValue.empty(), "Invalid dash  type!");
	return Attribute("stroke-dasharray",lValue);
}

/*!\brief Make stroke linecap attribute using cap type \c inType.
 * \ingroup SVGstyle
 * \see CapType
 * \see <a href="http://www.w3.org/TR/SVG/painting.html#StrokeLinecapProperty">
 * %SVG stroke-linecap specification</a>
 * 
 * This attribute defines how strokes are terminated. Supported
 * line termination are: eButt, eRound, and eSquare. Default is eButt.
 */
const SVG::Attribute SVG::strokeLineCap(SVG::CapType inType)
{
	string lValue;
	switch(inType) {
		case eButt: lValue = "butt"; break;
		case eRoundCap: lValue = "round"; break;
		case eSquare: lValue = "square"; break;
	}
	PACC_AssertM(!lValue.empty(), "Invalid stroke linecap type!");
	return Attribute("stroke-linecap",lValue);
}

/*!\brief Make stroke linejoin attribute using join type \c inType.
 * \ingroup SVGstyle
 * \see JoinType
 * \see <a href="http://www.w3.org/TR/SVG/painting.html#StrokeLinejoinProperty">
 * %SVG stroke-linejoin specification</a>
 * 
 * This attribute defines how strokes are joined together. Supported
 * line join are : eMiter, eRound, and eBevel. Default is eMiter.
 */
const SVG::Attribute SVG::strokeLineJoin(SVG::JoinType inType) 
{
	string lValue;
	switch(inType) {
		case eMiter: lValue = "miter"; break;
		case eRoundJoin: lValue = "round"; break;
		case eBevel: lValue = "bevel"; break;
	}			
	PACC_AssertM(!lValue.empty(), "Invalid stroke linejoin type!");
	return Attribute("stroke-linejoin",lValue);
}

/*!\brief Make stroke miter limit attribute to ratio \c inRatio.
 * \ingroup SVGstyle
 * \see strokeLineJoin
 *
 * This style attribute has no effect on strokes with line join not
 * set to miter. The default miter limit ratio is 4.
 *
 */
const SVG::Attribute SVG::strokeMiterLimit(double inRatio) {
	return Attribute("stroke-miterlimit", String::convert(inRatio));
}

/*! \brief make stroke opacity attribute using value \c inValue 
 * (0=transparent).
 * \ingroup SVGstyle
 *
 * Valid opacity values range from 0 (fully transparent) 
 * to 1 (fully opaque). Default is 1.
 */
const SVG::Attribute SVG::strokeOpacity(double inValue) {
	PACC_AssertM(inValue >= 0 && inValue <= 1, "Invalid opacity value!");
	return Attribute("stroke-opacity", String::convert(inValue));
}

/*!\brief Make stroke width attribute using value \c inValue.
 * \ingroup SVGstyle
 *
 * Default stroke width is 1.
 */
const SVG::Attribute SVG::strokeWidth(double inValue) {
	PACC_AssertM(inValue >= 0, "Invalid width value!");
	return Attribute("stroke-width", String::convert(inValue));
}

/*!\brief Make text anchor position attribute using type \c inType.
 * \ingroup SVGstyle
 * \see AnchorType
 *
 * This attribute allows you to specify the position of the text anchor. 
 * valid options are: eStart, eMiddle, and eEnd. Default is eStart.
 */
const SVG::Attribute SVG::textAnchor(SVG::AnchorType inType)
{
	string lValue;
	switch(inType) {
		case eStart: lValue = "start"; break;
		case eMiddle: lValue = "middle"; break;
		case eEnd: lValue = "end"; break;
	}
	PACC_AssertM(!lValue.empty(), "Invalid text anchor type!");
	return Attribute("text-anchor",lValue);
}

/*!\brief Make text baseline attribute using type \c inType.
 * \ingroup SVGstyle
 * \see BaselineType
 *
 * This attribute allows you to specify the position of the text baseline. 
 * valid options are: eAlpha, eTop, eCentral, and eBottom. Default is eAlpha.
 *
 * This attribute appears not yet supported by Qt SVG.
 */
const SVG::Attribute SVG::textBaseline(SVG::BaselineType inType)
{
	string lValue;
	switch(inType) {
		case eAlpha: lValue = "alphabetical"; break;
		case eBottom: lValue = "text-after-edge"; break;
		case eCentral: lValue = "central"; break;
		case eTop: lValue = "hanging"; break;
	}
	PACC_AssertM(!lValue.empty(), "Invalid text baseline type!");
	return Attribute("dominant-baseline",lValue);
}
