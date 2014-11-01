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
 * \file PACC/XML/Document.cpp
 * \brief Class methods for the %XML document.
 * \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.45 $
 * $Date: 1970/05/08 22:59:39 $
 */

#include "PACC/XML/Document.hpp"
#include "PACC/XML/Streamer.hpp"
#include "PACC/Util/Assert.hpp"
#include <fstream>
#include <stdexcept>

using namespace std;
using namespace PACC;

/*! 
\return An iterator to the Added child

This method first allocates a new node of type \c inType and value \c inValue, and inserts this node as the last child of the referenced node. It then returns an iterator to the newly allocated node, which can be used to set tag attributes. For example, to add child tag "Mytag" to the node referenced by iterator \c lPos in document \c lDocument:
\code
Iterator lPos;
...
Iterator lChild = lDocument.addChild(lPos, "Mytag");
lChild->setAttribute("attribute", "value");
...
\endcode
*/
XML::Iterator XML::Document::addChild(const XML::Iterator& inPos, const string& inValue, NodeType inType)
{
	return attachChild(inPos, new Node(inValue, inType));
}

/*! 
\return An iterator to the new root.

This method first allocates a new node of type \c inType and value \c inValue, and inserts this node as the last root element of the document. It then returns an iterator to the newly allocated node, which can be used to set tag attributes. For example, to add root tag "Myroot" to document \c lDocument:
\code
Iterator lPos;
...
Iterator lChild = lDocument.addRoot(lPos, "Mytag");
lChild->setAttribute("attribute", "value");
...
\endcode
*/
XML::Iterator XML::Document::addRoot(const string& inValue, XML::NodeType inType)
{
	// allocate new node
	Node* lNode = new Node(inValue, inType);
	// insert at end of root list
	insertAsLastChild(lNode);
	return lNode;
}

/*! 
\return An iterator to the new sibling.

This method first allocates a new node of type \c inType and value \c inValue, and inserts this node in front of the referenced node. It then returns an iterator to the newly allocated node, which can be used to set tag attributes. For example, to add sibling tag "Mytag" to the node referenced by iterator \c lPos in document \c lDocument:
\code
Iterator lPos;
...
Iterator lChild = lDocument.addSibling(lPos, "Mytag");
lChild->setAttribute("attribute", "value");
...
\endcode
*/
XML::Iterator XML::Document::addSibling(const XML::Iterator& inPos, const string& inValue, XML::NodeType inType)
{
	PACC_AssertM(inPos, "Invalid iterator!");
	return attachSibling(inPos, new Node(inValue, inType));
}

/*! 
\return An iterator to the attached child.

This method inserts the pre-allocated node \c inChild (and its sub-tree) as the last child of the  node referenced by iterator \c inPos. Note that \c inPos is assumed to point within this document, and that the newly attached child becomes property of this document. 

\attention A document sub-tree needs to be detached (see Document::detach) before it can be move to another document or another branch of the same document (nodes are not reference counted). Otherwise, the subtree will be owned twiced, and deallocated by the first owner which is deleted. This can produce strange behaviors that are very difficult to debug. Beware!
*/
XML::Iterator XML::Document::attachChild(const XML::Iterator& inPos, XML::Node* inChild)
{
	PACC_AssertM(inPos, "Invalid iterator!");
	PACC_AssertM(inChild, "Cannot attach nul pointer!");
	inPos->insertAsLastChild(inChild);
	return inChild;
}

/*! 
\return An Iterator to the attached sibling.

This method inserts the pre-allocated node \c inSibling (and its sub-tree) as the sibling that precedes the node referenced by iterator \c inPos. Note that \c inPos is assumed to point with this document, and that the newly attached sibling becomes property of this document. 

\attention A document sub-tree needs to be detached (see Document::detach) before it can be move to another document or another branch of the same document (nodes are not reference counted). Otherwise, the subtree will be owned twiced, and deallocated by the first owner which is deleted. This can produce strange behaviors that are very difficult to debug. Beware!
*/
XML::Iterator XML::Document::attachSibling(const XML::Iterator& inPos, XML::Node* inSibling)
{
	PACC_AssertM(inPos, "Invalid iterator!");
	PACC_AssertM(inSibling, "Cannot attach nul pointer!");
	inPos->insertAsPreviousSibling(inSibling);
	return inSibling;
}

/*! 
\return A pointer to the detached node.

Tree nodes should never be owned by multiple document objects because no reference counting is implemented. This method can be used to remove node ownership from one document so that the corresponding sub-tree can be moved to another document. A detached sub-tree (node) can be re-attached to a document using methods Document::attachChild or Document::attachSibling.
*/
XML::Node* XML::Document::detach(const XML::Iterator& inPos)
{
	PACC_AssertM(inPos, "Invalid iterator!");
	return inPos->detachFromSiblingsAndParent();
}

