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
 * \file PACC/Util/Tokenizer.cpp
 * \brief Class methods for the input stream tokenizer.
 * \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.14 $
 * $Date: 2005/05/12 04:58:07 $
 */

#include "Util/Tokenizer.hpp"
#include "Util/Assert.hpp"
#include <stdexcept>

using namespace std;
using namespace PACC;

/*! \brief Retrieve the next character from the default input stream.
\return True if not end-of-stream.

This method returns the next input character through argument \c outChar. It also removes the character from the input stream. To simply peek at the character without removing it from the stream, use method Tokenizer::peekNextChar instead. 
*/
bool Tokenizer::getNextChar(char& outChar)
{
   PACC_AssertM(mStream, "undefined input stream!");
	if(!mBufCount) {
		// fill input read buffer
		mStream->read(mBufPtr=mBuffer, sizeof(mBuffer));
		mBufCount = mStream->gcount();
		if(mBufCount == 0) return false;
	}
	outChar = *(mBufPtr++); --mBufCount;
	return true;
}

/*! \brief Parse next token of default input stream.
\return String of next token found.

This method finds the next token in the default input stream, after removing any leading white space. An empty token (string) means that end of stream was reached.
\attention This method is depricated.
*/
string Tokenizer::getNextToken(void)
{
   string lToken;
   getNextToken(lToken);
   return lToken;
}

/*! \brief Parse next token of default input stream.
 \return True if a valid token was found, false otherwise.

 This method finds the next token in the default input stream, after removing any leading white space. The token is returned through argument \c outToken. An empty token (string) means that end of stream was reached.
 */
bool Tokenizer::getNextToken(string& outToken)
{
	PACC_AssertM(mStream, "undefined input stream!");
	unsigned char lChar;
	// get rid of leading white space
	do {
		if(!mBufCount) {
			// fill input read buffer
			mStream->read(mBufPtr=mBuffer, sizeof(mBuffer));
			mBufCount = mStream->gcount();
			if(mBufCount == 0) {
				outToken.clear();
				return false;
			}
		}
		lChar = *(mBufPtr++); --mBufCount;
		if(lChar == '\n') ++mLine;
	} while(mDelimiters[lChar] == 1);
	outToken = lChar;
	// append until next white space or single char token
	char lOutBuffer[1024];
	while(!mDelimiters[lChar] && mBufPtr != mBuffer) {
		unsigned lOutCount = 0;
		while(lOutCount < sizeof(lOutBuffer)) {
			if(!mBufCount) {
				// fill input read buffer
				mStream->read(mBufPtr=mBuffer, sizeof(mBuffer));
				mBufCount = mStream->gcount();
				if(mBufCount == 0) break;
			}
			lChar = *(mBufPtr++); --mBufCount;
			if(mDelimiters[lChar]) {
				// put character back into buffer
				--mBufPtr; ++mBufCount;
				break;
			}
			lOutBuffer[lOutCount++] = lChar;
			// check for end-of-line counter
			if(lChar == '\n') ++mLine;
		}
		outToken.append(lOutBuffer, lOutCount);
	}
	return !outToken.empty();
}

//! Return single character delimiters.
string Tokenizer::getSingleCharTokens(void) const 
{
   string lSingleCharTokens;
   for(unsigned int i=0; i < 256; ++i) if(mDelimiters[i] == 2) lSingleCharTokens += (char) i;
   return lSingleCharTokens;
}

//! Return white space delimiters.
string Tokenizer::getWhiteSpace(void) const 
{
   string lWhiteSpace;
   for(unsigned int i=0; i < 256; ++i) if(mDelimiters[i] == 1) lWhiteSpace += (char) i;
   return lWhiteSpace;
}

/*! \brief Return the next character from the default input stream.
\return True if not end-of-stream.

This method returns the next character through argument \c outChar, without removing it from the input stream. 
*/
bool Tokenizer::peekNextChar(char& outChar)
{
   PACC_AssertM(mStream, "undefined input stream!");
	if(!mBufCount) {
		// fill input read buffer
		mStream->read(mBufPtr=mBuffer, sizeof(mBuffer));
		mBufCount = mStream->gcount();
		if(mBufCount == 0) return false;
	}
	outChar = *mBufPtr;
	return true;
}

/*! \brief Set white space and single character delimiters.

The white space and single character delimiters are set to the characters contained in strings \c inWhiteSpace and \c inSingleCharTokens, respectively. The white space characters delimit tokens but are not tokens themselves. Single character tokens are tokens that delimit other tokens.
*/
void Tokenizer::setDelimiters(const string &inWhiteSpace, const string &inSingleCharTokens) 
{
   memset(mDelimiters, 0, sizeof(mDelimiters));
   for(string::const_iterator i = inWhiteSpace.begin(); i != inWhiteSpace.end(); ++i) {
      mDelimiters[(unsigned)*i] = 1;
   }
   for(string::const_iterator i = inSingleCharTokens.begin(); i != inSingleCharTokens.end(); ++i) {
		PACC_AssertM(mDelimiters[(unsigned)*i] == 0, "a delimiter cannot be both white space and single char token!");
      mDelimiters[(unsigned)*i] = 2;
   }
}
