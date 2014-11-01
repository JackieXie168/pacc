/*
 *  Portable Agile C++ Classes (PACC)
 *  Copyright (C) 2001-2004 by Marc Parizeau
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
 * \file   PACC/Math/Vector.cpp
 * \brief  Method definitions for class Vector.
 * \author Marc Parizeau and Christian Gagn&eacute;, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.16 $
 * $Date: 2007/02/24 19:33:15 $
 */

#include "PACC/Math/Vector.hpp"
#include "PACC/Util/StringFunc.hpp"
#include <stdexcept>

using namespace std;
using namespace PACC;

/*!
 This method will try to interpret the input node as a matrix (see Matrix::read).
 For example, the following defines a vector of size 4:
 \verbatim
 <Vector name="My Vector" size="4">1;2;3;4</Vector>
 \endverbatim
 Note that the tag name (here "Vector") is irrelevant; any name can be used. The 
 method either returns the value of the "name" attribute, if present, or an empty 
 string, otherwise. Attribute "size"is optional, but must be 
 coherent with the parsed matrix, if present. Otherwise, an std::runtime_error 
 exception will be raised.

 This method also supports the following deprecated markup style:
 \verbatim
 <Vector name="My Vector">
   <Float v="1.2"/>
   <Float v="23.8"/>
   ...
 </Vector>
 \endverbatim
 */
string Vector::read(const XML::ConstIterator& inNode)
{
	if(!inNode) throw runtime_error("Vector::read() nothing to read!");
	try {
		// try the standard matrix format
		Matrix::read(inNode);
		if(mCols != 1 && (mCols != 0 || mRows != 0)) 
			throwError("Vector::read() must a single column!", inNode);
		if(inNode->isDefined("size")) {
			int lSize = String::convertToInteger(inNode->getAttribute("size"));
			if((int)mRows != lSize) throwError("Vector::read() invalid 'size' attribute!", inNode);
		}
	} catch(const runtime_error& inError) {
		// try the deprecated format
		clear();
		for(XML::ConstIterator lChild = inNode->getFirstChild(); lChild; ++lChild) {
			if(lChild->getType() == XML::eData && lChild->getValue() == "Float") {
				string lValue;
				if((lValue = lChild->getAttribute("v")) != "" || (lValue = lChild->getAttribute("value")) != "") push_back(String::convertToFloat(lValue));
				else if(lChild->getFirstChild() && lChild->getFirstChild()->getType() == XML::eString) push_back(String::convertToFloat(lChild->getFirstChild()->getValue()));
			}
		}
		mCols = 1;
		mRows = vector<double>::size();
		if(inNode->isDefined("size")) {
			int lSize = String::convertToInteger(inNode->getAttribute("size"));
			if((int)mRows != lSize) throwError("Vector::read() invalid size attribute", inNode);
		}
		if(inNode->isDefined("name")) mName = inNode->getAttribute("name");
	}
	return mName;
}

/*!
 See Vector::read for a description of the write format. By default, the precision 
 of the output is set to 15 digits. This value can be changed using method
 Matrix::setWritePrecision.
 */
void Vector::write(XML::Streamer& outStream, const string& inTag) const
{
	PACC_AssertM(mCols == 1, "Vector::write() invalid vector!");
	outStream.openTag(inTag, false);
	if(mName != "") outStream.insertAttribute("name", mName);
	outStream.insertAttribute("size", mRows);
	if(size() > 0) {
		ostringstream lContent;
		lContent.precision(mPrec);
		for(unsigned int i = 0; i < mRows; ++i) {
			if(i != 0) lContent << ";";
			lContent << (*this)[i];
		}
		outStream.insertStringContent(lContent.str());
	}
	outStream.closeTag();
}

/*!
*/
ostream& PACC::operator<<(ostream &outStream, const Vector& inVector)
{
	XML::Streamer lStream(outStream);
	inVector.write(lStream);
	return outStream;
}

/*! 
This method uses the first data tag of the parse tree to read the vector. The corresponding tree root is then erased. Any read error throws a std::runtime_error.
*/
XML::Document& PACC::operator>>(XML::Document& inDocument, Vector& outVector)
{
	XML::Iterator lNode = inDocument.getFirstDataTag();
	outVector.read(lNode);
	inDocument.erase(lNode);
	return inDocument;
}
