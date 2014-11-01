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
 * \file PACC/Util/Tokenizer.hpp
 * \brief Class definition for the input stream tokenizer.
 * \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.16 $
 * $Date: 2005/09/17 03:50:19 $
 */

#ifndef PACC_Tokenizer_hpp_
#define PACC_Tokenizer_hpp_

#include <iostream>
#include <string>
#include <stack>

namespace PACC {
	
	using namespace std;
	
	/*! \brief Input stream tokenizer
	\author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
	\ingroup Util
	\ingroup XML
	
	This class is for tokenizing an input stream. Tokens are either strings of characters seperated by white space, or special single-characters. By default, there are no special single-character tokens, and white space characters are spaces, tabs, cariage returns, and line-feeds. But these can be changed using method Tokenizer::setDelimiters. For instance, if "a" and "d" are single character tokens, then string "abcd" will be tokenized in "a"+"bc" + "d".
	*/
	class Tokenizer {
		public:
		//! Construct a default tokenizer with no input stream.
		Tokenizer(void) : mLine(1), mStream(0), mBufPtr(0), mBufCount(0) {setDelimiters(" \t\n\r", "");}
		//! Construct a tokenizer with input stream \c inStream.
		Tokenizer(istream& inStream) : mLine(1), mStream(&inStream), mBufPtr(0), mBufCount(0)  {setDelimiters(" \t\n\r", "");}
		
		//! Parse next token of default input stream.
		string getNextToken(void);
		//! Parse next token of default input stream.
		bool getNextToken(string& outToken);
		//! Return the next character from the default input stream.
		int peekNextChar(void);
		//! Put back token \c inToken.
		void putbackToken(const string& inToken);
		
		//! Return the current line number.
		unsigned int getLineNumber(void) {return mLine;}
		//! Return the name of the current input stream.
		const string& getStreamName(void) const {return mName;}
		//! Return the current single character delimiters.
		string getSingleCharTokens(void) const;
		//! Return the current white space delimiters.
		string getWhiteSpace(void) const;
		//! Set the white space delimiters to the characters of \c inWhiteSpace, and the single character delimiters to those of \c inToken.
		void setDelimiters(const string& inWhiteSpace, const string& inTokens);
		//! Set the current line number to \c inLine.
		void setLineNumber(unsigned int inLine) {mLine = inLine;}
		//! Set the input stream to \c inStream.
		void setStream(istream& inStream) {mStream = &inStream; mLine = 1; mBufCount = 0; mTokens = stack<string>();}
		//! Set the name of the current input stream to \c inName.
		void setStreamName(const string& inName) {mName = inName;}
		
		protected:
		unsigned int mLine; //!< Current line number.
		string mName; //!< Name of default input stream.
		istream* mStream; //!< Default input stream.
		char mDelimiters[256]; //!< Single character tokens.
		char mBuffer[1024]; //!< Input read buffer
		char* mBufPtr; //!< Pointer to next character in the read buffer
		unsigned int mBufCount; //!< Number of characters in the read buffer
		stack<string> mTokens; //!< Stack of putback tokens.
		
		//! Fill the input read buffer
		inline unsigned int fillBuffer(void) {
			mBufCount = mStream->readsome(mBufPtr=mBuffer, sizeof(mBuffer));
			if(mBufCount == 0) {
				// in case of console input, wait for the next character
				mStream->read(mBuffer, 1);
				mBufCount = mStream->gcount();
			}
			return mBufCount;
		}
		
	};
	
} // end of PACC namespace

#endif // PACC_Tokenizer_hpp_
