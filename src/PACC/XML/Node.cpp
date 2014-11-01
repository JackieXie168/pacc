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
 * \file PACC/XML/Node.cpp
 * \brief Class methods for the %XML parse tree node.
 * \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.43 $
 * $Date: 2008/04/17 21:17:03 $
 */

#include "PACC/XML/Node.hpp"
#include "PACC/XML/Iterator.hpp"
#include "PACC/XML/Streamer.hpp"
#include "PACC/Util/Assert.hpp"
#include <sstream>
#include <stdexcept>
#include <cstring>

using namespace std;
using namespace PACC;

XML::Node::DefaultQuotes XML::Node::smMap;

/*!
*/
XML::Node::Node(void) : mType(eRoot) {
	mParent = mFirstChild = mLastChild = mPrevSibling = mNextSibling = NULL;
}

/*!
*/
XML::Node::Node(const string& inValue, XML::NodeType inType) : mType(inType) {
	(*this)[""] = inValue; 
	mParent = mFirstChild = mLastChild = mPrevSibling = mNextSibling = NULL;
}

/*!
*/
XML::Node::Node(const string& inValue, const XML::AttributeList& inAttrList) : AttributeList(inAttrList), mType(eData) {
	(*this)[""] = inValue;
	mParent = mFirstChild = mLastChild = mPrevSibling = mNextSibling = NULL;
}

/*!
*/
XML::Node::Node(const XML::Node& inNode) : AttributeList() {
	mParent = mFirstChild = mLastChild = mPrevSibling = mNextSibling = NULL;
	operator=(inNode);
}

/*!
This method recursively deletes all of its children. 
 */
XML::Node::~Node(void) {
	// delete all child nodes
	eraseChildren();
	// detach from parent and siblings
	detachFromSiblingsAndParent();
	// cleanup node pointers
	mParent = mFirstChild = mLastChild = mPrevSibling = mNextSibling = NULL;
}

/*! 
\return A reference to this node
\attention The copied tree must not be a sub-tree of this node. Otherwise, the internal tree structure will become corrupted.
*/
XML::Node& XML::Node::operator=(const Node& inRoot)
{
	// do not self assign!
	if(&inRoot == this) return *this;
	// delete all child nodes
	XML::Iterator lChild = getFirstChild();
	while(lChild) delete &(*(lChild++));
	// fix child pointers
	mFirstChild = mLastChild = NULL;
	// assign type and attributes
	mType = inRoot.mType;
	map<string,string>::operator=(inRoot);
	// copy all children of inRoot
	for(XML::ConstIterator lNode = inRoot.getFirstChild(); lNode; ++lNode) {
		// allocate and copy node
		Node* lChildNode = new Node(*lNode);
		// is this the first child?
		if(mFirstChild == NULL) mFirstChild = mLastChild = lChildNode;
		else {
			//adjust sibling pointers
			mLastChild->mNextSibling = lChildNode;
			lChildNode->mPrevSibling = mLastChild;
			mLastChild = lChildNode;
		}
		// adjust parent pointer
		lChildNode->mParent = this;
	}
	return *this;
}

/*!
 */
XML::Node::DefaultQuotes::DefaultQuotes(void) {
	// initialize quote list
	(*this)["amp"] = '&';
	(*this)["lt"] = '<';
	(*this)["gt"] = '>';
	(*this)["apos"] = '\'';
	(*this)["quot"] = '"';
}

/*! 
\return A reference to the converted string.
The default quotes are "&amp;", "&lt;", "&gt;", "&apos;", and "&quot;". Argument \c ioMap can be used to specify any conversion table. 
*/
string& XML::Node::convertFromQuotes(string& ioString, map<string,char>& ioMap)
{
	string::size_type lStart, lEnd = 0;
	while((lStart = ioString.find('&', lEnd)) < ioString.size() && 
				(lEnd = ioString.find(';', lStart)) < ioString.size())
	{
		string lToken = ioString.substr(lStart+1, lEnd-lStart-1);
		if(ioMap.find(lToken) != ioMap.end()) {
			ioString[lStart] = ioMap[lToken];
			ioString.erase(lStart+1, lEnd-lStart);
			lEnd = lStart+1;
		}
	}
	return ioString;
}

