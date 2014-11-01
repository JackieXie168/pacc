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
 */

#include "PACC/SVG/Document.hpp"
#include "PACC/XML/Finder.hpp"
#include "PACC/XML/Document.hpp"
#include <iostream>
#include <sstream>
#include <stdexcept>

using namespace std;
using namespace PACC;

/*!
 * \defgroup SVGdoc Containers
 * \ingroup SVG
 * \brief Containers for graphics primitives.
 */

//! Construct a valid document with title \c inTitle, size \c inSize, and style \c inStyle.
SVG::Document::Document(const string& inTitle, const Size& inSize, 
						const Style& inStyle) 
: Frame("svg"), mTitle(inTitle) 
{
	setOrigin(0, 0);
	setSize(inSize);
	*this += inStyle; 
}

//! Set title of this canvas.
void SVG::Document::setTitle(const string& inTitle) 
{
	mTitle = inTitle;
}

//! Read this document from parser tree node \c inNode.
void SVG::Document::read(const XML::ConstIterator& inNode)
{
	if(!inNode) throw runtime_error("read() nothing to read!");
	XML::ConstFinder lFinder(inNode);
	XML::ConstIterator lPos = lFinder.find("/title");
	if(lPos && lPos->getType() == XML::eString) {
		mTitle = lPos->getValue();
	}
	lPos = lFinder.find("/svg/g/svg");
	if(!lPos) throw runtime_error("read() invalid document!");
	XML::Node::operator=(*lPos);
}

//! Serialize this document into stream \c outStream.
void SVG::Document::write(ostream& outStream) const 
{
	XML::Streamer lStream(outStream);
	lStream.insertHeader();
	lStream.openTag("svg");
	lStream.insertAttribute("width", getSize().width);
	lStream.insertAttribute("height", getSize().height);
	lStream.insertAttribute("xmlns", "http://www.w3.org/2000/svg");
	lStream.insertAttribute("version", "1.2");
	lStream.insertAttribute("baseProfile", "tiny");
	lStream.openTag("title", false);
	lStream.insertStringContent(mTitle);
	lStream.closeTag();
	Group lCoords;
	lCoords.setTransform(Scale(1,-1) + Translate(0,-getSize().height));
	lCoords << *this;
	lCoords.serialize(lStream);
	lStream.closeTag();
}

//!\brief Insert canvas \c inCanvas into output stream \c outStream.
//!\ingroup SVGdoc
ostream& PACC::operator<<(ostream& outStream, const SVG::Document& inDocument)
{
	inDocument.write(outStream);
	return outStream;
}

//!\brief Extract network \c outCanvas from input stream \c inStream.
//!\ingroup SVGdoc
istream& PACC::operator>>(istream& inStream, SVG::Document& outDocument)
{
	XML::Document lDocument;
	lDocument.parse(inStream);
	outDocument.read(lDocument.getFirstDataTag());
	return inStream;
}
