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
 * \file PACC/XML/Finder.cpp
 * \brief Class methods for the %XML tag finder.
 * \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.21 $
 * $Date: 2007/06/20 04:17:10 $
 */

#include "PACC/XML/Finder.hpp"
#include <sstream>
#include <stdexcept>

using namespace std;
using namespace PACC;

/*! 
Only regular markup is considered in this search. Here is an usage example for finding the first instance of tag "MyTag" in document \c lDocument:
\code
Document lDocument;
...
Finder lFinder(lDocument.getFirstDataTag());
// find the first instance
Iterator lTag = lFinder.find("//Mytag");
if(lTag)
{
	// process this instance of markup "Mytag"
	...
}
\endcode
After processing a found tag, the search may be continued using method Finder::findNext. 
*/
XML::Iterator XML::Finder::find(const string& inPath)
{
	// reset the path steps 
	mSteps.clear();
	// parse the search path
	istringstream lStream(inPath);
	Tokenizer lTokenizer(lStream);
	lTokenizer.setDelimiters("", "/");
	string lToken;
	while(lTokenizer.getNextToken(lToken)) {
		// next token should be '/' 
		if(lToken[0] != '/') {
			// unless path is relative; then make it absolute!
			if(mSteps.empty()) {
				mSteps.push_back("*");
				mSteps.push_back(lToken);
				continue;
			} else throw runtime_error(string("Finder::find() invalid search path: ")+inPath);
		}
		int lNextChar = lTokenizer.peekNextChar();
		if(lNextChar == -1) break;
		// process '//' special case
		if(lNextChar == '/') {
			// but ignore '///'
			if(mSteps.empty() || mSteps.back() != "") mSteps.push_back("");
		} else if(lTokenizer.getNextToken(lToken)) mSteps.push_back(lToken);
		else break;
	}
	// search for the path
	search(mNode, 0);
	// pop first match from queue
	Iterator lPos;
	if(!mMatches.empty()) {
		lPos = mMatches.front();
		mMatches.pop();
	}
	// return first match or null iterator
	return lPos;
}

/*! 
The search proceeds from the point where the last markup was found. Here is an example where we search for all instances of markup "Mytag" in document \c lDocument:
\code
Document lDocument;
...
Finder lFinder(lDocument.getFirstDataTag());
// find all instances of tag
for(Iterator lTag = lFinder.find("//Mytag"); lTag; lTag = lFinder.findNext())
{
	// process each instance
	...
}
\endcode
*/
XML::Iterator XML::Finder::findNext(void)
{
	Iterator lPos;
	// pop next match from the queue
	if(!mMatches.empty()) {
		lPos = mMatches.front();
		mMatches.pop();
	}
	// return next match or null iterator
	return lPos;
}

//! Search for step \c inStep starting at position \c inPos.
void XML::Finder::search(XML::Iterator inPos, unsigned int inStep) {
	// stop recursion?
	if(!inPos || inStep >= mSteps.size()) return;
	// process current step
	if(mSteps[inStep] == "..") {
		// move up to parent
		if(inPos != mNode) {
			if(inStep+1 < mSteps.size()) {
				if(mSteps[inStep+1] == "..") search(inPos->getParent(), inStep+1);
				else {
					for(Iterator lPos = inPos->getParent()->getFirstChild(); lPos; ++lPos)
						search(lPos, inStep+1);
				}
			} else mMatches.push(inPos->getParent());
		} 
	} else if(mSteps[inStep] == "") {
		// walk tree in level order
		queue<Iterator> lSearchQueue;
		lSearchQueue.push(inPos);
		while(!lSearchQueue.empty()) {
			// get next node
			Iterator lPos = lSearchQueue.front();
			lSearchQueue.pop();
			// push all its children onto the search queue
			for(Iterator lChild = lPos->getFirstChild(); lChild; ++lChild) lSearchQueue.push(lChild);
			// if not last step, search recursively
			if(inStep+1 < mSteps.size()) search(lPos, inStep+1);
			else mMatches.push(lPos);
		}
	} else {
		if(mSteps[inStep] == "*" || inPos->getValue() == mSteps[inStep]) {
			// found a match
			if(inStep+1 < mSteps.size()) {
				if(mSteps[inStep+1] == "..") search(inPos, inStep+1);
				else {
					for(Iterator lPos = inPos->getFirstChild(); lPos; ++lPos)
						search(lPos, inStep+1);
				}
			} else mMatches.push(inPos);
		}
	}
}

