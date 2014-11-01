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
 * $Revision: 1.51 $
 * $Date: 2007/03/03 01:54:21 $
 */

#ifndef PACC_XML_Streamer_hpp_
#define PACC_XML_Streamer_hpp_

#include "PACC/Util/Assert.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <stack>

namespace PACC { 
	
	using namespace std;
	
	namespace XML {
		
		/*! \brief Simple document streamer.
		\author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
		\ingroup XML
		
		This class is for streaming %XML markup with automatic indentation. To 
		add a new markup tag, use method Streamer::openTag, followed by calls to 
		Streamer::insertAttribute to add attributes. Then call again Streamer::openTag 
		to insert any embedded markup. Don't forget to close each tag by calling 
		Streamer::closeTag or Streamer::closeAll. 
		
		Here is a simple usage example:
		\code
		Streamer lStream(cout);
		lStream.insertHeader();
		lStream.openTag("MyTag");
		lStream.insertAttribute("name", "value");
		lStream.openTag("MySubTag1");
		lStream.insertAttribute("x", 10);
		lStream.insertAttribute("y", 20);
		lStream.closeTag();
		lStream.openTag("MySubTag2", false);
		lStream.insertStringContent("this is my sub-tag number 2!");
		lStream.closeTag();
		lStream.closeTag();
		\endcode
		which will produce the following result:
		\code
		<?xml version="1.0" encoding="ISO-8859-1"?>
		<MyTag name="value">
		<MySubTag1 x="10" y="20"/>
		<MySubTag2>this is my sub-tag number 2!</MySubTag2>
		</MyTag>
		\endcode
		*/
		class Streamer {
		 public:
			//! Constructs object for streaming %XML markup into output stream \c inStream using an indentation width of \c inWidth.
			Streamer(ostream& inStream, unsigned int inWidth=2) : mStream(inStream), mIndentWidth(inWidth), mClosed(true), mOneAttribute(false), mIndentAttributes(false) {}
			
			//! Close all opened markup tags.
			void closeAll(void);
			
			//! Close the last opened markup tag.
			void closeTag(void);
			
			//! Convert special xml characters to quotes.
			static string& convertToQuotes(string& ioString, const char* inQuotes="<>&'\"");
			
			//! Return precision of output stream for double values.
			unsigned int getPrecision(void) {return mStream.precision();}
			
			//!Return output stream of streamer.
			ostream& getStream(void) {return mStream;}
			
			//! Insert a string attribute with name \c inName and value \c inValue into the current start tag.
			void insertAttribute(const string& inName, const char* inValue);
				
			//! Insert a string attribute with name \c inName and value \c inValue into the current start tag.
			void insertAttribute(const string& inName, const string& inValue) {
				insertAttribute(inName, inValue.c_str());
			}
				
			/*!
			\brief Insert a template attribute with name \c inName and value \c inValue into the current start tag.

			This method assumes that typename \c Type knows how to insert itself into a stream using \c operator<<.
			 */
			template <typename Type>
				void insertAttribute(const string& inName, Type inValue) {
					if(mIndentAttributes && mTags.top().second) {
						mStream << endl << std::string(mTags.size()*mIndentWidth, ' ');
					} else mStream << " ";
					mStream << inName << "=\"" << inValue << "\"";
					mOneAttribute = true;
				}
			
			//! Insert CDATA section \c inCDATA (cannot contain any "]]>" substring).
			void insertCDATA(const string& inCDATA) {
				PACC_AssertM(inCDATA.find("]]>",0)==string::npos, "an XML CDATA cannot contain any ']]>' substring!");
				insertStringContent(string("<![CDATA[")+inCDATA+string("]]>"), false);
			}
			
			//! Insert comment \c inComment (cannot contain any "--" substring).
			void insertComment(const string& inComment) {
				PACC_AssertM(inComment.find("--",0)==string::npos, "an XML comment cannot contain any '--' substring!");
				insertStringContent(string("<!--")+inComment+string("-->"), false);
			}
			
			/*! \brief Insert STL container \c inObject into this streamer, using tag name \c inName.
				
				If no tag name is specified, the container's content is inserted into the current tag. Assumes that the container's elements can insert themselves into a streamer using \c operator<<.
				*/
			template <typename Container>
				void insertContainer(const Container& inObject, const string& inName="") {
					// insert optional tag name
					if(inName != "") openTag(inName);
					// iterate through container
					for(typename Container::const_iterator i = inObject.begin(); i != inObject.end(); ++i) {
						(*this) << (*i);
					}
					if(inName != "") closeTag();
				}
			
			//! Insert a valid xml declarative tag with encoding \c inEncoding (default is ISO-8859-1).
			void insertHeader(const string& inEncoding = "ISO-8859-1");
			
			/*! \brief Insert a primitive tag with tag name \c inTagName, attribute name \c inAttName, and value \c inValue.
				
				The format for this primitive tag is &lt;inTagName inAttName="inValue"/&gt;. This method assumes that typename \c Type knows how to insert itself into a stream using \c operator<<.
				*/
			template <typename Type>
				void insertPrimitiveTag(Type inValue, const string& inTagName, const string& inAttName="v") {
					openTag(inTagName);
					insertAttribute(inAttName, inValue);
					closeTag();
				}
			
			//! Insert string \c inString as content of current markup.
			void insertStringContent(const string& inContent, bool inConvert=true);
			
			//! Open new markup tag using name \c inName.
			void openTag(const string& inName, bool inIndent=true);
			
			//! Set attribute indentation mode if argument \c inValue is true.
			void setAttributeIndentation(bool inValue) {mIndentAttributes = inValue;}
			
			//! Set precision of output stream to \c inValue digits for double values.
			void setPrecision(unsigned int inValue) {mStream.precision(inValue);}

			//! Insert an integer markup with value \c inValue into this streamer.
			Streamer& operator<<(int inValue) {
				insertPrimitiveTag(inValue, "Integer");
				return *this;
			}
			
			//! Insert a float markup with value \c inValue into this streamer.
			Streamer& operator<<(double inValue) {
				insertPrimitiveTag(inValue, "Float");
				return *this;
			}
			
			//! Insert a string markup with value \c inValue into this streamer.
			Streamer& operator<<(const string& inValue) {
				insertPrimitiveTag(inValue, "String");
				return *this;
			}			
			
			//! Insert object \c inObject into this stream, assuming that it defines a write function.
			template<typename Type> 
				Streamer& operator<<(const Type& inObject) {
					inObject.write(*this);
					return *this;
				}
			
		 protected:
			ostream& mStream; //!< Output stream
			stack< pair<string, bool> > mTags; //!< Stack of opened tags
			unsigned int mIndentWidth; //!< Width of markup indentation
			bool mClosed; //!< Indicates whether start tag has been closed
			bool mOneAttribute; //!< Indicates whether the tag contains at least one attribute
			bool mIndentAttributes; //!< Indicates whether attributes should also be indented
		};
		
	} // end of XML namespace
	
} // end of PACC namespace

#endif // PACC_XML_Streamer_hpp_
