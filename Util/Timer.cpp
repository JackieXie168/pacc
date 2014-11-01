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
 * $Revision: 1.13 $
 * $Date: 2005/06/02 06:59:17 $
 */

#include "Util/Timer.hpp"
#include "Util/Assert.hpp"

#if defined(WIN32)
#include <windows.h>
#include <stdexcept>
namespace PACC {
	struct TimerStruct {
		LARGE_INTEGER mCounter;
		LARGE_INTEGER mFrequency;
	};
}
#else
#if defined(__GNUG__) && (defined(__i386__) || defined(__ppc__))
#include <sys/time.h>
#include <unistd.h>
namespace PACC {
	struct TimerStruct {
		unsigned long long mCount;
		double mPeriod;
	};
}
#else
#include <sys/time.h>
namespace PACC {
	struct TimerStruct {
		timeval mTime;
		double mPeriod;
	};
}
#endif
#endif

using namespace std;
using namespace PACC;

#if defined(__GNUG__)
namespace PACC {
	//! Return high resolution time-stamp count.
	unsigned long long getHRCount(void)
	{
		unsigned long long lCount = 0;
#if defined(__i386__)
		__asm__ volatile("rdtsc" : "=A" (lCount));
#else
#if defined(__ppc__)
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
		PACC_AssertM(false, "Timer:getHighResolutionCount() method not supported for this processor!");
#endif
#endif
		return lCount;
	}
}
#endif

#if defined(__GNUG__)
namespace PACC {
	//! Return counter period in seconds, using delay \c inDelay microseconds and by averaging over \c inRepeat iterations.
	double calibrateHRCount(unsigned int inDelay, unsigned int inRepeat)
	{
		double lPeriod = 0;
		for(unsigned int i = 0; i < inRepeat; ++ i) {
			timeval lStartTime, lTime;
			::gettimeofday(&lStartTime, 0);
			unsigned long long lStartCount = getHRCount();
			::usleep(inDelay);
			::gettimeofday(&lTime, 0);
			unsigned long long lCount = getHRCount() - lStartCount;
			lTime.tv_sec -= lStartTime.tv_sec;
			lTime.tv_usec -= lStartTime.tv_usec;
			lPeriod += (lTime.tv_sec + lTime.tv_usec*0.000001)/lCount;
		}
		lPeriod /= inRepeat;
		return lPeriod;
	}
}
#endif

//! Construct using current system date and time.
Timer::Timer(void) : mTimer(new TimerStruct)
{
   TimerStruct* lTimer = (TimerStruct*) mTimer;
#if defined(WIN32)
   PACC_AssertM(QueryPerformanceFrequency(&lTimer->mFrequency), "Timer::Timer() no performance counter on this processor!");
#else
#if defined(__GNUG__) && (defined(__i386__) || defined(__ppc__))
	// callibrate timer period; this will take about 0.5 sec
	lTimer->mPeriod = calibrateHRCount(50000, 10);
#else
	// use the microseconds of gettimeofday
	lTimer->mPeriod = 0.000001;
#endif
#endif
   reset();
}

//! Construct by copying timer \c inTimer.
Timer::Timer(const Timer& inTimer) : mTimer(new TimerStruct) 
{
   operator=(inTimer);
}

//! Delete opaque structure.
Timer::~Timer(void)
{
   if(mTimer) delete (TimerStruct*) mTimer;
}

//! Assign timer \c inTimer to this timer.
Timer& Timer::operator=(const Timer& inTimer)
{
   // do not self assign
   if(&inTimer != this) 
   {
      *((TimerStruct*)mTimer) = *((TimerStruct*)inTimer.mTimer);
   }
   return *this;
}

//! Return current value of timer (in seconds).
double Timer::getValue(void) const
{
   TimerStruct* lTimer = (TimerStruct*) mTimer;
#if defined(WIN32)
   LARGE_INTEGER lCurrent;
   QueryPerformanceCounter(&lCurrent);
   return (double)(lCurrent.QuadPart-lTimer->mCounter.QuadPart)/lTimer->mFrequency.QuadPart;
#else
#if defined(__GNUG__) && (defined(__i386__) || defined(__ppc__))
	return (getHRCount()-lTimer->mCount)*lTimer->mPeriod;
#else
   timeval lCurrent;
   ::gettimeofday(&lCurrent, 0);
   return (lCurrent.tv_sec-lTimer->mTime.tv_sec) + (double)(lCurrent.tv_usec-lTimer->mTime.tv_usec)/1000000;
#endif
#endif
}

//! reset timer to zero.
void Timer::reset(void)
{
   TimerStruct* lTimer = (TimerStruct*) mTimer;
#if defined(WIN32)
   QueryPerformanceCounter(&lTimer->mCounter);
#else
#if defined(__GNUG__) && (defined(__i386__) || defined(__ppc__))
	lTimer->mCount = getHRCount();
#else
   ::gettimeofday(&lTimer->mTime, 0);
#endif
#endif
}
