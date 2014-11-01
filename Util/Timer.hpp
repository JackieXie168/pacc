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
 * $Revision: 1.14 $
 * $Date: 2005/10/05 12:33:01 $
 */

#ifndef PACC_Timer_hpp
#define PACC_Timer_hpp

namespace PACC {

	/*! \brief Portable timer class.
	\author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
	\ingroup Util
	
	This class implements a simple stopwatch timer that is always running. Method Timer::getValue can be used to return the current timer value in seconds. Method Timer::reset resets this value to 0. Under Windows, this class uses the QueryPerformanceCounter method to retrieve a high-resolution hardware time-stamp. When using the gcc compiler, this class also retrieves a hardware time-stamp counter on the following platforms:
	- Pentium family (i386)
	- PowerPC family (ppc)
	.
	For all other cases, the class uses the standard gettimeofday method to retrieve a somewhat high resolution time stamp (max resolution is micro-seconds).
	
	The current high resolution time stamp can be retrieved using method Timer::getCount. The time period associated with a single count increment is platform dependent. Its value can be fetch with method Timer::getCountPeriod.
	
	\attention Hardware time-stamp counters are dependent of the CPU clock frequency. On platforms where this frequency is variable (e.g. laptops), the time values in seconds may become false if the clock frequency is changed by the OS. When using a hardware counter, the class constructor conducts a calibration procedure using method gettimeofday. This procedure (if applicable) lasts about 0.1 sec with default parameters (see Timer::calibrateCountPeriod).   
	*/
	class Timer {

		public:

		//! Construct using current system date and time.
		Timer(void) {calibrateCountPeriod(); reset();}

		//! Calibrate the count period.
		void calibrateCountPeriod(unsigned int inDelay=10000, unsigned int inTimes=10);

		//! Return current high resolution count.
		unsigned long long getCount(void) const;

		//! Return time period of count (in seconds).
		double getCountPeriod(void) const {return mPeriod;}

		//! Return current value of timer (in seconds).
		double getValue(void) const {return (getCount()-mCount)*mPeriod;}

		//! reset timer to zero.
		void reset(void) {mCount = getCount();}

		protected:
			
		unsigned long long mCount; //!< Count at last reset.
		double mPeriod; //!< Time period of a single count.
	};

} // end of PACC namespace

#endif // PACC_Timer_hpp
