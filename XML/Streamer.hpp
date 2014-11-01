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
 * \file PACC/XML/Streamer.hpp
 * \brief Class definition for the %XML streamer.
 * \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.36 $
 * $Date: 2005/06/04 04:43:52 $
 */

#ifndef PACC_XML_Streamer_hpp_
#define PACC_XML_Streamer_hpp_

#include <iostream>
#include <string>
#include <stack>

namespace PACC { 
   
   using namespace std;
   
   namespace XML {

/*! \brief Simple document streamer.
\author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
\ingroup XML

This class is for streaming %XML markup with automatic indentation. To add a new markup tag, use method Streamer::openTag, followed by calls to Streamer::insertAttribute to add attributes. Then call again Streamer::openTag to insert any embedded markup. Don't forget to close each tag by calling Streamer::closeTag. Here is a simple usage example:
\code
Streamer lStream(cout);
lStream.insertHeader();
lStream.openTag("MyTag");
lStream.insertAttribute("name", "value");
lStream.openTag("MySubTag1");
lStream.insertAttribute("x", 10);
lStream.insertAttribute("y", 20);
lStream.closeTag();
lStream.openTag("MySubTag2");
lStream.insertStringContent("this is my sub-tag number 2!");
lStream.closeTag();
lStream.closeTag();
\endcode
which will produce the following result:
\code
<?xml version="1.0" encoding="ISO-8859-1"?>
<MyTag name="value">
	<MySubTag1 x="10" y="20"/>
	<MySubTag2>
	  this is my sub-tag number 2!
	</MySubTag2>
</MyTag>
\endcode
*/
		class Streamer {
       public:
			//! Constructs object for streaming %XML markup into output stream \c inStream using an indentation width of \c inWidth.
			Streamer(ostream& inStream, unsigned int inWidth=2) : mStream(inStream), mIndentWidth(inWidth), mClosed(true), mIndentAttributes(false), mAttribute(false) {}

			//! Close the last opened markup tag.
			void closeTag(void);
			//!Return output stream of streamer.
			ostream& getStream(void) {return mStream;}
			//! Insert an attribute with name \c inName and integer value \c inValue into the current start tag.
			void insertAttribute(const string& inName, int inValue);
			//! Insert an attribute with name \c inName and unsigned integer value \c inValue into the current start tag.
			void insertAttribute(const string& inName, unsigned int inValue);
			//! Insert an attribute with name \c inName and float value \c inValue into the current start tag.
			void insertAttribute(const string& inName, double inValue);
			//! Insert an attribute with name \c inName and string value \c inValue into the current start tag.
			void insertAttribute(const string& inName, const string& inValue);
			//! Insert CDATA section \c inCDATA.
			void insertCDATA(const string& inCDATA);
			//! Insert comment \c inComment.
			void insertComment(const string& inComment);
			template<class Container>
				void insertContainer(const Container& inContainer, const string& inName="");
			//! Insert a valid xml declarative tag with encoding \c inEncoding (default is ISO-8859-1).
			void insertHeader(const string& inEncoding = "ISO-8859-1");
			//! Insert a primitive tag with tag name \c inTagName, attribute name \c inAttName, and floating point value \c inValue.
			void insertPrimitiveTag(double invalue, const string& inTagName="Float", const string& inAttributeName="v");
			//! Insert a primitive tag with tag name \c inTagName, attribute name \c inAttName, and integer value \c inValue.
			void insertPrimitiveTag(int invalue, const string& inTagName="Integer", const string& inAttributeName="v");
			//! Insert a primitive tag with tag name \c inTagName, attribute name \c inAttName, and string value \c inValue.
			void insertPrimitiveTag(string invalue, const string& inTagName="String", const string& inAttributeName="v");
			//! Insert string \c inString as content of current markup; convert characters '&', '<', or '>' to quotes if \c inConvert is true (default=false).
			void insertStringContent(const string& inContent, bool inConvert=false);
			//! Open new markup tag using name \c inName.
			void openTag(const string& inName);
			
			//! Set attribute indentation mode if argument \c inValue is true.
			void setAttributeIndentation(bool inValue) {mIndentAttributes = inValue;}
			//! Set width of markup indentation to \c inValue columns.
			void setIndentationWidth(unsigned int inValue) {mIndentWidth = inValue;}
			
			//! Insert an integer markup with value \c inValue into this streamer.
			Streamer& operator<<(int inValue);
			//! Insert a float markup with value \c inValue into this streamer.
			Streamer& operator<<(double inValue);
			//! Insert a string markup with value \c inValue into this streamer.
			Streamer& operator<<(const string& inValue);
			
			//! Insert any class \c inClass into this streamer, assuming that it defines a write function.
			template<class T> Streamer& operator<<(const T& inClass) {
				inClass.write(*this);
				return *this;
			}
      
       protected:
			ostream& mStream; //!< Output stream
			stack<string> mTags; //!< Stack of opened tags
			unsigned int mIndentWidth; //!< Width of markup indentation
			bool mClosed; //!< Indicates whether start tag has been closed
			bool mIndentAttributes; //!< Indicates whether attributes should be indented
			bool mAttribute; //!< Indicates whether the tag contains at least one attribute
			
			//! Convert special xml characters to quotes.
			static string& convertToQuotes(string& ioString, const string& inQuotes="&<>");		
      
		};

/*! \brief Inserts STL container \c inContainer using tag name \c inName.

If no tag name is specified, the container's content is inserted into the current tag. Assumes that the container's elements can insert themselves into a streamer using \c operator<<.
*/
		template<class Container>
			void Streamer::insertContainer(const Container& inContainer, //!< STL container to insert.
													 const string& inName //!< Name of tag.
													 ) {
				// insert optional tag name
				if(inName != "") openTag(inName);
				// iterate through container
				for(typename Container::const_iterator i = inContainer.begin(); i != inContainer.end(); ++i) {
					(*this) << (*i);
				}
				if(inName != "") closeTag();
			}

	} // end of XML namespace

} // end of PACC namespace

#endif // PACC_XML_Streamer_hpp_
