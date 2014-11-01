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
 * \file PACC/Util/NumberGenerator.cpp
 * \brief Class methods for the random number generator.
 * \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.10 $
 * $Date: 2005/04/19 18:19:16 $
 */

#include "Util/NumberGenerator.hpp"
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <iostream>
#include <fstream>

using namespace std;
using namespace PACC;

NumberGenerator PACC::rand1;

/*! \brief Initialize the generator using the seed in file "rand1.seed".

File "rand1.seed" must be in the current directory. If it doesn't exist, the seed is initialized using current time. Because the seed of file "rand1.seed" is only updated by the class destructor, it follows that any two generators initialized by this default constructor within the same scope will produce the exact same sequence of numbers.  
*/
NumberGenerator::NumberGenerator(void) 
{
   ifstream lStream("rand1.seed");
   if(lStream.good()) lStream >> idum;
   else idum = time(0);
   for(int j=NTAB+7; j >= 0; --j)
   {
      long k = idum/IQ;
      idum = IA*(idum-k*IQ)-IR*k;
      if(idum < 0) idum += IM;
      if(j < NTAB) iv[j] = idum;
   }
   iy = iv[0];
   iset = 0;
}

//! Initialize the generator the seed \c inSeed.
NumberGenerator::NumberGenerator(unsigned long inSeed) 
{
   idum = inSeed;
   for(int j=NTAB+7; j >= 0; --j)
   {
      long k = idum/IQ;
      idum = IA*(idum-k*IQ)-IR*k;
      if(idum < 0) idum += IM;
      if(j < NTAB) iv[j] = idum;
   }
   iy = iv[0];
   iset = 0;
}

/*! \brief Save the current seed in file "rand1.seed".

If the file doesn't exist, it is first created.
*/
NumberGenerator::~NumberGenerator(void)
{
	ofstream lStream("rand1.seed");
	if(!lStream.good()) cerr << "Unable to create file \"rand1.seed\"!" << endl;
	else lStream << idum;
}

//! Return a uniformly distributed random integer in range [\c inFirst,\c inLast].
int NumberGenerator::getUniformInteger(int inFirst, int inLast) 
{
   int lNumber = (int) (inFirst + (inLast - inFirst + 1) * getUniformFloat());
   if(lNumber > inLast) lNumber = inLast;
   return lNumber;
}

//! Return a uniformly distributed random floating point number in range [\c inFirst,\c inLast] (default is [0,1]).
double NumberGenerator::getUniformFloat(double inFirst, double inLast) 
{
   double lTmp, lNumber;

   long k = idum/IQ;
   idum = IA*(idum-k*IQ)-IR*k;
   if(idum < 0) idum += IM;
   int j = (int) iy/NDIV;
   iy = iv[j];
   iv[j] = idum;
   if((lTmp=AM*iy) > RNMX) lNumber = RNMX;
   else lNumber = lTmp;
   return inFirst + (inLast - inFirst) * lNumber;
}

//! Return a gaussian distributed random floating point number with mean \c inMean and standard deviation \c inStdDev. Default distribution is N(0,1).
double NumberGenerator::getGaussianFloat(double inMean, double inStdDev) 
{
   double fac, rsq, v1, v2;

   if(iset == 0) {
      do {
         v1 = 2.0*getUniformFloat() - 1.0;
         v2 = 2.0*getUniformFloat() - 1.0;
         rsq = v1*v1+v2*v2;
      } while(rsq >= 1.0 || rsq == 0);
      fac = sqrt(-2.0*log(rsq)/rsq);
      gset = v1*fac;
      iset = 1;
      return v2*fac*inStdDev + inMean;
   } else {
      iset = 0;
      return gset*inStdDev + inMean;
   }
}