/*!\return A pointer to this node.
This method removes this node from its parent tree. The list of sibling nodes is repaired accordingly.
*/
XML::Node* XML::Node::detachFromSiblingsAndParent(void) {
	// adjust sibling list
	if(mPrevSibling) mPrevSibling->mNextSibling = mNextSibling;
	if(mNextSibling) mNextSibling->mPrevSibling = mPrevSibling;
	if(mParent) {
		// adjust parent first and last child pointers
		if(mParent->mFirstChild == this) mParent->mFirstChild = mNextSibling;
		if(mParent->mLastChild == this) mParent->mLastChild = mPrevSibling;
	}
	mPrevSibling = mNextSibling = mParent = NULL;
	return this;
}

/*!
*/
void XML::Node::eraseChildren(void)
{
	// delete all child nodes
	XML::Iterator lChild = getFirstChild();
	while(lChild) delete &(*(lChild++));
}

/*!\return The number of child nodes.
*/
unsigned int XML::Node::getChildCount(void) const {
	unsigned int lCount = 0;
	for(ConstIterator lChild = getFirstChild(); lChild; ++lChild) ++lCount;
	return lCount;
}

/*!\return A pointer to the inserted child node.
*/
XML::Node* XML::Node::insertAsLastChild(XML::Node* inChild) {
	PACC_AssertM(inChild, "Cannot add null pointer node");
	PACC_AssertM(!inChild->mParent && !inChild->mPrevSibling && !inChild->mNextSibling, "Node must be detached before it can be added!");
	// is this new child the first?
	if(mFirstChild == NULL) mFirstChild = inChild;
	else {
		// insert after last
		inChild->mPrevSibling = mLastChild;
		mLastChild->mNextSibling = inChild;
	}
	// adjust parent pointers
	inChild->mParent = this;
	mLastChild = inChild;
	return inChild;
}

/*!\return A pointer to the inserted sibling node.
*/
XML::Node* XML::Node::insertAsPreviousSibling(XML::Node* inSibling) {
	PACC_AssertM(inSibling, "Cannot insert null pointer node");
	PACC_AssertM(!inSibling->mParent && !inSibling->mPrevSibling && !inSibling->mNextSibling, "Node must be detached before it can be inserted!");
	// is this new sibling the first?
	if(mPrevSibling == NULL) {
		inSibling->mNextSibling = this;
		mPrevSibling = inSibling;
		// adjust first child of parent
		if(mParent) mParent->mFirstChild = inSibling;
	} else {
		// this node is neither the first or the last
		mPrevSibling->mNextSibling = inSibling;
		inSibling->mPrevSibling = mPrevSibling;
		inSibling->mNextSibling = this;
		mPrevSibling = inSibling;
	}
	// adjust parent pointer
	inSibling->mParent = mParent;
	return inSibling;
}

