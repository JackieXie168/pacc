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
 * $Revision: 1.31 $
 * $Date: 2008/10/29 21:55:14 $
 */

#include "PACC/Util/Tokenizer.hpp"
#include "PACC/Util/Assert.hpp"
#include <stdexcept>
#include <cstdlib>
#include <cstring>

using namespace std;
using namespace PACC;

/*!
The internal read buffer size can be set with argument \c inBufSize (default=1024). This buffer can also be disactivated by setting this argument to 0. The internal read buffer can greatly accelerate the parse of the stream. A size between 512 and 1024 appears to give good results in most circumstances. 

 \attention It should be noted that the use of such a buffer implies that the stream must be fully parsed by this tokenizer, because there is no way to put it's content back into the stream.
 */
Tokenizer::Tokenizer(unsigned int inBufSize) 
: mLine(1), mName(), mStream(0), mBuffer(0), mBufSize(0), mBufPtr(0), mBufCount(0), mTokens()
{
	setDelimiters(" \t\n\r", "");
	setBufferSize(inBufSize);
}

/*!
The internal read buffer size can be set with argument \c inBufSize (default=1024). This buffer can also be disactivated by setting this argument to 0. The internal read buffer can greatly accelerate the parse of the stream. A size between 512 and 1024 appears to give the best results in most circumstances. 

 \attention It should be noted that the use of such a buffer implies that the stream must be fully parsed by this tokenizer, because there is no way to put it's content back into the stream.
 */
Tokenizer::Tokenizer(istream& inStream, unsigned int inBufSize) 
: mLine(1), mName(), mStream(&inStream), mBuffer(0), mBufSize(0), mBufPtr(0), mBufCount(0), mTokens()
{
	setDelimiters(" \t\n\r", "");
	setBufferSize(inBufSize);
}

/*!
 */
Tokenizer::~Tokenizer(void) {
	delete[] mBuffer;
}

/*!
\return String of next token found.
 
 This method finds the next token in the default input stream, after removing any leading white space. An empty token (string) means that end of stream was reached.
 \attention This method is deprecated.
 */
string Tokenizer::getNextToken(void)
{
	string lToken;
	getNextToken(lToken);
	return lToken;
}

/*!
\return True if a valid token was found, false otherwise.
 
 This method finds the next token in the default input stream, after removing any leading white space. The token is returned through argument \c outToken. An empty token (string) means that end of stream was reached.
 */
