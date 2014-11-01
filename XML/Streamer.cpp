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
 * \file PACC/XML/Streamer.cpp
 * \brief Class methods for the %XML streamer.
 * \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 
 * $Revision: 1.37 $
 * $Date: 2005/10/05 09:15:08 $
 */

#include "XML/Streamer.hpp"
#include "Util/Assert.hpp"
#include <stdexcept>

using namespace std;
using namespace PACC;

/*!
*/
void XML::Streamer::closeTag(void)
{
	PACC_AssertM(!mTags.empty(), "Streamer::closeTag() no tag to close!");
	if(!mClosed) {
		if(mTags.top().second && mIndentAttributes && mOneAttribute) {
			mStream << endl << string((mTags.size()-1)*mIndentWidth, ' ');
		}
		mStream << "/>" << flush;
		mOneAttribute = false;
	} else {
		if(mTags.top().second == true) {
			mStream << endl << string((mTags.size()-1)*mIndentWidth, ' ');
		}
		mStream << "</" << mTags.top().first << ">" << flush;
	}
	mTags.pop();
	mClosed = true;
}

/*!  
\return A reference to the converted string.
Supported characters are '&', '<', '>', "'", and '"'.
*/
string& XML::Streamer::convertToQuotes(string& ioString, const string& inQuotes)
{
	string::size_type lPos = 0;
	while((lPos = ioString.find_first_of(inQuotes, lPos)) < ioString.size())
	{
		switch(ioString[lPos]) {
			case '&':
				ioString.replace(lPos++, 1, "&amp;");
				break;
			case '<':
				ioString.replace(lPos++, 1, "&lt;");
				break;
			case '>':
				ioString.replace(lPos++, 1, "&gt;");
				break;
			case '\'':
				ioString.replace(lPos++, 1, "&apos;");
				break;
			case '"':
				ioString.replace(lPos++, 1, "&quot;");
				break;
		}
	}
	return ioString;
}

/*! 
The header tag has the form <?xml version="1.0" encoding="ISO-8859-1"?>.
*/
void XML::Streamer::insertHeader(const string& inEncoding)
{
	mStream << "<?xml version=\"1.0\"";
	if(!inEncoding.empty()) mStream << " encoding=\"" << inEncoding << "\"";
	mStream << "?>" << flush;
}

/*!
The specified string is assumed not to contain any of the following special characters: '&', '<', or '>'. Argument \c inConvert controls whether these should be automatically converted to quotes ("&amp;", "&lt;", and "&gt;"). By default, no conversion is conducted.
 */
void XML::Streamer::insertStringContent(const string& inString, bool inConvert)
{	
	if(inString.empty()) return;
	if(!mClosed) {
		if(mTags.top().second && mIndentAttributes && mOneAttribute) {
			mStream << endl << string((mTags.size()-1)*mIndentWidth, ' ');
		}
		mStream << ">";
		mOneAttribute = false;
		mClosed = true;
	}
	if(!mTags.empty() && mTags.top().second == true) {
		mStream << endl << string(mTags.size()*mIndentWidth, ' ');
	}
	if(inConvert) {
		string lContent(inString);
		mStream << convertToQuotes(lContent, "&<>");
	} else {
		mStream << inString;
	}
}

/*!
By default, markup tags are indented by the number of columns specified in the class constructor. However, argument \c inIndent can be set to false to disactivate indentation for all embedded markup. 
 */
void XML::Streamer::openTag(const string& inName, bool inIndent)
{
	if(!mClosed) {
		if(mTags.top().second == true && mIndentAttributes && mOneAttribute) {
			mStream << endl << string((mTags.size()-1)*mIndentWidth, ' ');
		}
		mStream << ">";
		mOneAttribute = false;
	}
	if((mTags.empty() && inIndent) || (!mTags.empty() && mTags.top().second == true)) {
		mStream << endl << string(mTags.size()*mIndentWidth, ' ');
	}
	mStream << "<" << inName;
	if(!mTags.empty() && mTags.top().second == false) inIndent = false;
	mTags.push(pair<string, bool>(inName, inIndent));
	mClosed = false;
}
