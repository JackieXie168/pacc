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
 * $Revision: 1.6 $
 * $Date: 2005/09/17 03:15:01 $
 */

#include "SVG/Canvas.hpp"
#include "XML/Finder.hpp"
#include <iostream>
#include <sstream>

using namespace std;
using namespace PACC;

/*!
*/
void SVG::Canvas::clear(void) 
{
	// find embedded group tag
	XML::Finder lFinder(this);
	XML::Iterator lPos = lFinder.find("/svg/g");
	((Group&)*lPos).clear();
}

/*!
 */
SVG::Canvas& SVG::Canvas::operator<<(const SVG::Primitive& inElement) 
{
	// find embedded group tag
	XML::Finder lFinder(this);
	XML::Iterator lPos = lFinder.find("/svg/g");
	// insert new element into internal group.
	((Group&)*lPos) << inElement;
	// send serialized canvas to viewer
	updateViewer();
	return *this;
}

/*!
 */
SVG::Canvas& SVG::Canvas::operator=(const SVG::Frame &inFrame) 
{
    Group::operator=(inFrame);
    updateViewer();
    return *this;
}

//!
string SVG::Canvas::getTitle(void) const 
{
	// find embedded title tag
	XML::Finder lFinder((XML::Node*)this);
	XML::Iterator lPos = lFinder.find("/svg/title");
	// return title value
	if(lPos && lPos->getFirstChild()) return lPos->getFirstChild()->getValue();
	else return "";
}

//! This method also adjusts the frame attributes, and connect to the viewer.
void SVG::Canvas::initCanvas(const string& inTitle) 
{
	// insert title
	setTitle(inTitle);
	// insert xmlns attribute
	setAttribute("xmlns", "http://www.w3.org/2000/svg");
	// remove position attributes
	removeAttribute("x");
	removeAttribute("y");
	// insert change of referential group
	SVG::Group lGroup;
	lGroup.setTransform(Scale(1, -1)+Translate(0, -String::convertToFloat(getAttribute("height"))));
	insertAsLastChild(new Node((Node&)lGroup));
	// allocate socket
	if(mSocket) delete mSocket;
	mSocket = new Socket::Cafe(Socket::Address(mPort, mAddress));
	// send serialize canvas to viewer
	ostringstream lStream;
	lStream << "NEWW        ";
	write(lStream);
	mSocket->sendMessage(lStream.str());
	mSocket->receiveMessage(mWinID);
}

//!
void SVG::Canvas::updateViewer(void) const
{
	ostringstream lStream;
	lStream << "REFR" << mWinID;
	write(lStream);
	mSocket->sendMessage(lStream.str());
}
	
//!
void SVG::Canvas::setTitle(const string& inTitle)
{
	// find embedded title tag
	XML::Finder lFinder((XML::Node*)this);
	XML::Iterator lPos = lFinder.find("/svg/title");
	// update title value
	if(lPos) {
		if(lPos->getFirstChild()) lPos->getFirstChild()->setValue(inTitle);
		else lPos->insertAsLastChild(new Node(inTitle, XML::eString));
	} else {
		lPos = insertAsLastChild(new Node("title"));
		lPos->insertAsLastChild(new Node(inTitle, XML::eString));
	}
}

/*!
 */
SVG::Point SVG::Canvas::waitForClick(int &outButtonClicked, double inMaxDelay) const
{
	ostringstream lOutStream;
	lOutStream << "GCLK" << mWinID;
	mSocket->sendMessage(lOutStream.str());
	string lMessage;
	mSocket->receiveMessage(lMessage);
	Point lPos;
	istringstream lInStream(lMessage);
	lInStream >> lPos.x >> lPos.y >> outButtonClicked;
	lPos.y = getSize().height - lPos.y;
	return lPos;
}
