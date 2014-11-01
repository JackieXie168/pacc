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

/*!\file PACC/SVG/Document.cpp
 * \brief Class methods for the SVG Document.
 * \author Marc Parizeau and Michel Fortin, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.2 $
 * $Date: 2007/02/24 19:31:38 $
 */

#include "PACC/SVG/Document.hpp"
#include "PACC/XML/Finder.hpp"
#include "PACC/XML/Document.hpp"
#include <iostream>
#include <sstream>

using namespace std;
using namespace PACC;

/*!
*/
void SVG::Document::read(const XML::ConstIterator& inNode)
{
	if(!inNode) throw runtime_error("read() nothing to read!");
	XML::ConstFinder lFinder(inNode);
	XML::ConstIterator lPos = lFinder.find("/svg/g/svg");
	if(!lPos) throw runtime_error("read() invalid document!");
	XML::Node::operator=(*lPos);
}

/*!
*/
void SVG::Document::write(ostream& outStream) const 
{
	XML::Streamer lStream(outStream);
	lStream.insertHeader();
	lStream.openTag("svg");
	lStream.insertAttribute("width", getSize().width);
	lStream.insertAttribute("height", getSize().height);
	lStream.insertAttribute("xmlns", "http://www.w3.org/2000/svg");
	lStream.openTag("title", false);
	lStream.insertStringContent(mTitle);
	lStream.closeTag();
	lStream.openTag("g");
	lStream.insertAttribute("transform", Scale(1,-1)+Translate(0, -getSize().height));
	serialize(lStream);
	lStream.closeTag();
	lStream.closeTag();
}

/*!
*/
ostream& PACC::operator<<(ostream &outStream, const SVG::Document& inDocument)
{
	inDocument.write(outStream);
	return outStream;
}

/*!
*/
istream& PACC::operator>>(istream &inStream, SVG::Document& outDocument)
{
	XML::Document lDocument;
	lDocument.parse(inStream);
	outDocument.read(lDocument.getFirstDataTag());
	return inStream;
}
