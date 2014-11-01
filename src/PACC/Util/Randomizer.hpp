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
 * \file PACC/Util/Randomizer.hpp
 * \brief Class definition for the random number generator.
 * \author Marc Parizeau and Christian Gagne, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.11 $
 * $Date: 2008/04/17 21:13:57 $
 */

#ifndef PACC_Randomizer_hpp_
#define PACC_Randomizer_hpp_

#include "PACC/Util/MTRand.hpp"
#include <vector>
#include <string>

namespace PACC {
	
	using namespace std;
	
	/*!
	\brief Random number generator
	 \author Marc Parizeau and Christian Gagn&eacute;, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
	 \ingroup Util
	 
	 This class encapsulates the "Mersenne Twister" random number generator defined in reference:
	 - M. Matsumoto and T. Nishimura, "Mersenne Twister: A 623-Dimensionally Equidistributed Uniform Pseudo-Random Number Generator", ACM Transactions on Modeling and Computer Simulation, Vol. 8, No. 1, January 1998, pp 3-30.
	 
	 Its implementation is based on the \c %MTRand class by Richard J. Wagner <http://www-personal.engin.umich.edu/~wagnerr/MersenneTwister.html>
	 
	 It can generate uniformly distributed booleans, integers and floats, or gaussian distributed floats.
	 */
	class Randomizer : protected MTRand {
	 public:
		/*! \brief auto-initialize with /dev/urandom or time() and clock()
		
		Do NOT use for CRYPTOGRAPHY without securely hashing several returned
		values together, otherwise the generator state can be learned after
		reading 624 consecutive values.
		*/
		Randomizer(void) {}
		//! Initialize the generator with seed \c inSeed.
		Randomizer(unsigned long int inSeed) : MTRand(inSeed) {}
		//! Initialize the generator with an array of seeds.
		Randomizer(const vector<unsigned long int>& inSeeds) : MTRand(&inSeeds[0], inSeeds.size()) {}
		//! Initialize the generator with state \c inState.
		Randomizer(const string& inState) {setState(inState);}
		
		//! Return a uniformly distributed random integer in range [0,\c inValue[.
		unsigned long int operator()(unsigned long inValue) {return randInt(inValue-1);}
		
		//! Return a uniformly distributed random boolean.
		bool getBoolean(void) {return randInt(1);}
		//! Return a uniformly distributed random integer in range [0,2^32[.
		unsigned long int getInteger(void) {return randInt();}
		//! Return a uniformly distributed random integer in range [0,\c inValue].
		unsigned long int getInteger(unsigned long int inValue) {return randInt(inValue);}
		//! Return a uniformly distributed random integer in range [\c inLow,\c inHigh].
		long int getInteger(long int inLow, long int inHigh) {return randInt(inHigh-inLow)+inLow;}
		//! Return a uniformly distributed random floating point number in range [0,1].
		double getFloat(void) {return rand();}
		//! Return a uniformly distributed random floating point number in range [0,\c inValue].
		double getFloat(const double& inValue) {return rand(inValue);}
		//! Return a uniformly distributed random floating point number in range [\c inLow,\c inHigh].
		double getFloat(const double& inLow, const double& inHigh) {return inLow+rand()*(inHigh-inLow);}
		//! Return a 53 bits uniformly distributed random floating point number in range [\c inLow,\c inHigh[. Default is [0,1[.
		double getFloat53(const double& inLow=0, const double& inHigh=1) {return inLow+rand53()*(inHigh-inLow);}
		//! Return a gaussian distributed random float with mean \c inMean and standard deviation \c inStdDev. Default is N(0,1).
		double getGaussian(const double& inMean=0, const double& inStdDev=1) {return randNorm(inMean, inStdDev);}
		
		//! Return state of generator.
		string getState(void) const;
		//! Set state of generator.
		void setState(const string& inState);
		
	};

	extern Randomizer rand; //!< Global random number generator

} // end of PACC namespace                    

#endif