bool Tokenizer::getNextToken(string& outToken)
{
	PACC_AssertM(mStream, "undefined input stream!");
	if(!mTokens.empty()) {
		// use putback tokens if available
		outToken = mTokens.top();
		mTokens.pop();
	} else if(mBufSize == 0) {
		// DO NOT use the input read buffer
		register unsigned char lChar;
		// get rid of leading white space
		do {
			lChar = mStream->get();
			if(mStream->eof()) {
				outToken.resize(0);
				return false;
			}
			if(lChar == '\n') ++mLine;
		} while(mDelimiters[lChar] == eWhiteSpace);
		outToken = lChar;
		// append until next white space or single char token
		char lOutBuffer[100];
		while(mDelimiters[lChar] == 0 && !mStream->eof()) {
			unsigned lOutCount = 0;
			while(lOutCount < sizeof(lOutBuffer)) {
				lChar = mStream->get();
				if(mStream->eof()) break;
				if(mDelimiters[lChar] != 0) {
					// put character back into stream
					mStream->putback(lChar);
					break;
				}
				lOutBuffer[lOutCount++] = lChar;
				// check for end-of-line counter
				if(lChar == '\n') ++mLine;
			}
			outToken.append(lOutBuffer, lOutCount);
		}
	} else {
		// otherwise, use the input read buffer
		register unsigned char lChar;
		// get rid of leading white space
		do {
			if(mBufCount == 0 && fillBuffer() == 0) {
				outToken.resize(0);
				return false;
			}
			lChar = *(mBufPtr++); --mBufCount;
			if(lChar == '\n') ++mLine;
		} while(mDelimiters[lChar] == eWhiteSpace);
		outToken = lChar;
		// append until next white space or single char token
		char lOutBuffer[100];
		while(mDelimiters[lChar] == 0 && mBufPtr != mBuffer) {
			unsigned lOutCount = 0;
			while(lOutCount < sizeof(lOutBuffer)) {
				if(mBufCount == 0 && fillBuffer() == 0) break;
				lChar = *(mBufPtr++); --mBufCount;
				if(mDelimiters[lChar] != 0) {
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
	}
	return !outToken.empty();
}

/*!
 */
string Tokenizer::getSingleCharTokens(void) const 
{
	string lSingleCharTokens;
	for(unsigned int i=0; i < 256; ++i) if(mDelimiters[i] == 2) lSingleCharTokens += (char) i;
	return lSingleCharTokens;
}

/*!
 */
string Tokenizer::getWhiteSpace(void) const 
{
	string lWhiteSpace;
	for(unsigned int i=0; i < 256; ++i) if(mDelimiters[i] == 1) lWhiteSpace += (char) i;
	return lWhiteSpace;
}

/*! \return -1 if end-of-stream.
 
 This method returns the next character without removing it from the input stream. 
 */
int Tokenizer::peekNextChar(void)
{
	PACC_AssertM(mStream, "undefined input stream!");
	// check for putback tokens
	if(!mTokens.empty()) return mTokens.top()[0];
	else if(mBufSize == 0) return mStream->peek();
	else {
		// otherwise, use buffer
		if(mBufCount == 0 && fillBuffer() == 0) return -1;
		return *mBufPtr;
	}
}

/*!
 This method uses a stack to store tokens that will be returned by Tokenizer::getNextToken in LIFO order.
 
 \attention Any number of tokens can be put back, but take note that these WILL NOT be parsed again, if the user decides to change delimiters.
 */
void Tokenizer::putbackToken(const string& inToken) 
{
	PACC_AssertM(!inToken.empty(), "cannot put back an empty string!");
	mTokens.push(inToken);
}

/*!
 The minimum buffer size is set to 10. A smaller buffer size will disable the use of the internal read buffer.
 
\attention This method should be called prior to the first call of method Tokenizer::getNextToken, because it is an error to resize a buffer that is not empty. Method Tokenizer::setStream should be called explicitely to flush the buffer .
 */
void Tokenizer::setBufferSize(unsigned int inSize)
{
	delete[] mBuffer;
	if(inSize < 10) inSize = 0;	
	if(inSize > 0) mBuffer = new char[inSize];
	else mBuffer = 0;
	mBufSize = inSize;
	mBufCount = 0;
}

/*!
The white space and single character delimiters are set to the characters contained in strings \c inWhiteSpace and \c inSingleCharTokens, respectively. The white space characters delimit tokens but are not tokens themselves. Single character tokens are tokens that delimit other tokens.
 */
void Tokenizer::setDelimiters(const string &inWhiteSpace, const string &inSingleCharTokens) 
{
	memset(mDelimiters, 0, sizeof(mDelimiters));
	for(string::const_iterator i = inWhiteSpace.begin(); i != inWhiteSpace.end(); ++i) {
		mDelimiters[(unsigned)*i] = eWhiteSpace;
	}
	for(string::const_iterator i = inSingleCharTokens.begin(); i != inSingleCharTokens.end(); ++i) {
		PACC_AssertM(mDelimiters[(unsigned)*i] == 0, "a delimiter cannot be both white space and single char token!");
		mDelimiters[(unsigned)*i] = eSingleChar;
	}
}

/*!
This method sets a new stream to be tokenized. It also flushes the internal read buffer.
 */
void Tokenizer::setStream(istream& inStream) 
{
	mStream = &inStream; 
	mLine = 1; 
	mBufCount = 0; 
	mTokens = stack<string>();
}
