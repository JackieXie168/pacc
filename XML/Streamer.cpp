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

 * $Revision: 1.32 $
 * $Date: 2005/05/12 05:01:34 $
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
	// indentation depends on the number of tags currently on the stack.
	unsigned int lIndent = mIndentWidth*(mTags.size()-1);
   if(!mClosed) {
		if(mIndentAttributes && mAttribute) mStream << "\n" << string(lIndent, ' ');
		mStream << "/>" << endl;
		mAttribute = false;
   } else  {
		mStream << string(lIndent, ' ') << "</" << mTags.top() << ">" << endl;
	}
   mClosed = true;
   mTags.pop();
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
				ioString.replace(lPos, 1, "&amp;");
				lPos += 5;
				break;
			case '<':
				ioString.replace(lPos, 1, "&lt;");
				lPos += 4;
				break;
			case '>':
				ioString.replace(lPos, 1, "&gt;");
				lPos += 4;
				break;
			case '\'':
				ioString.replace(lPos, 1, "&apos;");
				lPos += 6;
				break;
			case '"':
				ioString.replace(lPos, 1, "&quot;");
				lPos += 6;
				break;
		}
	}
	return ioString;
}

/*!
*/
void XML::Streamer::insertAttribute(const string& inName, int inValue)
{
	if(mIndentAttributes) {
		// indentation depends on the number of tags currently on the stack.
		unsigned int lIndent = mIndentWidth*mTags.size();
		mStream << "\n" << string(lIndent, ' ');
	} else mStream << " ";
   mStream << inName << "=\"" << inValue << "\"";
	mAttribute = true;
}

/*!
*/
void XML::Streamer::insertAttribute(const string& inName, unsigned int inValue)
{
	if(mIndentAttributes) {
		// indentation depends on the number of tags currently on the stack.
		unsigned int lIndent = mIndentWidth*mTags.size();
		mStream << "\n" << string(lIndent, ' ');
	} else mStream << " ";
   mStream << inName << "=\"" << inValue << "\"";
	mAttribute = true;
}

/*!
*/
void XML::Streamer::insertAttribute(const string& inName, double inValue)
{
	if(mIndentAttributes) {
		// indentation depends on the number of tags currently on the stack.
		unsigned int lIndent = mIndentWidth*mTags.size();
		mStream << "\n" << string(lIndent, ' ');
	} else mStream << " ";
   mStream << inName << "=\"" << inValue << "\"";
	mAttribute = true;
}

/*!
*/
void XML::Streamer::insertAttribute(const string& inName, const string& inValue)
{
	if(mIndentAttributes) {
		// indentation depends on the number of tags currently on the stack.
		unsigned int lIndent = mIndentWidth*mTags.size();
		mStream << "\n" << string(lIndent, ' ');
	} else mStream << " ";
	string lValue(inValue);
   mStream << inName << "=\"" << convertToQuotes(lValue, "&<>\"") << "\"";
	mAttribute = true;
}

/*!
*/
void XML::Streamer::insertCDATA(const string& inCDATA)
{
	insertStringContent(string("<![CDATA[")+inCDATA+string("]]>"));
}

/*!
*/
void XML::Streamer::insertComment(const string& inComment)
{
	insertStringContent(string("<!--")+inComment+string("-->"));
}

/*! 
The header tag as the form <?xml version="1.0" encoding="ISO-8859-1"?>.
*/
void XML::Streamer::insertHeader(const string& inEncoding)
{
   mStream << "<?xml version=\"1.0\"";
   if(!inEncoding.empty()) mStream << " encoding=\"" << inEncoding << "\"";
   mStream << "?>" << endl;
}

/*! 
If tag name is omitted, default name "Float" is used. If attribute name is omitted, default name "v" is used. The format for this primitive tag is thus <inTagName inAttName="inValue"/>.
*/
void XML::Streamer::insertPrimitiveTag(double inValue, const string& inTagName, const string& inAttName)
{
   openTag(inTagName);
   insertAttribute(inAttName, inValue);
   closeTag();
}

/*! 
If tag name is omitted, default name "Integer" is used. If attribute name is omitted, default name "v" is used. The format for this primitive tag is thus <inTagName inAttName="inValue"/>.
*/
void XML::Streamer::insertPrimitiveTag(int inValue, const string& inTagName, const string& inAttName)
{
   openTag(inTagName);
   insertAttribute(inAttName, inValue);
   closeTag();
}

/*! 
If tag name is omitted, default name "String" is used. If attribute name is omitted, default name "v" is used. The format for this primitive tag is thus <inTagName inAttName="inValue"/>.
*/
void XML::Streamer::insertPrimitiveTag(string inValue, const string& inTagName, const string& inAttName)
{
   openTag(inTagName);
   insertAttribute(inAttName, inValue);
   closeTag();
}

/*!
*/
void XML::Streamer::insertStringContent(const string& inString, bool inConvert)
{
	// indentation depends on the number of tags currently on the stack.
	unsigned int lIndent = mIndentWidth*mTags.size();
   if(!mClosed) {
		if(mIndentAttributes && mAttribute) mStream << "\n" << string(lIndent-mIndentWidth, ' ');
		mStream << ">" << endl;
		mAttribute = false;
	}
	mStream << string(lIndent, ' ');
   mClosed = true;
	if(!inConvert) mStream << inString << endl;
	else {
		// make copy
		string lContent(inString);
		mStream << convertToQuotes(lContent, "&<>") << endl;
	}
}

/*!
*/
void XML::Streamer::openTag(const string& inName)
{
	// indentation depends on the number of tags currently on the stack.
	unsigned int lIndent = mIndentWidth*mTags.size();
   if(!mClosed) {
		if(mIndentAttributes && mAttribute) mStream << "\n" << string(lIndent-mIndentWidth, ' ');
		mStream << ">" << endl;
		mAttribute = false;
	}
	mStream << string(lIndent, ' ');
   mStream << "<" << inName;
   mTags.push(inName);
   mClosed = false;
}

/*!
*/
XML::Streamer& XML::Streamer::operator<<(int inValue)
{
   insertPrimitiveTag(inValue);
   return *this;
}

/*!
*/
XML::Streamer& XML::Streamer::operator<<(double inValue)
{
   insertPrimitiveTag(inValue);
   return *this;
}

/*!
*/
XML::Streamer& XML::Streamer::operator<<(const string& inValue)
{
   insertPrimitiveTag(inValue);
   return *this;
}
