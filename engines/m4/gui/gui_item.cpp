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

#include "m4/gui/gui_item.h"
#include "m4/graphics/gr_pal.h"
#include "m4/globals.h"

namespace M4 {

#define _REDRAW_ALL 2
#define _REDRAW_ACTIVE 4
#define _INIT_LISTBOX 8

#define _LB_BLACK	__BLACK
#define _LB_LTGREY	0x07		//used to be 0x0c
#define _LB_DKGREY	0x08		//used to be 0x04
#define _LB_WHITE	0x0f
#define _LB_BROWN	0x07


//INTRFACE SPRITES
#define scrollUpWidth		11
#define scrollUpHeight		12
static const byte scrollUpData[] = {
	_LB_LTGREY, _LB_LTGREY, _LB_LTGREY, _LB_LTGREY, _LB_LTGREY, _LB_LTGREY, _LB_LTGREY, _LB_LTGREY, _LB_LTGREY, _LB_LTGREY, _LB_LTGREY,
	_LB_LTGREY, _LB_LTGREY, _LB_LTGREY, _LB_LTGREY, _LB_LTGREY,  _LB_WHITE, _LB_LTGREY, _LB_LTGREY, _LB_LTGREY, _LB_LTGREY, _LB_LTGREY,
	_LB_LTGREY, _LB_LTGREY, _LB_LTGREY, _LB_LTGREY,  _LB_WHITE,  _LB_BLACK,  _LB_WHITE, _LB_LTGREY, _LB_LTGREY, _LB_LTGREY, _LB_LTGREY,
	_LB_LTGREY, _LB_LTGREY, _LB_LTGREY,  _LB_WHITE,  _LB_BLACK,  _LB_BROWN,  _LB_BROWN,  _LB_WHITE, _LB_LTGREY, _LB_LTGREY, _LB_LTGREY,
	_LB_LTGREY, _LB_LTGREY,  _LB_WHITE,  _LB_BLACK,  _LB_BROWN,  _LB_BROWN,  _LB_BROWN,  _LB_BROWN,  _LB_WHITE, _LB_LTGREY, _LB_LTGREY,
	_LB_LTGREY,  _LB_WHITE,  _LB_BLACK,  _LB_BROWN,  _LB_BROWN,  _LB_BROWN,  _LB_BROWN,  _LB_BROWN, _LB_DKGREY,  _LB_WHITE, _LB_LTGREY,
	_LB_LTGREY,  _LB_WHITE,  _LB_WHITE,  _LB_WHITE,  _LB_BROWN,  _LB_BROWN, _LB_DKGREY,  _LB_WHITE,  _LB_WHITE,  _LB_WHITE, _LB_LTGREY,
	_LB_LTGREY, _LB_LTGREY, _LB_LTGREY,  _LB_WHITE,  _LB_BROWN,  _LB_BROWN, _LB_DKGREY,  _LB_WHITE, _LB_LTGREY, _LB_LTGREY, _LB_LTGREY,
	_LB_LTGREY, _LB_LTGREY, _LB_LTGREY,  _LB_WHITE,  _LB_BROWN,  _LB_BROWN, _LB_DKGREY,  _LB_WHITE, _LB_LTGREY, _LB_LTGREY, _LB_LTGREY,
	_LB_LTGREY, _LB_LTGREY, _LB_LTGREY,  _LB_WHITE, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY,  _LB_WHITE, _LB_LTGREY, _LB_LTGREY, _LB_LTGREY,
	_LB_LTGREY, _LB_LTGREY, _LB_LTGREY,  _LB_WHITE,  _LB_WHITE,  _LB_WHITE,  _LB_WHITE,  _LB_WHITE, _LB_LTGREY, _LB_LTGREY, _LB_LTGREY,
	_LB_LTGREY, _LB_LTGREY, _LB_LTGREY, _LB_LTGREY, _LB_LTGREY, _LB_LTGREY, _LB_LTGREY, _LB_LTGREY, _LB_LTGREY, _LB_LTGREY, _LB_LTGREY
};
static const Buffer scrollUpBuff = { scrollUpWidth, scrollUpHeight, (uint8 *)&scrollUpData[0], 0 };

static const byte scrollUpPressedData[] = {
	_LB_DKGREY, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY,
	_LB_DKGREY, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY,  _LB_WHITE, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY,
	_LB_DKGREY, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY,  _LB_WHITE, _LB_DKGREY,  _LB_WHITE, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY,
	_LB_DKGREY, _LB_DKGREY, _LB_DKGREY,  _LB_WHITE, _LB_DKGREY,  _LB_BROWN,  _LB_BROWN,  _LB_WHITE, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY,
	_LB_DKGREY, _LB_DKGREY,  _LB_WHITE, _LB_DKGREY,  _LB_BROWN,  _LB_BROWN,  _LB_BROWN,  _LB_BROWN,  _LB_WHITE, _LB_DKGREY, _LB_DKGREY,
	_LB_DKGREY,  _LB_WHITE, _LB_DKGREY,  _LB_BROWN,  _LB_BROWN,  _LB_BROWN,  _LB_BROWN, _LB_LTGREY,  _LB_BLACK,  _LB_WHITE, _LB_DKGREY,
	_LB_DKGREY,  _LB_WHITE,  _LB_WHITE,  _LB_WHITE,  _LB_BROWN,  _LB_BROWN, _LB_LTGREY,  _LB_WHITE,  _LB_WHITE,  _LB_WHITE, _LB_DKGREY,
	_LB_DKGREY, _LB_DKGREY, _LB_DKGREY,  _LB_WHITE,  _LB_BROWN,  _LB_BROWN, _LB_LTGREY,  _LB_WHITE, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY,
	_LB_DKGREY, _LB_DKGREY, _LB_DKGREY,  _LB_WHITE,  _LB_BROWN,  _LB_BROWN,  _LB_BLACK,  _LB_WHITE, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY,
	_LB_DKGREY, _LB_DKGREY, _LB_DKGREY,  _LB_WHITE, _LB_LTGREY,  _LB_BLACK,  _LB_BLACK,  _LB_WHITE, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY,
	_LB_DKGREY, _LB_DKGREY, _LB_DKGREY,  _LB_WHITE,  _LB_WHITE,  _LB_WHITE,  _LB_WHITE,  _LB_WHITE, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY,
	_LB_DKGREY, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY,
};
static const Buffer scrollUpPressedBuff = { scrollUpWidth, scrollUpHeight, (uint8 *)&scrollUpPressedData[0], 0 };

#define scrollDownWidth		11
#define scrollDownHeight	12
static const byte scrollDownData[] = {
	_LB_LTGREY, _LB_LTGREY, _LB_LTGREY, _LB_LTGREY, _LB_LTGREY, _LB_LTGREY, _LB_LTGREY, _LB_LTGREY, _LB_LTGREY, _LB_LTGREY, _LB_LTGREY,
	_LB_LTGREY, _LB_LTGREY, _LB_LTGREY,  _LB_WHITE,  _LB_WHITE,  _LB_WHITE,  _LB_WHITE,  _LB_WHITE, _LB_LTGREY, _LB_LTGREY, _LB_LTGREY,
	_LB_LTGREY, _LB_LTGREY, _LB_LTGREY,  _LB_WHITE,  _LB_BLACK,  _LB_BLACK,  _LB_BLACK,  _LB_WHITE, _LB_LTGREY, _LB_LTGREY, _LB_LTGREY,
	_LB_LTGREY, _LB_LTGREY, _LB_LTGREY,  _LB_WHITE,  _LB_BLACK,  _LB_BROWN,  _LB_BROWN,  _LB_WHITE, _LB_LTGREY, _LB_LTGREY, _LB_LTGREY,
	_LB_LTGREY, _LB_LTGREY, _LB_LTGREY,  _LB_WHITE,  _LB_BLACK,  _LB_BROWN,  _LB_BROWN,  _LB_WHITE, _LB_LTGREY, _LB_LTGREY, _LB_LTGREY,
	_LB_LTGREY,  _LB_WHITE,  _LB_WHITE,  _LB_WHITE,  _LB_BLACK,  _LB_BROWN,  _LB_BROWN,  _LB_WHITE,  _LB_WHITE,  _LB_WHITE, _LB_LTGREY,
	_LB_LTGREY,  _LB_WHITE,  _LB_BLACK,  _LB_BLACK,  _LB_BROWN,  _LB_BROWN,  _LB_BROWN,  _LB_BROWN, _LB_DKGREY,  _LB_WHITE, _LB_LTGREY,
	_LB_LTGREY, _LB_LTGREY,  _LB_WHITE,  _LB_BROWN,  _LB_BROWN,  _LB_BROWN,  _LB_BROWN, _LB_DKGREY,  _LB_WHITE, _LB_LTGREY, _LB_LTGREY,
	_LB_LTGREY, _LB_LTGREY, _LB_LTGREY,  _LB_WHITE,  _LB_BROWN,  _LB_BROWN, _LB_DKGREY,  _LB_WHITE, _LB_LTGREY, _LB_LTGREY, _LB_LTGREY,
	_LB_LTGREY, _LB_LTGREY, _LB_LTGREY, _LB_LTGREY,  _LB_WHITE, _LB_DKGREY,  _LB_WHITE, _LB_LTGREY, _LB_LTGREY, _LB_LTGREY, _LB_LTGREY,
	_LB_LTGREY, _LB_LTGREY, _LB_LTGREY, _LB_LTGREY, _LB_LTGREY,  _LB_WHITE, _LB_LTGREY, _LB_LTGREY, _LB_LTGREY, _LB_LTGREY, _LB_LTGREY,
	_LB_LTGREY, _LB_LTGREY, _LB_LTGREY, _LB_LTGREY, _LB_LTGREY, _LB_LTGREY, _LB_LTGREY, _LB_LTGREY, _LB_LTGREY, _LB_LTGREY, _LB_LTGREY
};
static const Buffer scrollDownBuff = { scrollDownWidth, scrollDownHeight, (uint8 *)&scrollDownData[0], 0 };

static const byte scrollDownPressedData[] = {
	_LB_DKGREY, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY,
	_LB_DKGREY, _LB_DKGREY, _LB_DKGREY,  _LB_WHITE,  _LB_WHITE,  _LB_WHITE,  _LB_WHITE,  _LB_WHITE, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY,
	_LB_DKGREY, _LB_DKGREY, _LB_DKGREY,  _LB_WHITE, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY,  _LB_WHITE, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY,
	_LB_DKGREY, _LB_DKGREY, _LB_DKGREY,  _LB_WHITE, _LB_DKGREY,  _LB_BROWN,  _LB_BROWN,  _LB_WHITE, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY,
	_LB_DKGREY, _LB_DKGREY, _LB_DKGREY,  _LB_WHITE, _LB_DKGREY,  _LB_BROWN,  _LB_BROWN,  _LB_WHITE, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY,
	_LB_DKGREY,  _LB_WHITE,  _LB_WHITE,  _LB_WHITE, _LB_DKGREY,  _LB_BROWN,  _LB_BROWN,  _LB_WHITE,  _LB_WHITE,  _LB_WHITE, _LB_DKGREY,
	_LB_DKGREY,  _LB_WHITE, _LB_DKGREY, _LB_DKGREY,  _LB_BROWN,  _LB_BROWN,  _LB_BROWN,  _LB_BROWN,  _LB_BLACK,  _LB_WHITE, _LB_DKGREY,
	_LB_DKGREY, _LB_DKGREY,  _LB_WHITE,  _LB_BROWN,  _LB_BROWN,  _LB_BROWN,  _LB_BROWN,  _LB_BLACK,  _LB_WHITE, _LB_DKGREY, _LB_DKGREY,
	_LB_DKGREY, _LB_DKGREY, _LB_DKGREY,  _LB_WHITE,  _LB_BROWN,  _LB_BROWN,  _LB_BLACK,  _LB_WHITE, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY,
	_LB_DKGREY, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY,  _LB_WHITE,  _LB_BLACK,  _LB_WHITE, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY,
	_LB_DKGREY, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY,  _LB_WHITE, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY,
	_LB_DKGREY, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY
};
static const Buffer scrollDownPressedBuff = { scrollDownWidth, scrollDownHeight, (uint8 *)&scrollDownPressedData[0], 0 };

#define thumbWidth			11
#define thumbHeight			11
static const byte thumbData[] = {
	_LB_LTGREY, _LB_LTGREY, _LB_LTGREY, _LB_LTGREY, _LB_LTGREY, _LB_LTGREY, _LB_LTGREY, _LB_LTGREY, _LB_LTGREY, _LB_LTGREY, _LB_LTGREY,
	_LB_LTGREY, _LB_LTGREY, _LB_LTGREY,  _LB_BLACK,  _LB_BLACK,  _LB_BLACK,  _LB_BLACK,  _LB_BLACK, _LB_LTGREY, _LB_LTGREY, _LB_LTGREY,
	_LB_LTGREY, _LB_LTGREY,  _LB_BLACK,  _LB_BROWN,  _LB_BROWN,  _LB_BROWN,  _LB_BROWN,  _LB_BROWN,  _LB_WHITE, _LB_LTGREY, _LB_LTGREY,
	_LB_LTGREY, _LB_LTGREY,  _LB_BLACK, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY,  _LB_WHITE, _LB_LTGREY, _LB_LTGREY,
	_LB_LTGREY, _LB_LTGREY,  _LB_BLACK,  _LB_BROWN,  _LB_BROWN,  _LB_BROWN,  _LB_BROWN,  _LB_BROWN,  _LB_WHITE, _LB_LTGREY, _LB_LTGREY,
	_LB_LTGREY, _LB_LTGREY,  _LB_BLACK, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY,  _LB_WHITE, _LB_LTGREY, _LB_LTGREY,
	_LB_LTGREY, _LB_LTGREY,  _LB_BLACK,  _LB_BROWN,  _LB_BROWN,  _LB_BROWN,  _LB_BROWN,  _LB_BROWN,  _LB_WHITE, _LB_LTGREY, _LB_LTGREY,
	_LB_LTGREY, _LB_LTGREY,  _LB_BLACK, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY,  _LB_WHITE, _LB_LTGREY, _LB_LTGREY,
	_LB_LTGREY, _LB_LTGREY,  _LB_BLACK,  _LB_BROWN,  _LB_BROWN,  _LB_BROWN,  _LB_BROWN,  _LB_BROWN,  _LB_WHITE, _LB_LTGREY, _LB_LTGREY,
	_LB_LTGREY, _LB_LTGREY, _LB_LTGREY,  _LB_WHITE,  _LB_WHITE,  _LB_WHITE,  _LB_WHITE,  _LB_WHITE,  _LB_WHITE, _LB_LTGREY, _LB_LTGREY,
	_LB_LTGREY, _LB_LTGREY, _LB_LTGREY, _LB_LTGREY, _LB_LTGREY, _LB_LTGREY, _LB_LTGREY, _LB_LTGREY, _LB_LTGREY, _LB_LTGREY, _LB_LTGREY
};
static const Buffer thumbBuff = { thumbWidth, thumbHeight, (uint8 *)&thumbData[0], 0 };

static const byte thumbPressedData[] = {
	_LB_DKGREY, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY,
	_LB_DKGREY, _LB_DKGREY, _LB_DKGREY,  _LB_WHITE,  _LB_WHITE,  _LB_WHITE,  _LB_WHITE,  _LB_WHITE, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY,
	_LB_DKGREY, _LB_DKGREY,  _LB_WHITE,  _LB_BROWN,  _LB_BROWN,  _LB_BROWN,  _LB_BROWN,  _LB_BROWN,  _LB_BLACK, _LB_DKGREY, _LB_DKGREY,
	_LB_DKGREY, _LB_DKGREY,  _LB_WHITE, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY,  _LB_BLACK, _LB_DKGREY, _LB_DKGREY,
	_LB_DKGREY, _LB_DKGREY,  _LB_WHITE,  _LB_BROWN,  _LB_BROWN,  _LB_BROWN,  _LB_BROWN,  _LB_BROWN,  _LB_BLACK, _LB_DKGREY, _LB_DKGREY,
	_LB_DKGREY, _LB_DKGREY,  _LB_WHITE, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY,  _LB_BLACK, _LB_DKGREY, _LB_DKGREY,
	_LB_DKGREY, _LB_DKGREY,  _LB_WHITE,  _LB_BROWN,  _LB_BROWN,  _LB_BROWN,  _LB_BROWN,  _LB_BROWN,  _LB_BLACK, _LB_DKGREY, _LB_DKGREY,
	_LB_DKGREY, _LB_DKGREY,  _LB_WHITE, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY,  _LB_BLACK, _LB_DKGREY, _LB_DKGREY,
	_LB_DKGREY, _LB_DKGREY,  _LB_WHITE,  _LB_BROWN,  _LB_BROWN,  _LB_BROWN,  _LB_BROWN,  _LB_BROWN,  _LB_BLACK, _LB_DKGREY, _LB_DKGREY,
	_LB_DKGREY, _LB_DKGREY, _LB_DKGREY,  _LB_BLACK,  _LB_BLACK,  _LB_BLACK,  _LB_BLACK,  _LB_BLACK,  _LB_BLACK, _LB_DKGREY, _LB_DKGREY,
	_LB_DKGREY, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY, _LB_DKGREY
};
static const Buffer thumbPressedBuff = { thumbWidth, thumbHeight, (uint8 *)&thumbPressedData[0], 0 };

#define minThumbY (scrollUpHeight + buttonHeight)
#define maxThumbY (scrollDownHeight + thumbHeight + (buttonHeight<<1))
#define thumbYRange (scrollUpHeight + scrollUpHeight + thumbHeight + (buttonHeight<<2) + buttonHeight - 1)



} // End of namespace M4
