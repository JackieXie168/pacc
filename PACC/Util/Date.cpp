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
 * \file PACC/Util/Date.cpp
 * \brief Class methods for the portable time and date.
 * \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.13 $
 * $Date: 2007/02/24 22:11:38 $
 */

#include "PACC/Util/Date.hpp"

using namespace std;
using namespace PACC;

//! Construct with specified date \c inYear, \c inMonth, and \c inDay.
Date::Date(unsigned int inYear, unsigned int inMonth, unsigned int inDay) : mTime(0)
{
	setDate(inYear, inMonth, inDay);
	setTime(0, 0, 0);
}

//! Return formatted date and time using the std::strftime string format \c inFormat.
std::string Date::get(const string& inFormat) const 
{
	tm* lTime = ::localtime(&mTime);
	int lBufSize = 128;
	char* lBuffer = new char[lBufSize];
	while(::strftime(lBuffer, lBufSize, inFormat.c_str(), lTime) == 0) {
		// buffer needs to be enlarged
		delete lBuffer;
		lBuffer = new char[lBufSize*=2];
	}
	string lDate(lBuffer);
	delete lBuffer;
	return lDate;
}

//! Set date to year \c inyear, month \c inMonth, and day \c inDay (time is unchanged).
void Date::setDate(unsigned int inYear, unsigned int inMonth, unsigned int inDay)
{
	struct tm* lTime = localtime(&mTime);
	lTime->tm_year = inYear-1900;
	lTime->tm_mon = inMonth-1;
	lTime->tm_mday = inDay;
	mTime = ::mktime(lTime);
}

//! Set time to \c inHour hour, \c inMinutes minutes, and \c inSeconds seconds (date is unchanged).
void Date::setTime(unsigned int inHour, unsigned int inMinutes, unsigned int inSeconds)
{
	struct tm* lTime = localtime(&mTime);
	lTime->tm_hour = inHour;
	lTime->tm_min = inMinutes;
	lTime->tm_sec = inSeconds;
	mTime = ::mktime(lTime);
}
