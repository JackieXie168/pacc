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
 * \file PACC/SVG/Document.hpp
 * \brief Class definition for the SVG drawing document.
 * \author Marc Parizeau and Michel Fortin, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 */

#ifndef PACC_SVG_Document_hpp_
#define PACC_SVG_Document_hpp_

#include "PACC/SVG/Frame.hpp"

namespace PACC {
	
	namespace SVG {
		
		using namespace std;
		
		/*!\brief Specialized frame to encapsulate a valid svg document.
		* \ingroup SVGdoc
		*/
		class Document : public Frame {
		 public:
			Document(const string& inTitle, const Size& inSize, const Style& inStyle=Style());
			
			void setTitle(const string& inTitle);
			
			void read(const XML::ConstIterator& inNode);			
			void write(ostream& outStream) const;
			
		 private:
			string mTitle; //<! Title of this document.
			
			//! Disabled methods.
			void clearTransform(void);
			void setTransform(const Transform& inTransform);
		};
		
	} // end of namespace SVG
	
	ostream& operator<<(ostream& outStream, const SVG::Document& inDocument);
	istream& operator>>(istream& inStream, SVG::Document& outDocument);
	
} // end of namespace PACC

#endif // PACC_SVG_Document_hpp_
