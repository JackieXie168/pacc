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
 * \file PACC/SVG/Types.cpp
 * \brief Class methods for miscellaneous %SVG data types.
 * \author Marc Parizeau and Michel Fortin, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 */

#include "PACC/SVG/Types.hpp"
#include <sstream>

using namespace std;
using namespace PACC;

/*!
 * \defgroup SVGtype Types
 * \ingroup SVG
 * \brief Data types for graphics primitives and attributes.
 */

//! Construct color using name \c inName.
SVG::Color::Color(ColorName inName) 
{
	switch(inName) {
		case eAliceBlue: string::operator=("aliceblue"); break;
		case eAntiqueWhite: string::operator=("antiquewhite"); break;
		case eAqua: string::operator=("aqua"); break;
		case eAquaMarine: string::operator=("aquamarine"); break;
		case eAzure: string::operator=("azure"); break;
		case eBeige: string::operator=("beige"); break;
		case eBisque: string::operator=("bisque"); break;
		case eBlack: string::operator=("black"); break;
		case eBlacheDalmond: string::operator=("blanchedalmond"); break;
		case eBlue: string::operator=("blue"); break;
		case eBlueViolet: string::operator=("blueviolet"); break;
		case eBrown: string::operator=("brown"); break;
		case eBurlyWood: string::operator=("burlywood"); break;
		case eCadetBlue: string::operator=("cadetblue"); break;
		case eChartreuse: string::operator=("chartreuse"); break;
		case eChocolate: string::operator=("chocolate"); break;
		case eCoral: string::operator=("coral"); break;
		case eCornFlowerBlue: string::operator=("cornflowerblue"); break;
		case eCornSilk: string::operator=("cornsilk"); break;
		case eCrimson: string::operator=("crimson"); break;
		case eCyan: string::operator=("cyan"); break;
		case eDarkBlue: string::operator=("darkblue"); break;
		case eDarkCyan: string::operator=("darkcyan"); break;
		case eDarkGoldenRod: string::operator=("darkgoldenrod"); break;
		case eDarkGray: string::operator=("darkgray"); break;
		case eDarkGreen: string::operator=("darkgreen"); break;
		case eDarkGrey: string::operator=("darkgrey"); break;
		case eDarkKhaki: string::operator=("darkkhaki"); break;
		case eDarkMagenta: string::operator=("darkmagenta"); break;
		case eDarkOliveGreen: string::operator=("darkolivegreen"); break;
		case eDarkOrange: string::operator=("darkorange"); break;
		case eDarkOrchid: string::operator=("darkorchid"); break;
		case eDarkRed: string::operator=("darkred"); break;
		case eDarkSalmon: string::operator=("darksalmon"); break;
		case eDarkSeaGreen: string::operator=("darkseagreen"); break;
		case eDarkSlateBlue: string::operator=("darkslateblue"); break;
		case eDarkSlateGray: string::operator=("darkslategray"); break;
		case eDarkSlateGrey: string::operator=("darkslategrey"); break;
		case eDarkTurquoise: string::operator=("darkturquoise"); break;
		case eDarkViolet: string::operator=("darkviolet"); break;
		case eDeepPink: string::operator=("deeppink"); break;
		case eDeepSkyBlue: string::operator=("deepskyblue"); break;
		case eDimGray: string::operator=("dimgray"); break;
		case eDimGrey: string::operator=("dimgrey"); break;
		case eDodgerBlue: string::operator=("dodgerblue"); break;
		case eFireBrick: string::operator=("firebrick"); break;
		case eFloralWhite: string::operator=("floralwhite"); break;
		case eForestGreen: string::operator=("forestgreen"); break;
		case eFuchsia: string::operator=("fuchsia"); break;
		case eGainsboro: string::operator=("gainsboro"); break;
		case eGostWhite: string::operator=("ghostwhite"); break;
		case eGold: string::operator=("gold"); break;
		case eGoldenRod: string::operator=("goldenrod"); break;
		case eGray: string::operator=("gray"); break;
		case eGrey: string::operator=("grey"); break;
		case eGreen: string::operator=("green"); break;
		case eGreenYellow: string::operator=("greenyellow"); break;
		case eHoneyDew: string::operator=("honeydew"); break;
		case eHotPink: string::operator=("hotpink"); break;
		case eIndianRed: string::operator=("indianred"); break;
		case eIndigo: string::operator=("indigo"); break;
		case eIvory: string::operator=("ivory"); break;
		case eKhaki: string::operator=("khaki"); break;
		case eLavender: string::operator=("lavender"); break;
		case eLavenderBlush: string::operator=("lavenderblush"); break;
		case eLawnGreen: string::operator=("lawngreen"); break;
		case eLemonChiffon: string::operator=("lemonchiffon"); break;
		case eLightBlue: string::operator=("lightblue"); break;
		case eLightCoral: string::operator=("lightcoral"); break;
		case eLightCyan: string::operator=("lightcyan"); break;
		case eLightGolderRodYellow: string::operator=("lightgoldenrodyellow"); break;
		case eLightGray: string::operator=("lightgray"); break;
		case eLightGreen: string::operator=("lightgreen"); break;
		case eLightGrey: string::operator=("lightgrey"); break;
		case eLigntPink: string::operator=("lightpink"); break;
		case eLightSalmon: string::operator=("lightsalmon"); break;
		case eLightSeaGreen: string::operator=("lightseagreen"); break;
		case eLightSkyBlue: string::operator=("lightskyblue"); break;
		case eLightSlateGray: string::operator=("lightslategray"); break;
		case eLightSlateGrey: string::operator=("lightslategrey"); break;
		case eLightSteelBlue: string::operator=("lightsteelblue"); break;
		case eLightYellow: string::operator=("lightyellow"); break;
		case eLime: string::operator=("lime"); break;
		case eLimeGreen: string::operator=("limegreen"); break;
		case eLinen: string::operator=("linen"); break;
		case eMagenta: string::operator=("magenta"); break;
		case eMaroon: string::operator=("maroon"); break;
		case eMediumAquaMarine: string::operator=("mediumaquamarine"); break;
		case eMediumBlue: string::operator=("mediumblue"); break;
		case eMediumOrchid: string::operator=("mediumorchid"); break;
		case eMediumPurple: string::operator=("mediumpurple"); break;
		case eMeidumSeaGreen: string::operator=("mediumseagreen"); break;
		case eMediumSlateBlue: string::operator=("mediumslateblue"); break;
		case eMediumSpringGreen: string::operator=("mediumspringgreen"); break;
		case eMediumTurquoise: string::operator=("mediumturquoise"); break;
		case eMediumVioletRed: string::operator=("mediumvioletred"); break;
		case eModNightBlue: string::operator=("midnightblue"); break;
		case eMintCream: string::operator=("mintcream"); break;
		case eMistyRose: string::operator=("mistyrose"); break;
		case eMoccasin: string::operator=("moccasin"); break;
		case eNavajoWhite: string::operator=("navajowhite"); break;
		case eNavy: string::operator=("navy"); break;
		case eNone: string::operator=("none"); break;
		case eOldLace: string::operator=("oldlace"); break;
		case eOlive: string::operator=("olive"); break;
		case eOliveDrab: string::operator=("olivedrab"); break;
		case eOrange: string::operator=("orange"); break;
		case eOrangered: string::operator=("orangered"); break;
		case eOrchid: string::operator=("orchid"); break;
		case ePaleGoldenRod: string::operator=("palegoldenrod"); break;
		case ePaleGreen: string::operator=("palegreen"); break;
		case ePaleTurquoise: string::operator=("paleturquoise"); break;
		case ePaleVioletRed: string::operator=("palevioletread");
		case ePapayaWhip: string::operator=("papayawhip"); break;
		case ePeachPuff: string::operator=("peachpuff"); break;
		case ePeru: string::operator=("peru"); break;
		case ePink: string::operator=("pink"); break;
		case ePlum: string::operator=("plum"); break;
		case ePowderBlue: string::operator=("powderblue"); break;
		case ePurple: string::operator=("purple"); break;
		case eRed: string::operator=("red"); break;
		case eRosyBrown: string::operator=("rosybrown"); break;
		case eRoyalBlue: string::operator=("royalblue"); break;
		case eSaddleBrown: string::operator=("saddlebrown"); break;
		case eSalmon: string::operator=("salmon"); break;
		case eSandyBrown: string::operator=("sandybrown"); break;
		case eSeaGreen: string::operator=("seagreen"); break;
		case eSeaShell: string::operator=("seashell"); break;
		case eSienna: string::operator=("sienna"); break;
		case eSilver: string::operator=("silver"); break;
		case eSkyBlue: string::operator=("skyblue"); break;
		case eSlateBlue: string::operator=("slateblue"); break;
		case eSlateGray: string::operator=("slategray"); break;
		case eSlateGrey: string::operator=("slategrey"); break;
		case eSnow: string::operator=("snow"); break;
		case eSpringGreen: string::operator=("springgreen"); break;
		case eSteelBlue: string::operator=("steelblue"); break;
		case eTan: string::operator=("tan"); break;
		case eTeal: string::operator=("teal"); break;
		case eThistle: string::operator=("thistle"); break;
		case eTomato: string::operator=("tomato"); break;
		case eTurquoise: string::operator=("turquoise"); break;
		case eViolet: string::operator=("violet"); break;
		case eWheat: string::operator=("wheat"); break;
		case eWhite: string::operator=("white"); break;
		case eWhiteSmoke: string::operator=("whitesmoke"); break;
		case eYellow: string::operator=("yellow"); break;
		case eYellowGreen: string::operator=("yellowgreen"); break;
	}
}

