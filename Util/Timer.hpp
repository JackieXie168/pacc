/*
 *  Portable Agile C++ Classes (PACC)
 *  Copyright (C) 2004 by Marc Parizeau
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
 * \file PACC/Util/Timer.hpp
 * \brief Class definition for the portable timer.
 * \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.9 $
 * $Date: 2005/04/23 18:27:02 $
 */

#ifndef PACC_Timer_hpp
#define PACC_Timer_hpp

namespace PACC {

   /*! \brief Portable timer class.
       \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
       \ingroup Util
	
	This class implements a simple stopwatch timer that is always running. Method Timer::getValue can be used to return the current timer value in seconds. Method Timer::reset resets this value to 0. Under the Windows OS, this class uses the QueryPerformanceCounter method to retrieves a high-resolution hardware time-stamp. When using the gcc compiler, this class also retrieves a hardware time-stamp counter on the following platforms:
	- Pentium family (i386)
	- PowerPC family (ppc)
	.
	for all other cases, the class uses the standard gettimeofday method to retrieve a somewhat high resolution time stamp.
	
	\attention: hardware time-stamp counters are dependent of the CPU clock frequency. On platforms where this frequency is variable (e.g. laptops), the time values in seconds may become false if the clock frequency is changed by the OS. When using a hardware counter, the class constructor conducts a calibration procedure using method gettimeofday. This procedure (if applicable) lasts about 0.5 sec.   
    */
   class Timer {

public:
		Timer(void);
      Timer(const Timer& inTimer);
      ~Timer(void);
      
      Timer& operator=(const Timer& inTimer);
      
      double getValue(void) const;
      void reset(void);
      
protected:
      void* mTimer; //!< Opaque structure of timer
		      
	};
   
} // end of PACC namespace

#endif // PACC_Timer_hpp
