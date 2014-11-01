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

/*!\file PACC/SVG/Canvas.cpp
 * \brief Class methods for the SVG containers (Group, Frame, and Canvas).
 * \author Marc Parizeau and Michel Fortin, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 */

#include "PACC/SVG/Canvas.hpp"
#include "PACC/XML/Finder.hpp"
#include "PACC/XML/Document.hpp"
#include <iostream>
#include <sstream>

using namespace std;
using namespace PACC;

//! Pop canvas on viewer at address \c inHostPort with title \c inTitle and size \c inSize.
SVG::Canvas::Canvas(const string& inTitle, const Size& inSize, const string& inHostPort) 
: Document(inTitle, inSize), Socket::Cafe(inHostPort) 
{
	initCanvas();
}

//! Pop canvas on viewer at address \c inAddress:inPort with title \c inTitle, size \c inSize, and style \c inStyle.
SVG::Canvas::Canvas(const string& inTitle, const Size& inSize, const Style& inStyle, const string& inHostPort) 
: Document(inTitle, inSize, inStyle), Socket::Cafe(inHostPort) 
{
	initCanvas();
}

//! Pop canvas on viewer at address \c inAddress:inPort using document \c inDocument.
SVG::Canvas::Canvas(const Document& inDocument, const string& inHostPort) 
: Document(inDocument), Socket::Cafe(inHostPort) 
{
	initCanvas();
}

//! Assign frame \c inFrame to this canvas.
SVG::Canvas& SVG::Canvas::operator=(const Document& inDocument) 
{
	Document::operator=(inDocument);
	updateViewer();
	return *this;
}

//! Insert graphic primitive \c inGraphic into this canvas.
SVG::Canvas& SVG::Canvas::operator<<(const Primitive& inGraphic) 
{
	Group::operator<<(inGraphic); 
	updateViewer(); 
	return *this;
}

//! Erase all drwing elements.
void SVG::Canvas::clear(void) 
{
	Group::clear(); 
	updateViewer();
}

//! Set size of canvas to size \c inSize.
void SVG::Canvas::setSize(const Size& inSize) 
{
	Document::setSize(inSize);
	updateViewer();
}

//! Set frame size to width \c inwidth and height \c inHeight.
void SVG::Canvas::setSize(double inWidth, double inHeight) 
{
	setSize(Size(inWidth,inHeight));
}

//! Pop canvas window on viewer.
void SVG::Canvas::initCanvas(void) 
{
	try {
		// construct new canvas message
		ostringstream lStream;
		lStream << "NEWW        ";
		write(lStream);
		// send message
		sendMessage(lStream.str());
		// save window id
		receiveMessage(mWinID);
		// close viewer connection
		close();
	} catch(const Socket::Exception& inErr) {
		cerr << inErr.getMessage() << endl;
	}
}

//! Send canvas to viewer.
void SVG::Canvas::updateViewer(void)
{
	try {
		// reconnect to viewer
		connect(std::string("localhost:61250"));
		// construct update message
		ostringstream lStream;
		lStream << "REFR" << mWinID;
		write(lStream);
		// send message
		sendMessage(lStream.str());
		// close connection
		close();
	} catch(const Socket::Exception& inErr) {
		cerr << inErr.getMessage() << endl;
	}
}

//! Wait up to \c inMaxDelay seconds for the user to click a mouse button.
SVG::Point SVG::Canvas::waitForClick(int& outButtonClicked, double inMaxDelay)
{
	ostringstream lOutStream;
	lOutStream << "GCLK" << mWinID;
	sendMessage(lOutStream.str());
	string lMessage;
	receiveMessage(lMessage);
	Point lPos;
	istringstream lInStream(lMessage);
	lInStream >> lPos.x >> lPos.y >> outButtonClicked;
	lPos.y = getSize().height - lPos.y;
	return lPos;
}
