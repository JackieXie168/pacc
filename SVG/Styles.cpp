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

/*!\file PACC/SVG/Styles.cpp
 * \brief Class methods for %SVG styles.
 * \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.1 $
 * $Date: 2005/06/08 18:46:50 $
 */

#include "SVG/Styles.hpp"

using namespace std;;
using namespace PACC;

// predefined styles
const SVG::FillRule SVG::FillRule::cNonZero("nonzero");
const SVG::FillRule SVG::FillRule::cEvenOdd("evenodd");
const SVG::Stroke SVG::Stroke::cNone(Color("none"));
const SVG::StrokeDashArray SVG::StrokeDashArray::cContinuous("none");
const SVG::StrokeDashArray SVG::StrokeDashArray::cDotted("1");
const SVG::StrokeDashArray SVG::StrokeDashArray::cDashed("3");
const SVG::StrokeLineCap SVG::StrokeLineCap::cButt("butt");
const SVG::StrokeLineCap SVG::StrokeLineCap::cRound("round");
const SVG::StrokeLineCap SVG::StrokeLineCap::cSquare("square");
const SVG::StrokeLineJoin SVG::StrokeLineJoin::cMiter("miter");
const SVG::StrokeLineJoin SVG::StrokeLineJoin::cRound("round");
const SVG::StrokeLineJoin SVG::StrokeLineJoin::cBevel("bevel");
const SVG::FontFamily SVG::FontFamily::cSerif("serif");
const SVG::FontFamily SVG::FontFamily::cSansSerif("sans-serif");
const SVG::FontStyle SVG::FontStyle::cNormal("normal");
const SVG::FontStyle SVG::FontStyle::cItalic("italic");
const SVG::FontStyle SVG::FontStyle::cOblique("oblique");
const SVG::FontWeight SVG::FontWeight::cNormal("normal");
const SVG::FontWeight SVG::FontWeight::cBold("bold");
const SVG::TextAnchor SVG::TextAnchor::cStart("start");
const SVG::TextAnchor SVG::TextAnchor::cMiddle("middle");
const SVG::TextAnchor SVG::TextAnchor::cEnd("end");
const SVG::TextDecoration SVG::TextDecoration::cNone("none");
const SVG::TextDecoration SVG::TextDecoration::cUnderline("underline");
const SVG::TextDecoration SVG::TextDecoration::cLineThrough("line-through");

SVG::StrokeDashArray::StrokeDashArray(const vector<float>& inDashArray) : XML::Attribute("stroke-dasharray", string()) 
{
   string lNumbers;
   for ( unsigned i = 0; i < inDashArray.size(); i++ ) lNumbers += String::convert(inDashArray[i]) + " ";
   getValue() = lNumbers;
}