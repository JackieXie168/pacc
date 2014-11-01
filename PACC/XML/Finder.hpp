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
* \file PACC/XML/Finder.hpp
 * \brief Class definition for the %XML tag finder.
 * \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.19 $
 * $Date: 2007/01/23 21:28:09 $
 */

#ifndef PACC_XML_Finder_hpp_
#define PACC_XML_Finder_hpp_

#include "PACC/XML/Iterator.hpp"
#include <stack>
#include <queue>

namespace PACC { 
	
	using namespace std;
	
	namespace XML {
		
		//! \brief %Finder for data tags.
		//! \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
		//! \ingroup XML
		//!
		//! This class is for finding markup tags in a document sub-tree. The search string is inspired from the xpath standard. The following syntax subset is currently supported:
		//! - Path ::= AbsolutePath | RelativePath	
		//! - AbsolutePath ::= '/' RelativePath | '//' RelativePath
		//! - RelativePath ::= Step | RelativePath '/' Step | RelativePath '//' Step
		//! - Step ::= '..' | '*' | TagName
		//! .
		//! Here is a list of simple examples:
		//! - "para" finds the para element children of the root node
		//! - "*" finds all element children of the root node
		//! - "/doc/chapter/section" finds all sections of all chapters for root node doc
		//! - "chapter//para" finds para elements descendants of the chapter element children of the root node
		//! - "//para" finds all the para descendants of the root node
		//! - "//para/*" finds all element children of all para descendants of the root node
		//! - "//para/.." finds the parents of all para descendants of the root node
		//! .
		//! There can be no spaces in a search path.
		class Finder : private Iterator {
		 public:
			//! Construct a finder for the sub-tree rooted on \c inRoot.
			Finder(const Iterator& inRoot) : Iterator(inRoot) {}
			
			//! Return the first markup instance that matches path \c inPath.
			Iterator find(const string& inPath);
			//! Return the next search result.
			Iterator findNext(void);
			
			protected:
			vector<string> mSteps; //!< Array of steps in search path.
			queue<Iterator> mMatches; //!< Search results.
			
			//! Internal search function.
			void search(Iterator inPos, unsigned int inStep);
		};
		
		//! \brief Const finder for data tags.
		//! \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
		//! \ingroup XML
		//!	
		//! This class is for finding markup tags in a document sub-tree using const iterators (see class Finder for more details).
		class ConstFinder : private ConstIterator {
		 public:
			//! Construct a const finder for the sub-tree rooted on \c inRoot.
			ConstFinder(const ConstIterator& inRoot) : ConstIterator(inRoot) {}
			
			//! Return the first markup instance that matches path \c inPath.
			ConstIterator find(const string& inPath);
			//! Return the next search result.
			ConstIterator findNext(void);
			
			protected:
			vector<string> mSteps; //!< Array of steps in search path.
			queue<ConstIterator> mMatches; //!< Search results.
			
			//! Internal search function.
			void search(ConstIterator inPos, unsigned int inStep);
		};
		
	} // end of namespace XML
	
} // end of namespace PACC

#endif // PACC_XML_Finder_hpp_
