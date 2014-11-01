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
 * \file PACC/Util/RandomPermutation.hpp
 * \brief Class definition for the random permutation.
 * \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.15 $
 * $Date: 2007/01/23 21:28:09 $
 */

#ifndef PACC_RandomPermutation_hpp_
#define PACC_RandomPermutation_hpp_

#include "PACC/Util/Randomizer.hpp"
#include <algorithm>
#include <vector>

namespace PACC {
	
	using namespace std;
	
	/*!\brief Random permutation generator.
	\author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
	\ingroup Util
	
	A random permutation of size X is a vector that contains integer 0 to X-1 randomly permutated.
	*/
	class RandomPermutation : public vector<unsigned int> {
	 public:
		//! Initialize permutation of size \c inSize, without any shuffling.
		RandomPermutation(unsigned int inSize=0) : vector<unsigned int>(inSize) {
			for(unsigned int i=0; i < inSize; ++i) (*this)[i] = i;
		}
		
		//! Shuffle permutation randomly using number generator \c inRand.
		RandomPermutation &permutate(Randomizer &inRand=PACC::rand) {
			random_shuffle(begin(), end(), inRand);
			return *this;
		}
		
	};
	
} // end of namespace PACC

#endif