/*!\return A node pointer to the parsed element. 
Any tag name defined in \c inNoParseTags will be treated as if its content is a string token (content will not be parsed).
*/
XML::Node* XML::Node::parse(PACC::Tokenizer& inTokenizer, const set<string>& inNoParseTags)
{
	Node* lNode = NULL;
	// look for start tag
	string lToken;
	inTokenizer.setDelimiters("", "<");
	if(!inTokenizer.getNextToken(lToken)) return 0;
	// remove any leading white space
	size_type lPos = lToken.find_first_not_of(" \t\r\n");
	if(lPos == string::npos) {
		if(!inTokenizer.getNextToken(lToken)) return 0;
	} else if(lPos > 0) lToken.erase(0, lPos);
	if(lToken[0] == '<') {
		// check for end tag
		if(inTokenizer.peekNextChar() == '/') {
			// found end tag; 
			inTokenizer.setDelimiters("", "/");
			inTokenizer.getNextToken(lToken);
			return 0;
		}
		// found start tag
		lNode = new Node;
		lNode->parseStartTag(inTokenizer, lToken);
		if(lToken[0] == '/') {
			// found end tag; next token must be '>'
			inTokenizer.setDelimiters("", ">");
			if(!inTokenizer.getNextToken(lToken)) lNode->throwError(inTokenizer, "unexpected eof");
			if(lToken[0] != '>') lNode->throwError(inTokenizer, "invalid start tag");
		} else if(lNode->getType() == eData) {
			// either read or parse tag content
			if(inNoParseTags.find((*lNode)[""]) != inNoParseTags.end()) {
				lNode->readContentAsString(inTokenizer);
			} else {
				Node* lChild;
				// parse all child
				while((lChild=parse(inTokenizer, inNoParseTags)) != NULL) lNode->insertAsLastChild(lChild);
				// test for valid end tag
				inTokenizer.setDelimiters("", " \t\n\r>");
				if(!inTokenizer.getNextToken(lToken)) lNode->throwError(inTokenizer, "unexpected eof");
				if(lToken != (*lNode)[""]) lNode->throwError(inTokenizer, "invalid end tag");
			}
			// next token must be '>'
			inTokenizer.setDelimiters(" \t\n\r", ">");
			if(!inTokenizer.getNextToken(lToken)) lNode->throwError(inTokenizer, "unexpected eof");
			if(lToken[0] != '>') lNode->throwError(inTokenizer, "invalid end tag");
		}
		// else node is not markup
	} else {
		// found a simple string node
		lNode = new Node;
		lNode->mType = eString;
		// remove any ending white space
		lPos = lToken.find_last_not_of(" \t\r\n");
		PACC_AssertM(lPos != string::npos, "Internal error!");
		if(lPos < lToken.size()-1) lToken.resize(lPos+1);
		// convert basic quotes
		(*lNode)[""] = convertFromQuotes(lToken);
	}
	return lNode;
}

/*! 
Ending token is returned through argument \c outToken.
*/
void XML::Node::parseAttributeList(PACC::Tokenizer& inTokenizer, string& outToken)
{
	inTokenizer.setDelimiters(" \t\n\r", "=/?>");
	// next token should be an attribute name
	if(!inTokenizer.getNextToken(outToken)) throwError(inTokenizer, "unexpected eof");
	// parse all attributes
	while(outToken[0] != '>' && outToken[0] != '/' && outToken[0] != '?')
	{
		if(outToken[0] == '=') throwError(inTokenizer, "missing attribute name");
		// ok, found an attribute name!
		string lName = outToken;
		// next token should be '='
		inTokenizer.setDelimiters(" \t\n\r", "=");
		if(!inTokenizer.getNextToken(outToken) || outToken[0] != '=') 
			throwError(inTokenizer, "invalid attribute");
		inTokenizer.setDelimiters(" \t\n\r", "'\"");
		// next token must be '"' or "'"
		if(!inTokenizer.getNextToken(outToken)) throwError(inTokenizer, "unexpected eof");
		string lValue;
		switch(outToken[0]) {
			case '\'':
				inTokenizer.setDelimiters("", "'");
				if(!inTokenizer.getNextToken(outToken)) throwError(inTokenizer, "unexpected eof");
					if(outToken[0] != '\'') {
						lValue = outToken;
						if(!inTokenizer.getNextToken(outToken)) throwError(inTokenizer, "unexpected eof");
					}
						break;
			case '"':
				inTokenizer.setDelimiters("", "\"");
				if(!inTokenizer.getNextToken(outToken)) throwError(inTokenizer, "unexpected eof");
					if(outToken[0] != '"') {
						lValue = outToken;
						if(!inTokenizer.getNextToken(outToken)) throwError(inTokenizer, "unexpected eof");
					}
						break;
			default:
				throwError(inTokenizer, "invalid attribute value");
		}
		// insert attribute
		(*this)[lName] = convertFromQuotes(lValue);
		inTokenizer.setDelimiters(" \t\n\r", "=/?>");
		if(!inTokenizer.getNextToken(outToken)) throwError(inTokenizer, "unexpected eof");
	}
}