/*! 
Upon return, iterator \c inPos is invalidated (should not be incremented, decremented, nor deferenced). For example, given document \c lDocument, the following code that iterates through the childs of the first root data tag is WRONG:
\code
...
Iterator lRoot = lDocument.getFirstDataTag();
// process all childs of root data tag.
for(Iterator lChild = lRoot->getFirstChild(); lChild; ++lChild) {
	// determine if this child should be erased
	...
	if(should_erase) lDocument.erase(lChild);
	// lChild is now invalid!
}
\endcode
The problem is that iterator \c lChild is invalidated by the call to the erase method. When the for-statement proceeds with operation \c ++lChild, the iterator becomes invalid and the loop may terminate early.

One way to circumvent this restriction is to use the postfix version of the increment operator in the method call. For example, the following code will work correctly:
\code
...
Iterator lChild = lRoot.getFirstChild();
while(lChild) {
	// process node information in order to determine if it should be erased
	...
	if(should_erase) lDocument.erase(lChild++);
	else ++lChild;
}
\endcode
*/
void XML::Document::erase(XML::Iterator inPos)
{
	// detach and delete
	delete inPos->detachFromSiblingsAndParent();
}

/*!
*/
void XML::Document::eraseRoots(void) 
{
	// delete all childs of the root node
	eraseChildren();
}

/*! 
An %XML document may contain three types of elements: declarative tags, special tags, and data tags. This method returns an iterator on the first data tag amongs the root elements of the document.
*/
XML::Iterator XML::Document::getFirstDataTag(void) 
{
	for(Iterator lTag = getFirstChild(); lTag; ++lTag) if(lTag->getType() == eData) return lTag;
	return 0;
}

/*! 
An %XML document may contain three types of elements: declarative tags, special tags, and data tags. This method returns an iterator on the first data tag amongs the root elements of the document.
*/
XML::ConstIterator XML::Document::getFirstDataTag(void) const
{
	for(ConstIterator lTag = getFirstChild(); lTag; ++lTag) if(lTag->getType() == eData) return lTag;
	return 0;
}

/*! 
An %XML document may contain three types of elements: declarative tags, special tags, and data tags. This method returns an iterator on the first element encountered in the document (usually the <?xml version="1.0"?> tag).
*/
XML::Iterator XML::Document::getFirstRoot(void) 
{
	return getFirstChild();
}

/*! 
An %XML document may contain three types of elements: declarative tags, special tags, and data tags. This method returns an iterator on the first element encountered in the document (usually the <?xml version="1.0"?> tag).
*/
XML::ConstIterator XML::Document::getFirstRoot(void) const
{
	return getFirstChild();
}

/*!
*/
void XML::Document::parse(const string& inFileName)
{
	// open file
	ifstream lStream(inFileName.c_str());
	if(!lStream.good()) throw runtime_error(string("Document::parse() unable to open file ")+inFileName);
	// parse file content
	parse(lStream, inFileName);
}

/*!
*/
void XML::Document::parse(istream& inStream, const string& inName)
{
	Tokenizer lTokenizer(inStream);
	lTokenizer.setStreamName(inName);
	eraseRoots();
	Node* lNode = 0;
	// parse all elements
	while((lNode=Node::parse(lTokenizer, mNoParseTags)) != NULL) insertAsLastChild(lNode);
	if(lTokenizer.peekNextChar() != -1) {
		// stream contains bad markup
		lTokenizer.setDelimiters("", "\n\r");
		Node lBad(string("</")+lTokenizer.getNextToken(), XML::eRoot);
		lBad.throwError(lTokenizer, "invalid markup");
	}
}

/*!
A negative value of \c inWidth disables all indentation (including line feeds). Argument \c inIndentAttributes controls the indentation of markup attributes. The default is to not indent attributes.
 */
void XML::Document::serialize(ostream& outStream, int inWidth, bool inIndentAttributes) const
{
	bool lIndent = (inWidth >= 0);
	Streamer lStream(outStream, (lIndent ? (unsigned int)inWidth : 0));
	lStream.setAttributeIndentation(inIndentAttributes);
	// retrieve first root
	ConstIterator lRoot = getFirstRoot();
	if(lRoot)
	{
		// check for xml header
		if(lRoot->getType() != eDecl) lStream.insertHeader();
		// serialize all roots
		while(lRoot) (lRoot++)->serialize(lStream, lIndent);
	}
}

/*!
The content of noparse markups will not be parsed, but simply read as a string. Noparse tags are useful for not wasting time parsing markup that is uninteresting for a given application. It is a compromise between the DOM and SAX models of XML parsers.
 
 \attention The node type for noparse markup is NodeType::eNoParse. Such a node is similar to NodeType::eString, but may embed some unparsed markup.
*/
void XML::Document::setNoParse(const string& inTag)
{
	mNoParseTags.insert(inTag);
}

/*!
*/
void XML::Document::unsetNoParse(const string& inTag)
{
	mNoParseTags.erase(inTag);
}

/*!
*/
ostream& PACC::operator<<(ostream &outStream, const XML::Document& inDocument)
{
	inDocument.serialize(outStream);
	return outStream;
}

/*!
*/
istream& PACC::operator>>(istream &inStream, XML::Document& outDocument)
{
	outDocument.parse(inStream);
	return inStream;
}
