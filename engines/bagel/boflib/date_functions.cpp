/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "common/system.h"
#include "bagel/boflib/date_functions.h"

namespace Bagel {

#define MAX_TIMESTR 9
#define MAX_DATESTR 9

CHAR *strtime(CHAR *pszBuffer) {
	Assert(pszBuffer != nullptr);

	TimeDate td;
	g_system->getTimeAndDate(td);

	Common::sprintf_s(pszBuffer, MAX_TIMESTR, "%.2d:%.2d:%.2d",
		td.tm_hour, td.tm_min, td.tm_sec);

	return pszBuffer;
}

CHAR *strdate(CHAR *pszBuffer) {
	Assert(pszBuffer != nullptr);

	TimeDate td;
	g_system->getTimeAndDate(td);

	Common::sprintf_s(pszBuffer, MAX_TIMESTR, "%.2d:%.2d:%.2d",
					  td.tm_mday, td.tm_mon, td.tm_year);

	return pszBuffer;
}

CHAR *strrev(CHAR *pszBuffer) {
	Assert(pszBuffer != nullptr);

	CHAR *p1, *p2, chTemp;
	INT i, n;

	n = strlen(pszBuffer);
	p1 = pszBuffer;
	p2 = pszBuffer + n - 1;
	n /= 2;

	for (i = 0; i < n; i++) {
		chTemp = *p1;
		*p1 = *p2;
		*p2 = chTemp;

		p1++;
		p2--;
	}

	return pszBuffer;
}

VOID TimeUnpack(USHORT num, USHORT *hour, USHORT *min, USHORT *sec) {
	// Can't write to nullptr pointers
	Assert((sec != nullptr) && (min != nullptr) && (hour != nullptr));

	*sec = (USHORT)((num & 0x1f) * 2);
	*min = (USHORT)((num & 0x7e0) >> 0x05);
	*hour = (USHORT)((num & 0xf800) >> 0x0b);
}

USHORT TimePack(USHORT hour, USHORT min, USHORT sec) {
	sec >>= 1;
	min <<= 0x05;
	hour <<= 0x0b;
	sec = (USHORT)(sec + min + hour);

	return sec;
}

VOID DateUnpack(USHORT num, USHORT *year, USHORT *month, USHORT *day) {
	// Validate write to nullptr pointers
	Assert((day != nullptr) && (month != nullptr) && (year != nullptr));

	*day = (USHORT)(num & 0x1f);
	*month = (USHORT)((num & 0x1e0) >> 0x05);
	*year = (USHORT)((num & 0xfe00) >> 0x09);
	*year += 1980;
}

USHORT DatePack(USHORT year, USHORT month, USHORT day) {
	month <<= 0x05;
	if (year < 100)
		year -= 80;
	else if (year >= 1980)
		year -= 1980;
	year <<= 0x09;
	day = (USHORT)(day + month + year);

	return day;
}

} // namespace Bagel
