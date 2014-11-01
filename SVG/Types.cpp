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

/*!\file PACC/SVG/Types.cpp
 * \brief Class methods for miscellaneous %SVG data types.
 * \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.1 $
 * $Date: 2005/06/08 18:46:50 $
 */

#include "SVG/Types.hpp"
#include <sstream>

using namespace std;
using namespace PACC;

/* Color constants */
const SVG::Color SVG::Color::cAliceBlue("aliceblue");
const SVG::Color SVG::Color::cAntiqueWhite("antiquewhite");
const SVG::Color SVG::Color::cAqua("aqua");
const SVG::Color SVG::Color::cAquaMarine("aquamarine");
const SVG::Color SVG::Color::cAzure("azure");
const SVG::Color SVG::Color::cBeige("beige");
const SVG::Color SVG::Color::cBisque("bisque");
const SVG::Color SVG::Color::cBlack("black");
const SVG::Color SVG::Color::cBlacheDalmond("blanchedalmond");
const SVG::Color SVG::Color::cBlue("blue");
const SVG::Color SVG::Color::cBlueViolet("blueviolet");
const SVG::Color SVG::Color::cBrown("brown");
const SVG::Color SVG::Color::cBurlyWood("burlywood");
const SVG::Color SVG::Color::cCadetBlue("cadetblue");
const SVG::Color SVG::Color::cChartreuse("chartreuse");
const SVG::Color SVG::Color::cChocolate("chocolate");
const SVG::Color SVG::Color::cCoral("coral");
const SVG::Color SVG::Color::cCornFlowerBlue("cornflowerblue");
const SVG::Color SVG::Color::cCornSilk("cornsilk");
const SVG::Color SVG::Color::cCrimson("crimson");
const SVG::Color SVG::Color::cCyan("cyan");
const SVG::Color SVG::Color::cDarkBlue("darkblue");
const SVG::Color SVG::Color::cDarkCyan("darkcyan");
const SVG::Color SVG::Color::cDarkGoldenRod("darkgoldenrod");
const SVG::Color SVG::Color::cDarkGray("darkgray");
const SVG::Color SVG::Color::cDarkGreen("darkgreen");
const SVG::Color SVG::Color::cDarkGrey("darkgrey");
const SVG::Color SVG::Color::cDarkKhaki("darkkhaki");
const SVG::Color SVG::Color::cDarkMagenta("darkmagenta");
const SVG::Color SVG::Color::cDarkOliveGreen("darkolivegreen");
const SVG::Color SVG::Color::cDarkOrange("darkorange");
const SVG::Color SVG::Color::cDarkOrchid("darkorchid");
const SVG::Color SVG::Color::cDarkRed("darkred");
const SVG::Color SVG::Color::cDarkSalmon("darksalmon");
const SVG::Color SVG::Color::cDarkSeaGreen("darkseagreen");
const SVG::Color SVG::Color::cDarkSlateBlue("darkslateblue");
const SVG::Color SVG::Color::cDarkSlateGray("darkslategray");
const SVG::Color SVG::Color::cDarkSlateGrey("darkslategrey");
const SVG::Color SVG::Color::cDarkTurquoise("darkturquoise");
const SVG::Color SVG::Color::cDarkViolet("darkviolet");
const SVG::Color SVG::Color::cDeepPink("deeppink");
const SVG::Color SVG::Color::cDeepSkyBlue("deepskyblue");
const SVG::Color SVG::Color::cDimGray("dimgray");
const SVG::Color SVG::Color::cDimGrey("dimgrey");
const SVG::Color SVG::Color::cDodgerBlue("dodgerblue");
const SVG::Color SVG::Color::cFireBrick("firebrick");
const SVG::Color SVG::Color::cFloralWhite("floralwhite");
const SVG::Color SVG::Color::cForestGreen("forestgreen");
const SVG::Color SVG::Color::cFuchsia("fuchsia");
const SVG::Color SVG::Color::cGainsboro("gainsboro");
const SVG::Color SVG::Color::cGostWhite("ghostwhite");
const SVG::Color SVG::Color::cGold("gold");
const SVG::Color SVG::Color::cGoldenRod("goldenrod");
const SVG::Color SVG::Color::cGray("gray");
const SVG::Color SVG::Color::cGrey("grey");
const SVG::Color SVG::Color::cGreen("green");
const SVG::Color SVG::Color::cGreenYellow("greenyellow");
const SVG::Color SVG::Color::cHoneyDew("honeydew");
const SVG::Color SVG::Color::cHotPink("hotpink");
const SVG::Color SVG::Color::cIndianRed("indianred");
const SVG::Color SVG::Color::cIndigo("indigo");
const SVG::Color SVG::Color::cIvory("ivory");
const SVG::Color SVG::Color::cKhaki("khaki");
const SVG::Color SVG::Color::cLavender("lavender");
const SVG::Color SVG::Color::cLavenderBlush("lavenderblush");
const SVG::Color SVG::Color::cLawnGreen("lawngreen");
const SVG::Color SVG::Color::cLemonChiffon("lemonchiffon");
const SVG::Color SVG::Color::cLightBlue("lightblue");
const SVG::Color SVG::Color::cLightCoral("lightcoral");
const SVG::Color SVG::Color::cLightCyan("lightcyan");
const SVG::Color SVG::Color::cLightGolderRodYellow("lightgoldenrodyellow");
const SVG::Color SVG::Color::cLightGray("lightgray");
const SVG::Color SVG::Color::cLightGreen("lightgreen");
const SVG::Color SVG::Color::cLightGrey("lightgrey");
const SVG::Color SVG::Color::cLigntPink("lightpink");
const SVG::Color SVG::Color::cLightSalmon("lightsalmon");
const SVG::Color SVG::Color::cLightSeaGreen("lightseagreen");
const SVG::Color SVG::Color::cLightSkyBlue("lightskyblue");
const SVG::Color SVG::Color::cLightSlateGray("lightslategray");
const SVG::Color SVG::Color::cLightSlateGrey("lightslategrey");
const SVG::Color SVG::Color::cLightSteelBlue("lightsteelblue");
const SVG::Color SVG::Color::cLightYellow("lightyellow");
const SVG::Color SVG::Color::cLime("lime");
const SVG::Color SVG::Color::cLimeGreen("limegreen");
const SVG::Color SVG::Color::cLinen("linen");
const SVG::Color SVG::Color::cMagenta("magenta");
const SVG::Color SVG::Color::cMaroon("maroon");
const SVG::Color SVG::Color::cMediumAquaMarine("mediumaquamarine");
const SVG::Color SVG::Color::cMediumBlue("mediumblue");
const SVG::Color SVG::Color::cMediumOrchid("mediumorchid");
const SVG::Color SVG::Color::cMediumPurple("mediumpurple");
const SVG::Color SVG::Color::cMeidumSeaGreen("mediumseagreen");
const SVG::Color SVG::Color::cMediumSlateBlue("mediumslateblue");
const SVG::Color SVG::Color::cMediumSpringGreen("mediumspringgreen");
const SVG::Color SVG::Color::cMediumTurquoise("mediumturquoise");
const SVG::Color SVG::Color::cMediumVioletRed("mediumvioletred");
const SVG::Color SVG::Color::cModNightBlue("midnightblue");
const SVG::Color SVG::Color::cMintCream("mintcream");
const SVG::Color SVG::Color::cMistyRose("mistyrose");
const SVG::Color SVG::Color::cMoccasin("moccasin");
const SVG::Color SVG::Color::cNavajoWhite("navajowhite");
const SVG::Color SVG::Color::cNavy("navy");
const SVG::Color SVG::Color::cOldLace("oldlace");
const SVG::Color SVG::Color::cOlive("olive");
const SVG::Color SVG::Color::cOliveDrab("olivedrab");
const SVG::Color SVG::Color::cOrange("orange");
const SVG::Color SVG::Color::cOrangered("orangered");
const SVG::Color SVG::Color::cOrchid("orchid");
const SVG::Color SVG::Color::cPaleGoldenRod("palegoldenrod");
const SVG::Color SVG::Color::cPaleGreen("palegreen");
const SVG::Color SVG::Color::cPaleTurquoise("paleturquoise");
const SVG::Color SVG::Color::cPaleVioletRed("palevioletred");
const SVG::Color SVG::Color::cPapayaWhip("papayawhip");
const SVG::Color SVG::Color::cPeachPuff("peachpuff");
const SVG::Color SVG::Color::cPeru("peru");
const SVG::Color SVG::Color::cPink("pink");
const SVG::Color SVG::Color::cPlum("plum");
const SVG::Color SVG::Color::cPowderBlue("powderblue");
const SVG::Color SVG::Color::cPurple("purple");
const SVG::Color SVG::Color::cRed("red");
const SVG::Color SVG::Color::cRosyBrown("rosybrown");
const SVG::Color SVG::Color::cRoyalBlue("royalblue");
const SVG::Color SVG::Color::cSaddleBrown("saddlebrown");
const SVG::Color SVG::Color::cSalmon("salmon");
const SVG::Color SVG::Color::cSandyBrown("sandybrown");
const SVG::Color SVG::Color::cSeaGreen("seagreen");
const SVG::Color SVG::Color::cSeaShell("seashell");
const SVG::Color SVG::Color::cSienna("sienna");
const SVG::Color SVG::Color::cSilver("silver");
const SVG::Color SVG::Color::cSkyBlue("skyblue");
const SVG::Color SVG::Color::cSlateBlue("slateblue");
const SVG::Color SVG::Color::cSlateGray("slategray");
const SVG::Color SVG::Color::cSlateGrey("slategrey");
const SVG::Color SVG::Color::cSnow("snow");
const SVG::Color SVG::Color::cSpringGreen("springgreen");
const SVG::Color SVG::Color::cSteelBlue("steelblue");
const SVG::Color SVG::Color::cTan("tan");
const SVG::Color SVG::Color::cTeal("teal");
const SVG::Color SVG::Color::cThistle("thistle");
const SVG::Color SVG::Color::cTomato("tomato");
const SVG::Color SVG::Color::cTurquoise("turquoise");
const SVG::Color SVG::Color::cViolet("violet");
const SVG::Color SVG::Color::cWheat("wheat");
const SVG::Color SVG::Color::cWhite("white");
const SVG::Color SVG::Color::cWhiteSmoke("whitesmoke");
const SVG::Color SVG::Color::cYellow("yellow");
const SVG::Color SVG::Color::cYellowGreen("yellowgreen");

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

SVG::RGBColor::RGBColor(float inRed, float inGreen, float inBlue) : Color(makeRGBColorString(inRed, inGreen, inBlue)) {}

std::string SVG::Point::getStringValue() const 
{
    ostringstream lStream;
    lStream << x << "," << y;
    return lStream.str();
}

//! Concatenate with point \c inPoint, and return new point list.
SVG::PointList SVG::Point::operator+(const Point &inPoint) const 
{
	return PointList(*this) += inPoint;
}

//! Concatenate with point list \c inList and return new point list.
SVG::PointList SVG::Point::operator+(const SVG::PointList &inList) const 
{
	return PointList(*this) += inList;
}


// Size
std::string SVG::Size::getStringValue(void) const 
{
    ostringstream lStream;
    lStream << width << "," << height;
    return lStream.str();
}

std::string SVG::PointList::getStringValue(void) const {
    if(size() == 0 ) return string("");
    string lString((*this)[0].getStringValue());
    for ( unsigned i = 1; i < size(); i++ ) lString +=  string(" ") + (*this)[i].getStringValue();
    return lString;
}

