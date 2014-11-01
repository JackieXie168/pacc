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
 * \file PACC/XML/Node.hpp
 * \brief Class definition for the %XML parse tree node.
 * \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.38 $
 * $Date: 2007/09/15 01:29:01 $
 */

#ifndef PACC_XML_Node_hpp_
#define PACC_XML_Node_hpp_

#include "PACC/Util/Tokenizer.hpp"
#include "PACC/XML/Attribute.hpp"
#include <map>
#include <set>
#include <string>

namespace PACC { 
	
	using namespace std;
	
	namespace XML {
		
		/*! \brief Types of parse tree node.
		\author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
		\ingroup XML
		*/
		enum NodeType {
			eCDATA, //!< CDATA section (<![CDATA[ ... ]]>)
			eComment, //!< Comment (<!-- ... -->).
			eData, //!< Regular data markup element.
			eNoParse, //!< Unparsed markup content.
			eDecl, //!< %XML declaration (<?xml ... ?>).
			ePI, //!< Processing instruction (<? ... ?>).
			eRoot, //!< %Document root (internal use only).
			eSpecial, //!< Special element (<! ... >).
			eString //!< Literal string.
		};
		
		// forward declarations
		class Iterator;
		class ConstIterator;
		class Streamer;
		
		/*! 
			\brief %Node of the document tree.
			\author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
			\ingroup XML
			
			A tree node represents an %XML element. Method Node::getType is used to retrieve the element type. This can be a regular data markup (XML::eData), an xml declaration (XML::eDecl), a processing instruction (XML::ePI), a comment (XML::eComment), a CDATA section (XML::eCDATA), a special element for document type declaration (XML::eSpecial), or a literal string (XML::eString). Method Node::getValue is used to retrieve different values depending on the node type. For regular markups, this method returns the tag name. For comments, CDATA sections, processing instructions, and other special elements, it returns the literal string of these elements. For the xml declaration, it simply returns "xml". It should be noted that internal DTDs are not supported by the parser (document type nodes cannot embed other nodes).
			
			Markup data elements can have content represented by child nodes. A Node can parse itself from a stream Tokenizer. Any parse error throws a \c runtime_error exception. Method Node::getFirstChild is used to retrieve an Iterator on the first child of this node. Method Node::getParent is used to retrieve an Iterator on the parent of this node. An Iterator is used to iterate on sibling nodes.
			
			Nodes are also derived from a map of attribute name/value pairs that can be fetched and set using methods Node::getAttribute and Node::setAttribute. Finally, a node can serialize itself into an %XML Streamer.
			*/
		class Node : public AttributeList {
			public:
			//! Construct empty root node.
			Node(void);
			
			//! Construct node of type \c inType (default NodeType::eTag) with value \c inValue.
			Node(const string& inValue, NodeType inType=eData);
			
			//! Construct node of type \c eData with value \c inValue and attribute list \c inAttrList.
			Node(const string& inValue, const AttributeList& inAttrList);
			
			//! Copy constructor: make deep copy of node \c inNode.
			Node(const Node& inNode);
			
			//! Delete the sub-tree rooted by this node.
			~Node(void);
			
			//! Make deep copy of the sub-tree rooted by node \c inRoot. 
			Node& operator=(const Node&);
			
			//! Return an iterator pointing to the first child of this node.
			Node* getFirstChild(void) {return mFirstChild;}
			
			//! Return a const iterator pointing to the first child of this node.
			const Node* getFirstChild(void) const {return mFirstChild;}
			
			//! Return an iterator pointing to the last child of this node.
			Node* getLastChild(void) {return mLastChild;}
			
			//! Return a const iterator pointing to the last child of this node.
			const Node* getLastChild(void) const {return mLastChild;}
			
			//! Return an iterator pointing to the next sibling of this node.
			Node* getNextSibling(void) {return mNextSibling;}
			
			//! Return a const iterator pointing to the next sibling of this node.
			const Node* getNextSibling(void) const {return mNextSibling;}
			
			//! Return an iterator pointing to the previous sibling of this node.
			Node* getPreviousSibling(void) {return mPrevSibling;}
			
			//! Return a const iterator pointing to the previous sibling of this node.
			const Node* getPreviousSibling(void) const {return mPrevSibling;}
			
			//! Return an iterator pointing to the parent of this node.
			Node* getParent(void) {return mParent;}
			
			//! Return a const iterator pointing to the parent of this node.
			const Node* getParent(void) const {return mParent;}
			
			//! Returns the node type (see enum XML::NodeType).
			const NodeType& getType(void) const {return mType;}
			
			//! Returns the node value (tag name for NodeType::eData).
			const string& getValue(void) const {return getAttribute("");}   
			
			//! Set the node type (see enum XML::NodeType).
			void setType(NodeType inType) {mType = inType;}         
			
			//! set node value (tag name for nodes of type NodeType::eTag).
			void setValue(const string& inValue) {setAttribute("", inValue);}         
			
			//! Detach this node from its siblings and parent.
			Node* detachFromSiblingsAndParent(void);
			
			//! Delete all children.
			void eraseChildren(void);
			
			//! Return the number of child nodes.
			unsigned int getChildCount(void) const;
			
			//! Insert node \c inChild after the last child of this node.
			Node* insertAsLastChild(Node* inChild);
			
			//! Insert node \c inSibling as the sibling that preceeds this node.
			Node* insertAsPreviousSibling(Node* inSibling);
			
			//! Parse next tag using stream tokenizer \c inTokenizer.
			static Node* parse(Tokenizer& inTokenizer, const set<string>& inNoParseTags);
			
			//! Serialize this node into %XML streamer \c outStream.
			void serialize(Streamer& outStream, bool inIndent=true) const;
			
			protected:
			NodeType mType; //!< Type of node.
			Node* mParent; //!< Pointer to parent node.
			Node* mFirstChild; //!< Pointer to first child node.
			Node* mLastChild; //!< Pointer to last child node.
			Node* mPrevSibling; //!< Pointer to previous sibling node.
			Node* mNextSibling; //!< Pointer to next sibling node.
			
			class DefaultQuotes : public std::map<std::string,char> {
			public:
				DefaultQuotes(void);
			};
			
			static DefaultQuotes smMap; //!< Conversion map for translating quotes.
			
			//! Convert basic xml quotes into ascii characters.
			static string& convertFromQuotes(string& ioString, map<string,char>& ioMap=smMap);
			
			//! Parse the attribute list using stream tokenizer \c inTokenizer.
			void parseAttributeList(Tokenizer& inTokenizer, string& outToken);
			
			//! Parse in stream tokenizer \c inTokenizer the start tag of this node.
			void parseStartTag(Tokenizer& inTokenizer, string& outToken);
			
			//! Read in stream tokenizer \c inTokenizer the content of this node (current tag) as a single string (do not parse content).
			void readContentAsString(Tokenizer& inTokenizer);
			
			//! Throw runtime error with message \c inMessage using tokenizer \c inTokenizer.
			void throwError(Tokenizer& inTokenizer, const string& inMessage) const;
			
			friend class Iterator;
			friend class ConstIterator;
			friend class Document;
			
		};
		
	} // end of XML namespace 
	
	//! Insert node \c inNode into output stream \c outStream.
	ostream& operator<<(ostream &outStream, const XML::Node& inNode);
	//! Extract node \c outNode from input stream \c inStream.
	istream& operator>>(istream &inStream, XML::Node& outNode);
	
} // end of PACC namespace

#endif // PACC_XML_Node_hpp_