/*! 
This method assumes that token "<" has already been read. It returns the ending token through argument \c outToken.
*/
void XML::Node::parseStartTag(PACC::Tokenizer& inTokenizer, string& outToken)
{
	// parse tag name
	inTokenizer.setDelimiters("", " \t\n\r/>");
	if(!inTokenizer.getNextToken(outToken)) throwError(inTokenizer, "unexpected eof");
	if(outToken.find_first_of(" \t\n\r/>") != string::npos) throwError(inTokenizer, "invalid start tag");
	string& lValue = (*this)[""];
	switch(outToken[0]) {
		case '!':
			if(outToken.size() >= 3 && outToken[1] == '-' && outToken[2] == '-') {
				// process comment
				mType = eComment;
				outToken.erase(0, 3);
				inTokenizer.setDelimiters("", ">");
				do {
					int lSize = outToken.size();
					if(lSize > 2 && outToken[lSize-2] == '-' && outToken[lSize-1] == '-') {
						lValue += outToken.erase(lSize-2, 2);
						break;
					} else lValue += outToken;
				} while(inTokenizer.getNextToken(outToken));
				if(!inTokenizer.getNextToken(outToken)) throwError(inTokenizer, "unexpected eof");
			} else if(outToken.size() >= 8 && memcmp(outToken.data()+1, "[CDATA[", 7) == 0) {
				// process cdata section
				mType = eCDATA;
				outToken.erase(0, 8);
				inTokenizer.setDelimiters("", ">");
				do {
					int lSize = outToken.size();
					if(lSize >= 2 && outToken[lSize-2] == ']' && outToken[lSize-1] == ']') {
						lValue += outToken.erase(lSize-2, 2);
						break;
					} else lValue += outToken;
				} while(inTokenizer.getNextToken(outToken));
				if(!inTokenizer.getNextToken(outToken)) throwError(inTokenizer, "unexpected eof");
			} else {
				// process special element (doctype, attribute, etc.)
				mType = eSpecial;
				lValue = outToken.erase(0, 1);
				inTokenizer.setDelimiters("", ">");
				if(!inTokenizer.getNextToken(outToken)) throwError(inTokenizer, "unexpected eof");
				if(outToken[0] != '>') {
					lValue += outToken;
					if(!inTokenizer.getNextToken(outToken)) throwError(inTokenizer, "unexpected eof");
				}
			}
			break;
		case '?':
			if(outToken == "?xml") {
				// process xml declaration
				mType = eDecl;
				lValue = outToken.erase(0, 1);
				parseAttributeList(inTokenizer, outToken);
				if(outToken[0] != '?') throwError(inTokenizer, "invalid xml declaration");
				if(!inTokenizer.getNextToken(outToken)) throwError(inTokenizer, "unexpected eof");
				if(outToken[0] != '>') throwError(inTokenizer, "invalid xml declaration");
			} else {
				// process special processing instruction
				mType = ePI;
				lValue = outToken.substr(1, outToken.size()-1);
				inTokenizer.setDelimiters("", "?>");
				while(inTokenizer.getNextToken(outToken)) {
					if(outToken[0] == '?') {
						if(!inTokenizer.getNextToken(outToken)) throwError(inTokenizer, "unexpected eof");
						if(outToken[0] == '>') break;
						else lValue += '?';
					} 
					lValue += outToken;
				}
				if(outToken.empty()) throwError(inTokenizer, "unexpected eof");
			}
			break;
		default:
			// process data markup
			mType = eData;
			lValue = outToken;
			parseAttributeList(inTokenizer, outToken);
	}
}

/*!
*/
void XML::Node::readContentAsString(PACC::Tokenizer& inTokenizer)
{
	// create child node
	Node* lChild = new Node;
	insertAsLastChild(lChild);
	lChild->setType(eNoParse);
	// parse until end tag
	inTokenizer.setDelimiters("", "<>");
	string lToken;
	int lCount = 1;
	const string& lTag = (*this)[""];
	string& lString = (*lChild)[""];
	while(lCount > 0) {
		// check every start tag
		if(!inTokenizer.getNextToken(lToken)) throwError(inTokenizer, string("unexpected eof"));
		if(lToken[0] == '<') {
			if(!inTokenizer.getNextToken(lToken)) throwError(inTokenizer, string("unexpected eof"));
			if(lToken[0] == '/' && memcmp(lToken.data()+1, lTag.data(), lTag.size()) == 0) --lCount;
			else if(lToken[lToken.size()-1] != '/' && memcmp(lToken.data(), lTag.data(), lTag.size()) == 0) ++lCount;
			if(lCount > 0) {
				lString += "<";
				lString += lToken;
			}
		} else lString += lToken;
	}
	// remove any leading white space
	size_type lPos = lString.find_first_not_of(" \t\r\n");
	if(lPos == string::npos) {
		// string is all white space
		lString.resize(0);
	} else {
		// erase leading white space
		lString.erase(0, lPos);
		// remove any ending white space
		lPos = lString.find_last_not_of(" \t\r\n");	
		PACC_AssertM(lPos != string::npos, "Internal error!");
		// erase trailing white space
		if(lPos < lString.size()-1) lString.resize(lPos+1);
	}
}