//! Construct color using string \c inColor
SVG::Color::Color(const string& inColor) : string(inColor) 
{
}

/*!
* Function that output a string in the form "rgb(10%, 12%, 100%)" representing
 * a SVG Color.
 */
std::string makeRGBColorString(float inRed, float inGreen, float inBlue) 
{
	ostringstream lStream;
	lStream << "rgb(" << inRed*100 << "%, " << inGreen*100 << "%, " << inBlue*100 << "%)";
	return lStream.str();
}

//! Construct with components \c inRed, \c inGreen, and \c inBlue.
SVG::RGBColor::RGBColor(float inRed, float inGreen, float inBlue) : Color(makeRGBColorString(inRed,inGreen,inBlue)) {}

//! Construct gray color of intensity \c inLevel.
SVG::Gray::Gray(float inLevel) : RGBColor(inLevel, inLevel, inLevel) 
{
}

//! Return point as string value.
std::string SVG::Point::getStringValue() const 
{
	ostringstream lStream;
	lStream << x << "," << y;
	return lStream.str();
}

//! Concatenate with point \c inPoint, and return new point list.
SVG::PointList SVG::Point::operator+(const Point& inPoint) const 
{
	return PointList(*this) += inPoint;
}

//! Concatenate with point list \c inList and return new point list.
SVG::PointList SVG::Point::operator+(const SVG::PointList& inList) const 
{
	return PointList(*this) += inList;
}


//! Return size as string value.
std::string SVG::Size::getStringValue(void) const 
{
	ostringstream lStream;
	lStream << width << "," << height;
	return lStream.str();
}

//! Return a string representation of this list.
std::string SVG::PointList::getStringValue(void) const {
	if(size() == 0 ) return string("");
	string lString((*this)[0].getStringValue());
	for ( unsigned i = 1; i < size(); i++ ) lString +=  string(" ") + (*this)[i].getStringValue();
	return lString;
}

