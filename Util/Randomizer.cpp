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
 * \file PACC/Util/Randomizer.cpp
 * \brief Class methods for the random number generator.
 * \author Marc Parizeau and Christian Gagn&eacute;, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.3 $
 * $Date: 2005/06/02 06:59:17 $
 */

#include "Util/Randomizer.hpp"
#include <sstream>

using namespace std;
using namespace PACC;

Randomizer PACC::rand;

/*! Return state of generator.
*/
string Randomizer::getState(void) const
{
	ostringstream lStream;
	for(unsigned i = 0; i < N; ++i) lStream << state[i] << ",";
	lStream << left;
	return lStream.str();
}

/*! Set state of generator.
*/
void Randomizer::setState(const string& inState)
{
	istringstream lStream(inState);
	for(unsigned int i = 0; i < N; ++i) {
		lStream >> state[i];
		lStream.get();
	}
	lStream >> left;
	pNext = &state[N-left];
}
