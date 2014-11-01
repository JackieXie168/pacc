/*
 *  Portable Agile C++ Classes (PACC)
 *  Copyright (C) 2001-2005 by Marc Parizeau
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
 *  \file   PACC/Math/QRandSequencer.hpp
 *  \brief  Class definition for the scrambled Halton quasi-random sequence generator.
 *  \author Christian Gagne <cgagne@gmail.com>
 *  \author Olivier Teytaud <olivier.teytaud@lri.fr>
 *  $Revision: 1.3 $
 *  $Date: 2007/01/23 21:28:23 $
 */

#ifndef PACC_QRandSequencer_hpp
#define PACC_QRandSequencer_hpp

#include <vector>

#include "PACC/Util/Randomizer.hpp"
#include "PACC/Math/Vector.hpp"
#include "PACC/Math/Matrix.hpp"

namespace PACC {

/*!
 *  \class QRandSequencer PACC/Math/QRandSequencer.hpp "PACC/Math/QRandSequencer.hpp"
 *  \brief Scrambled Halton quasi-random sequence generator.
 *  \author Christian Gagne <cgagne@gmail.com>
 *  \author Olivier Teytaud <olivier.teytaud@lri.fr>
 *  \author Marc Parizeau <marc.parizeau@gel.ulaval.ca>
 *  \ingroup Math
 *
 *  Generate low-discrepancy sequences of numbers (quasirandom numbers) using the
 *  scrambled Halton algorithm.
 */
	class QRandSequencer {

	 public:
		explicit QRandSequencer(unsigned int inDimensionality=0, PACC::Randomizer& inRand=PACC::rand);

		//! Delete this sequence generator.
		~QRandSequencer(void) {}
		
		static void computeSqRootCovar(PACC::Matrix& outSqRootCovar, PACC::Matrix& inCovar);
		
		void getGaussianVector(PACC::Vector& outVector);
		void getGaussianVector(PACC::Vector& outVector, const PACC::Vector& inCenter, const PACC::Vector& inStDev);
		void getGaussianVector(PACC::Vector& outVector, const PACC::Vector& inCenter, const PACC::Matrix& inSqRootCovar);

		void getIntegerSequence(std::vector<long>& outSequence, long inMinValue, long inMaxValue);
		void getIntegerSequence(std::vector<long>& outSequence, const std::vector<long>& inMinValues, const std::vector<long>& inMaxValues);

		void getUniformVector(PACC::Vector& outVector, double inMinValue=0., double inMaxValue=1.);
		void getUniformVector(PACC::Vector& outVector, const PACC::Vector& inMinValues, const PACC::Vector& inMaxValues);

		std::string getState(void) const;
		void setState(const std::string& inState);

		//! \brief Return dimensionality of low-discrepancy sequences.
		inline unsigned int getDimensionality(void) const {return mDimensionality;}
		
		void reset(unsigned int inDimensionality, PACC::Randomizer& inRand=PACC::rand);
		
	 protected:
		std::vector<unsigned int>                mBases;          //!< Prime numbers used as bases.
		std::vector< std::vector<unsigned int> > mCounters;       //!< Counter in prime bases.
		std::vector< std::vector<unsigned int> > mPermutations;   //!< Number permutations in given bases.
		unsigned int mDimensionality; //!< Dimensionality of the numbers.
		unsigned int mCount;          //!< Count generated numbers.

		void generateSequence(std::vector<unsigned long>& outValues, std::vector<unsigned long>& outMaxValues);
	};
}

#endif // PACC_QRandSequencer_hpp