/*!
Argument \c inIndent is used to control indentation. By default (\c inIndent=true), the sub-tree rooted by this node will be serialized with indentation. If \c inIndent=false, then the node will be serialized without any form of indentation (including line feeds).
 */
void XML::Node::serialize(XML::Streamer& outStream, bool inIndent) const
{
	switch(mType)
	{
		case eCDATA:
		{
			outStream.insertCDATA(getValue());
			break;
		}
		case eComment: 
		{
			outStream.insertComment(getValue());
			break;
		}
		case eData:
		{
			// check for tag with single string content
			ConstIterator lChild = getFirstChild();
			if(lChild && lChild->mType == eString && !lChild->getNextSibling()) {
				// disable indentation
				outStream.openTag(getValue(), false);
			} else {
				outStream.openTag(getValue(), inIndent);
			}
			// serialize attribute list
			for(map<string,string>::const_iterator i = begin(); i != end(); ++i) {
				if(i->first != "") outStream.insertAttribute(i->first, i->second);
			}
			// serialize child nodes
			while(lChild) (lChild++)->serialize(outStream, inIndent);
			outStream.closeTag();
			break;
		}
		case eNoParse:
		{
			outStream.insertStringContent(getValue(), false);
			break;
		}
		case ePI: 
		{
			string lValue = string("<?") + getValue() + string("?>");
			outStream.insertStringContent(lValue, false);
			break;
		}
		case eSpecial:
		{
			string lValue = string("<!") + getValue() + string(">");
			outStream.insertStringContent(lValue, false);
			break;
		}
		case eString:
		{
			outStream.insertStringContent(getValue(), true);
			break;
		}
		case eDecl: 
		{
			string lValue = "<?xml version=\"";
			if(isDefined("version")) lValue += getAttribute("version");
			else lValue += "1.0";
			lValue += "\"";
			if(isDefined("encoding")) lValue += " encoding=\"" + getAttribute("encoding") + "\"";
			lValue += "?>";
			outStream.insertStringContent(lValue, false);
			break;
		}
		default:
		{
			PACC_AssertM(false, "Unknown node type!");
		}
	}
}

/*!
*/
void XML::Node::throwError(PACC::Tokenizer& inTokenizer, const string& inMessage) const
{
	ostringstream lStream;
	lStream << "\nXML parse error";
	if(inTokenizer.getStreamName() != "")
		lStream << " in file \"" << inTokenizer.getStreamName() << "\",";
	lStream << " at line ";
	lStream << inTokenizer.getLineNumber();
	switch(mType) {
		case eCDATA: lStream << "\nfor CDATA \""; break;
		case eComment: lStream << "\nfor comment \""; break;
		case eData: lStream << "\nfor markup \""; break;
		case ePI: lStream << "\nfor processing instruction \""; break;
		case eRoot: lStream << "\nfor root element \""; break;
		case eSpecial: lStream << "\nfor special element \""; break;
		case eString: lStream << "\nfor literal string \""; break;
		case eDecl: lStream << "\nfor declaration \""; break;
		default: lStream << "\nfor unknown element \""; break;
	}
	if(getValue().size() < 40) lStream << getValue() << "\": " << inMessage;
	else lStream << getValue().substr(0,40) << "...\": " << inMessage;
	throw runtime_error(lStream.str());
}

/*!
*/
ostream& PACC::operator<<(ostream &outStream, const XML::Node& inNode)
{
	XML::Streamer lStream(outStream);
	inNode.serialize(lStream);
	return outStream;
}

/*!
*/
istream& PACC::operator>>(istream &inStream, XML::Node& outNode)
{
	Tokenizer lTokenizer(inStream);
	XML::Node* lNode = XML::Node::parse(lTokenizer, set<string>());
	outNode = *lNode;
	delete lNode;
	return inStream;
}
