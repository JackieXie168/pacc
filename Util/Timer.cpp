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
 * \file PACC/Util/Timer.cpp
 * \brief Class methods for the portable timer.
 * \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.19 $
 * $Date: 2005/10/05 12:33:01 $
 */

#include "Util/Timer.hpp"
#include "Util/Assert.hpp"

//#undef __ppc__

#if defined(WIN32)
#include <windows.h>
#include <stdexcept>
#else
#include <sys/time.h>
#include <unistd.h>
#endif

using namespace std;
using namespace PACC;

/*! 
Under Windows, this method calls the QueryPerformanceFrequency function to determine the count period. When using the gcc compiler on an i386 family processor (Pentium or AMD), or on a PowerPC family processor, this method calibrates the hardware time-stamp using the standard gettimeofday function. This calibration is conducted by measuring a delay of approximatly \c inDelay micro-seconds, and the count period is averaged over \c inTimes runs. In all other cases, the count period is fixed at 1 micro-second (the maximum resolution of gettimeofday).

This method is called automatically by the class constructor. Under normal circumstances, the user should not concern himself with calibration. 
 */
void Timer::calibrateCountPeriod(unsigned int inDelay, unsigned int inTimes)
{
#if defined(WIN32)
	// use the windows counter
	LARGE_INTEGER lFrequency;
	PACC_AssertM(QueryPerformanceFrequency(&lFrequency), "Timer::Timer() no performance counter on this processor!");
	mPeriod = 1. / lFrequency.QuadPart;
#else
#if defined(__GNUG__) && (defined(__i386__) || defined(__ppc__))
	mPeriod = 0;
	// calibrate by matching the time-stamps with the micro-seconds of gettimeofday
	for(unsigned int i = 0; i < inTimes+1; ++ i) {
		timeval lStartTime, lTime;
		::gettimeofday(&lStartTime, 0);
		unsigned long long lStartCount = getCount();
		::usleep(inDelay);
		::gettimeofday(&lTime, 0);
		unsigned long long lCount = getCount() - lStartCount;
		lTime.tv_sec -= lStartTime.tv_sec;
		lTime.tv_usec -= lStartTime.tv_usec;
		// dismiss the first run of the loop
		if(i != 0) mPeriod += (lTime.tv_sec + lTime.tv_usec*0.000001)/lCount;
	}
	mPeriod /= inTimes;
#else
	// use the microseconds of gettimeofday
	mPeriod = 0.000001;
#endif
#endif
}

/*! 
This method returns the highest resolution count available on this platform. Under Windows, it returns the hardware performance counter value as provided by its QueryPerformanceCounter method. When using the gcc compiler on an i386 family processor (Pentium or AMD), or on a PowerPC family processor, this method also returns the hardware performance counter value using in-lined assembly code. In all other cases, it returns a count based on the standard gettimeofday function (in micro-seconds).
 */
unsigned long long Timer::getCount(void) const
{
	unsigned long long lCount = 0;
#if defined(WIN32)
	LARGE_INTEGER lCurrent;
	QueryPerformanceCounter(&lCurrent);
	lCount = lCurrent.QuadPart;
#else
#if defined(__GNUG__) && defined(__i386__)
	__asm__ volatile("rdtsc" : "=A" (lCount));
#else
#if defined(__GNUG__) && defined(__ppc__)
	register unsigned int lLow;
	register unsigned int lHigh1;
	register unsigned int lHigh2;
	do {
		// make sure that high bits have not changed
		__asm__ volatile ( "mftbu %0" : "=r" (lHigh1) );
		__asm__ volatile ( "mftb  %0" : "=r" (lLow) );
		__asm__ volatile ( "mftbu %0" : "=r" (lHigh2) );
	} while(lHigh1 != lHigh2);
	// transfer to lCount
	unsigned int *lPtr = (unsigned int*) &lCount;
	*lPtr++ = lHigh1; *lPtr = lLow;
#else
	timeval lCurrent;
	::gettimeofday(&lCurrent, 0);
	lCount = (unsigned long long)lCurrent.tv_sec*1000000 + lCurrent.tv_usec;
#endif
#endif
#endif
	return lCount;
}
