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
 * \file PACC/Util/StringFunc.hpp
 * \brief Definition of string conversion methods.
 * \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.7 $
 * $Date: 2005/09/15 14:13:24 $
 */

/*! \namespace PACC::String
* \brief %String conversion functions.
*/

#ifndef PACC_StringFunc_hpp_
#define PACC_StringFunc_hpp_

#include <string>
#include <sstream>
#include <stdlib.h>

namespace PACC {
	
	using namespace std;
	
	namespace String {
		
		//! Convert string \c inString to integer value in base \c inBase (default is base 10).
		inline long int convertToInteger(const string& inString, int inBase=10) {
			return strtol(inString.c_str(), 0, inBase);
		}
		
		//! Convert string \c inString to float value (Warning: no error checking).
		inline double convertToFloat(const string& inString) {
			return strtod(inString.c_str(), 0);
		}
		
		//! Convert data type \c inData to string.
		template <typename Type>
		string convert(Type inData) {
			ostringstream lStream;
			lStream << inData;
			return lStream.str();
		}
		
	} // end of String namespace
	
} // end of PACC namespace

#endif // PACC_StringFunc_hpp_
