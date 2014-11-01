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
 * \file PACC/Util/NumberGenerator.hpp
 * \brief Class definition for the random number generator.
 * \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.9 $
 * $Date: 2004/06/02 04:57:21 $
 */

#ifndef PACC_NumberGenerator_hpp_
#define PACC_NumberGenerator_hpp_

#define IA 16807
#define IM 2147483647
#define AM (1.0/IM)
#define IQ 127773
#define IR 2836
#define NTAB 32
#define NDIV (1+(IM-1)/NTAB)
#define EPS 1.2e-7
#define RNMX (1.0-EPS)

namespace PACC {

   /*!
   \brief Random number generator
   \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
   \ingroup Util
   
   This class encapsulates the "rand1" number generator defined in the
   "Numerical Recipes in C" book. It can generate uniformly distributed 
   booleans, integers and floats, or gaussian distributed floats.
   */
   class NumberGenerator
   {
   public:
      NumberGenerator(void);
      NumberGenerator(unsigned long inSeed);
      ~NumberGenerator(void);
   
      //! Return a uniformly distributed random integer in range [\c 0, \c inValue-1].
      int operator()(unsigned long inValue) {return getUniformInteger(0, inValue-1);}
   
      //! Return the current seed of the generator.
      unsigned int getCurrentSeed(void) {return idum;}
      //! Return a uniformly distributed random boolean.
      bool getUniformBool(void) {return getUniformInteger(0,1);}
      int getUniformInteger(int inFirst, int inLast);
      double getUniformFloat(double inFirst=0., double inLast=1.0);
      double getGaussianFloat(double inMean=0, double inStdDev=1); 
                  
   private:
      long iy;
      long iv[NTAB];
      long idum;
   
      int iset;
      double gset;
   
   };

   extern NumberGenerator rand1; //!< Global random number generator

} // end of PACC namespace                    

#endif

