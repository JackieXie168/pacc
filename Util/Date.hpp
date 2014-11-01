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
 * \file PACC/Util/Date.hpp
 * \brief Class definition for the portable time and date.
 * \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.11 $
 * $Date: 2006/01/15 22:50:49 $
 */

#ifndef PACC_Date_hpp
#define PACC_Date_hpp

#include <string>
#include <ctime>

namespace PACC {
	
	using namespace std;
	
	/*! \brief Portable time and date class.
	\author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
	\ingroup Util
	*/
	class Date {
	 public:
		//! Construct using current system date and time.
		Date(void) : mTime(::time(0)) {}
		//! Construct from \c inSeconds seconds since January 1st, 1970.
		explicit Date(unsigned int inSeconds) : mTime(inSeconds) {}
		// Construct with specified date.
		Date(unsigned int inYear, unsigned int inMonth, unsigned int inDay);
		
		//! Return number of seconds between this date and date \c inDate.
		unsigned int operator-(const Date& inDate) const {return mTime-inDate.mTime;}
		//! Return this date less \c inSeconds seconds.
		Date operator-(unsigned int inSeconds) const {Date lDate(mTime); return lDate -= inSeconds;}
		//! Remove \c inSeconds seconds from this date.
		Date& operator-=(unsigned int inSeconds) {mTime -= inSeconds; return *this;}
		//! Return this date plus \c inSeconds seconds.
		Date operator+(unsigned int inSeconds) const {Date lDate(mTime); return lDate += inSeconds;}
		//! Add \c inSeconds seconds to this date.
		Date& operator+=(unsigned int inSeconds) {mTime += inSeconds; return *this;}
		
		// Return formatted date and time using the std::strftime string format \c inFormat.
		string get(const string& inFormat = "%a %b %d %T %Z %Y") const;
		//! Return day of month for this date (1-31).
		unsigned int getDayOfMonth(void) const {return ::localtime(&mTime)->tm_mday;}
		//! Return day of week for this date (sunday=0-6).
		unsigned int getDayOfWeek(void) const {return ::localtime(&mTime)->tm_wday;}
		//! Return day of year for this date (1-365).
		unsigned int getDayOfYear(void) const {return ::localtime(&mTime)->tm_yday+1;}
		//! Return hour of day for this date (0-23).
		unsigned int getHourOfDay(void) const {return ::localtime(&mTime)->tm_hour;}
		//! Return minutes of hour for this date (0-59).
		unsigned int getMinutesOfHour(void) const {return ::localtime(&mTime)->tm_min;}
		//! Return seconds of minutes for this date (0-59).
		double getSecondsOfMinute(void) const {return ::localtime(&mTime)->tm_sec;}
		//! Return month of year for this date (1-12).
		unsigned int getMonthOfYear(void) const {return ::localtime(&mTime)->tm_mon+1;}
		//! Return this date expressed in seconds since January 1st, 1970.
		unsigned int getTimeInSeconds(void) const {return mTime;}
		//! Return year of this date.
		unsigned int getYear(void) const {return ::localtime(&mTime)->tm_year+1900;}
		
		// Set date to year \c inyear, month \c inMonth, and day \c inDay (time is unchanged).
		void setDate(unsigned int inYear, unsigned int inMonth, unsigned int inDay);
		// Set time to \c inHour hour, \c inMinutes minutes, and \c inSeconds seconds (date is unchanged).
		void setTime(unsigned int inHour, unsigned int inMinutes, unsigned int inSeconds);

	 protected:
		time_t mTime; //!< Number of seconds since Jan 1st, 1970.
		
	};
	
} // end of PACC namespace

#endif // PACC_Date_hpp
