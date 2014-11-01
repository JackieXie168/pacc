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
 * $Revision: 1.13 $
 * $Date: 2008/04/17 21:05:44 $
 */

#include "PACC/SVG/Canvas.hpp"
#include "PACC/XML/Finder.hpp"
#include "PACC/XML/Document.hpp"
#include <iostream>
#include <sstream>

using namespace std;
using namespace PACC;

/*!
 */
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

/*!
 */
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

/*!
 */
SVG::Point SVG::Canvas::waitForClick(int &outButtonClicked, double inMaxDelay)
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