/*! 
Only regular markup is considered in this search. Here is an usage example for finding the first instance of tag "MyTag" in document \c lDocument:
\code
Document lDocument;
...
ConstFinder lFinder(lDocument.getFirstDataTag());
// find the first instance
ConstIterator lTag = lFinder.find("//Mytag");
if(lTag)
{
	// process this instance of markup "Mytag"
	...
}
\endcode
After processing a found tag, the search may be continued using method Finder::findNext. 
*/
XML::ConstIterator XML::ConstFinder::find(const string& inPath)
{
	// reset the path steps 
	mSteps.clear();
	// parse the search path
	istringstream lStream(inPath);
	Tokenizer lTokenizer(lStream);
	lTokenizer.setDelimiters("", "/");
	string lToken;
	while(lTokenizer.getNextToken(lToken)) {
		// next token should be '/' 
		if(lToken[0] != '/') {
			// unless path is relative; then make it absolute!
			if(mSteps.empty()) {
				mSteps.push_back("*");
				mSteps.push_back(lToken);
				continue;
			} else throw runtime_error(string("ConstFinder::find() invalid search path: ")+inPath);
		}
		int lNextChar = lTokenizer.peekNextChar();
		if(lNextChar == -1) break;
		// process '//' special case
		if(lNextChar == '/') {
			// but ignore '///'
			if(mSteps.empty() || mSteps.back() != "") mSteps.push_back("");
		} else if(lTokenizer.getNextToken(lToken)) mSteps.push_back(lToken);
		else break;
	}
	// search for the path
	search(mNode, 0);
	// pop first match from queue
	ConstIterator lPos;
	if(!mMatches.empty()) {
		lPos = mMatches.front();
		mMatches.pop();
	}
	// return first match or null iterator
	return lPos;
}

/*! 
The search proceeds from the point where the last markup was found. Here is an example where we search for all instances of markup "Mytag" in document \c lDocument:
\code
Document lDocument;
...
ConstFinder lFinder(lDocument.getFirstDataTag());
// find all instances of tag
for(ConstIterator lTag = lDocument.find("//Mytag"); lTag; lTag = lDocument.findNext())
{
	// process each instance
	...
}
\endcode
*/
XML::ConstIterator XML::ConstFinder::findNext(void)
{
	ConstIterator lPos;
	// pop next match from queue
	if(!mMatches.empty()) {
		lPos = mMatches.front();
		mMatches.pop();
	}
	// return next match or null iterator
	return lPos;
}

//! Search for step \c inStep starting at position \c inPos.
void XML::ConstFinder::search(XML::ConstIterator inPos, unsigned int inStep) {
	// stop recursion?
	if(!inPos || inStep >= mSteps.size()) return;
	// process current step
	if(mSteps[inStep] == "..") {
		// move up to parent
		if(inPos != mNode) {
			if(inStep+1 < mSteps.size()) {
				if(mSteps[inStep+1] == "..") search(inPos->getParent(), inStep+1);
				else {
					for(ConstIterator lPos = inPos->getParent()->getFirstChild(); lPos; ++lPos)
						search(lPos, inStep+1);
				}
			} else mMatches.push(inPos->getParent());
		} 
	} else if(mSteps[inStep] == "") {
		// walk tree in level order
		queue<ConstIterator> lSearchQueue;
		lSearchQueue.push(inPos);
		while(!lSearchQueue.empty()) {
			// get next node
			ConstIterator lPos = lSearchQueue.front();
			lSearchQueue.pop();
			// push all its children onto the search queue
			for(ConstIterator lChild = lPos->getFirstChild(); lChild; ++lChild) lSearchQueue.push(lChild);
			// if not last step, search recursively
			if(inStep+1 < mSteps.size()) search(lPos, inStep+1);
			else mMatches.push(lPos);
		}
	} else {
		if(mSteps[inStep] == "*" || inPos->getValue() == mSteps[inStep]) {
			// found a match
			if(inStep+1 < mSteps.size()) {
				if(mSteps[inStep+1] == "..") search(inPos, inStep+1);
				else {
					for(ConstIterator lPos = inPos->getFirstChild(); lPos; ++lPos)
						search(lPos, inStep+1);
				}
			} else mMatches.push(inPos);
		}
	}
}
