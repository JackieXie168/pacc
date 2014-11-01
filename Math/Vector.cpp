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
 *  \file   PACC/Math/Vector.cpp
 *  \brief  Method definitions for class Vector.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.5 $
 *  $Date: 2005/04/20 15:39:36 $
 */

#include "Math/Vector.hpp"
#include "Util/StringFunc.hpp"
#include <stdexcept>
#include <cmath>

using namespace std;
using namespace PACC;

//! Compute scalar product of this vector with vector \c inVector.
double Vector::operator*(const Vector& inVector) const
{
	PACC_AssertM(mRows == inVector.mRows, "vector mismatch for multiply!");
	double lResult = 0;
	for(unsigned int i = 0; i < mRows; ++i) lResult += (*this)[i] * inVector[i];
	return lResult;
}

//! Compute L1 norm for this vector.
double Vector::computeL1Norm(void) const
{
	PACC_AssertM(mRows > 0, "cannot compute L1 norm!");
	double lResult = 0;
	for(unsigned int i = 0; i < mRows; ++i) lResult += fabs((*this)[i]);
	return lResult;
}

//! Compute L2 norm for this vector.
double Vector::computeL2Norm(void) const
{
	PACC_AssertM(mRows > 0, "cannot compute L2 norm!");
	double lResult = 0;
	for(unsigned int i = 0; i < mRows; ++i) lResult += (*this)[i]*(*this)[i];
	return sqrt(lResult);
}

//! Compute square of L2 norm for this vector.
double Vector::computeL2Norm2(void) const
{
	PACC_AssertM(mRows > 0, "cannot compute L2 norm!");
	double lResult = 0;
	for(unsigned int i = 0; i < mRows; ++i) lResult += (*this)[i]*(*this)[i];
	return lResult;
}

//! Compute Linf norm for this vector.
double Vector::computeLinfNorm(void) const
{
	PACC_AssertM(mRows > 0, "cannot compute Linf norm!");
	double lResult = 0;
	for(unsigned int i = 0; i < mRows; ++i) if(lResult < fabs((*this)[i])) lResult = (*this)[i];
	return lResult;
}

/*! \brief Read from parse tree node \c inNode.

Vector elements must be enumerated in row order, and seperated by semi-columns (';'), comas (','), or white space (the semi-column is the recommended delimiter). For example:
\verbatim
<Vector name="My Vector" size="4">1;2;3;4</Vector>
\endverbatim
The number of elements must match the value of the "size" attribute.

This method also supports the following deprecated markup style:
\verbatim
<Vector name="My Vector">
  <Float v="1.2"/>
  <Float v="23.8"/>
  ...
</Vector>
\endverbatim
Any read error raises an std::runtime_error exception.
*/
string Vector::read(const XML::Iterator& inNode)
{
   if(!inNode) throw runtime_error("Vector::read() nothing to read!");
   clear();
	for(XML::Iterator lChild = inNode->getFirstChild(); lChild; ++lChild) {
		if(lChild->getType() == XML::eString) {
			// this is the recommended markup style
			istringstream lStream(lChild->getValue());
			Tokenizer lTokenizer(lStream);
			lTokenizer.setDelimiters(" \n\r\t,;", "");
			string lToken;
			while(lTokenizer.getNextToken(lToken)) {
				push_back(String::convertToFloat(lToken));
			}
		} else if(lChild->getType() == XML::eData && lChild->getValue() == "Float") {
			string lValue;
			if((lValue = lChild->getAttribute("v")) != "" || (lValue = lChild->getAttribute("value")) != "") push_back(String::convertToFloat(lValue));
			else if(lChild->getFirstChild() && lChild->getFirstChild()->getType() == XML::eString) push_back(String::convertToFloat(lChild->getFirstChild()->getValue()));
		}
	}
	mCols = 1;
	mRows = vector<double>::size();
	if(inNode->isDefined("size")) {
		unsigned int lSize = String::convertToInteger(inNode->getAttribute("size"));
		if(mRows != lSize) throwError("Vector::read() number of elements does not match the size attribute", inNode);
	}
   string lName = inNode->getAttribute("name");
	if(lName != "") mName = lName;
	return lName;
}

//! Write this vector into streamer \c outStream using tag name \c inTag (see Vector::read for format).
void Vector::write(XML::Streamer& outStream, const string& inTag) const
{
	outStream.openTag(inTag);
	if(mName != "") outStream.insertAttribute("name", mName);
	outStream.insertAttribute("size", mRows);
	if(size() > 0) {
		ostringstream lContent;
		for(unsigned int i = 0; i < mRows; ++i) {
			if(i != 0) lContent << ";";
			lContent << (*this)[i];
		}
		outStream.insertStringContent(lContent.str());
	}
	outStream.closeTag();
}

//! Insert vector \c inVector into output stream \c outStream.
ostream& PACC::operator<<(ostream &outStream, const Vector& inVector)
{
   XML::Streamer lStream(outStream);
   inVector.write(lStream);
   return outStream;
}

/*! \brief Extract vector \c outVector from %XML document \c inDocument.

This method uses the first data tag of the parse tree to read the vector. The corresponding tree root is then erased. Any read error throws a std::runtime_error.
*/
XML::Document& PACC::operator>>(XML::Document& inDocument, Vector& outVector)
{
   XML::Iterator lNode = inDocument.getFirstDataTag();
	outVector.read(lNode);
	inDocument.erase(lNode);
   return inDocument;
}
