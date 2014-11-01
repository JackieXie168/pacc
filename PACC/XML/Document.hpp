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
 * \file PACC/XML/Document.hpp
 * \brief Class definition for the %XML document.
 * \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.42 $
 * $Date: 1970/05/08 23:05:04 $
 */

#ifndef PACC_XML_Document_hpp_
#define PACC_XML_Document_hpp_

#include "PACC/XML/Iterator.hpp"
#include <queue>

namespace PACC { 

	using namespace std;

	namespace XML {

		/*! \brief %Document container and parser.
		\author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
		\ingroup XML
   
		This class is for parsing and manipulating xml documents. It respects the basic %XML syntax, except for the following items:
		-# It does not deal with character encodings;
		-# It does not support DTD (external doctypes are ignored; internal ones are not allowed);
		.
		In other words, it can read most data coded in %XML, but cannot validate its syntax, nor translate its character encodings (this translation must be done by the application). Also, document type declarations should not contain any embedded elements (external references are ignored).
		
		The document is internaly represented as a list (forest) of m-ary rooted trees. These trees can represent different types of elements:
		-# Regular markup data (XML::eData)
		-# Literal strings (XML::eString)
		-# %XML declaration of the form <?xml ... ?> (XML::eDecl)
		-# Comments of the form &lt;!-- ... --> (XML::eComment)
		-# CDATA sections of the form <![CDATA[ ... ]]> (XML::eCDATA)
		-# Processing instructions of the form <? ... ?> (XML::ePI)
		-# Special elements of the form <! ... > (XML::eSpecial)
		. 
		Tree nodes are derived from a map of attribute/value string pairs and contain a list of child node pointers. An Iterator class is defined in order to iterate through these nodes. Iterators wrap pointers to node elements of the document tree with ++ and -- operators to iterate through sibling nodes. For example:
\code
Document lDocument;
lDocument.parse("Myfile.xml");
Iterator lTag;
// search the root elements of the document for the first data tag
for(lTag = lDocument.getFirstRoot(); lTag; ++lTag) if(lTag->getType() != eData) break;
...
\endcode
		can be used to iterate over the root elements of document \c lDocument in order to locate its first root markup tag. The same result can be achieved using helper method Document::getFirstDataTag. The type of a node can be retrieved using method Node::getType.
		
		Markup data can contain embedded child tags which can be iterated over just like any other node. For instance, to iterate over the childs of the first markup element of document \c lDocument:
\code
Iterator lRoot = lDocument.getFirstDataTag();
for(Iterator lChild = lRoot->getFirstChild(); lChild; ++lChild) {
	// process each child
	...
}
\endcode

		A document can also be constructed from scratch, or modified, using methods Document::addChild, Document::addRoot, Document::addSibling, Document::attachChild, Document::attachSibling, Document::detach, Document::erase, and Document::eraseRoots. It can be serialized into a stream using method Document::serialize, and parsed from a stream using Document::parse. To search for specific markup data, one may use an instance of the Finder class.
		*/
		class Document : protected Node {
			public:   
			//! Constructs an empty document.
			Document(void) {}
			
			//! Constructs a document from an input stream.
			Document(istream& inStream, const string& inName="") {parse(inStream, inName);}
			
			//! Delete document.
			~Document(void) {eraseRoots();}
			
			//! Add a new child to the node referenced by iterator \c inPos.
			Iterator addChild(const Iterator& inPos, const string& inValue, NodeType inType=eData);
			
			//! Add a new root node in this document.
			Iterator addRoot(const string& inValue, NodeType inType=eData);
			
			//! Add a new sibling in front of the node referenced by iterator \c inPos.
			Iterator addSibling(const Iterator& inPos, const string& inValue, NodeType inType=eData);
			
			//! Attach node \c inChild as a child of the node referenced by iterator \c inPos.
			Iterator attachChild(const Iterator& inPos, Node* inChild);
			
			//! Attach node \c inNode as a sibling in front of the node referenced by iterator \c inPos.
			Iterator attachSibling(const Iterator& inPos, Node* inSibling);
			
			//! Detach the sub-tree rooted at the node referenced by iterator \c inPos.
			Node* detach(const Iterator& inPos);
			
			//! Delete the node referenced by iterator \c inPos.
			void erase(Iterator inPos);
			
			//! Delete all root elements of this document.
			void eraseRoots(void);
			
			//! Return an iterator on the first data tag of this document.
			Iterator getFirstDataTag(void);
			
			//! Return a const iterator on the first data tag of this document.
			ConstIterator getFirstDataTag(void) const;
			
			//! Return an iterator on the first root element of this document.
			Iterator getFirstRoot(void);
			
			//! Return a const iterator on the first root element of this document.
			ConstIterator getFirstRoot(void) const;
			
			//! Parse a document contained in the file named \c inFileName; 
			void parse(const string& inFileName);
			
			//! Parse a document from input stream \c inStream, using string \c inName as stream name for error messages.
			void parse(istream& inStream, const string& inName="");
			
			//! Serialize document into an output stream \c outStream, using indentation of \c inWidth characters.
			void serialize(ostream& outStream, int inWidth=2, bool inIndentAttributes=false) const;
			
			//! Add \c inTag to the list of tag names for which content should not be parsed for this document. 
			void setNoParse(const string& inTag);
			
			//! Remove \c inTag from the list of tags for which content should not be parsed for this document. 
			void unsetNoParse(const string& inTag);
			
			protected:
			set<string> mNoParseTags; //!< Tag names for which content should not be parsed
			
		};
		
	} // end of XML namespace
	
	//! Insert document \c inDocument into output stream \c outStream.
	ostream& operator<<(ostream &outStream, const XML::Document& inDocument);
	//! Extract document \c outDocument from input stream \c inStream.
	istream& operator>>(istream &inStream, XML::Document& outDocument);

} // end of PACC namespace

#endif // PACC_XML_Document_hpp_
