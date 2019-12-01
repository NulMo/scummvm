/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifndef ULTIMA8_USECODE_REMORSEINTRINSICS_H
#define ULTIMA8_USECODE_REMORSEINTRINSICS_H

#include "ultima8/usecode/intrinsics.h"
#include "ultima8/world/item.h"

namespace Ultima8 {

// Crusader: No Remorse Intrinsics
Intrinsic RemorseIntrinsics[] = {
	// 0x000
	0,
	Item::I_getFrame,
	Item::I_setFrame,
	Item::I_getMapArray,
	Item::I_getStatus,
	Item::I_orStatus,
	0,
	0,
	0,
	Item::I_getZ,
	0,
	0, // something with npcdata
	0,
	0,
	0,
	0,
	// 0x010
	Item::I_getQLo,
	0,
	0,
	Item::I_getX,
	Item::I_getY,
	0, // something with sound?
	Item::I_getShape,
	0,
	0,
	0,
	Item::I_setQLo,
	Item::I_popToCoords, // different than U8's?
	Item::I_andStatus,
	Item::I_create,
	0,
	0,
	// 0x020
	0,
	0,
	Item::I_push,
	0,
	Item::I_setShape, // different than U8's?
	Item::I_touch,
	Item::I_getQHi,
	0,
	0,
	0,
	0,
	Item::I_getQLo,
	0,
	Item::I_setQHi,
	0,
	0,
	// 0x030
	Item::I_pop,
	Item::I_andStatus,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0, // something with sound?
	Item::I_getFamily,
	0,
	0,
	0,
	0,
	// 0x040
	0,
	0,
	0,
	0,
	Item::I_getQHi,
	0,
	Item::I_getQHi,
	0,
	Item::I_getQHi,
	0,
	Item::I_getQHi,
	0,
	Item::I_getQHi,
	0,
	Item::I_getQHi,
	0,
	// 0x050
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	Item::I_setFrame,
	0,
	Item::I_legalCreateAtPoint,
	0,
	0,
	0,
	0,
	0,
	// 0x060
	0,
	0,
	Item::I_legalCreateAtCoords,
	0,
	Item::I_getQLo,
	Item::I_getNpcNum,
	0,
	Item::I_andStatus,
	0,
	0,
	0,
	Item::I_getNpcNum,
	Item::I_andStatus,
	0,
	0,
	0,
	// 0x070
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	Item::I_getQuality,
	Item::I_setQuality,
	Item::I_use, // different than U8's?
	0,
	0,
	0,
	Item::I_getQLo,
	0,
	0,
	// 0x080
	0,
	0,
	Item::I_getNpcNum,
	0,
	0,
	0,
	0,
	Item::I_getNpcNum,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	// 0x090
	0,
	0,
	Item::I_andStatus,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	Item::I_getQLo,
	Item::I_getUnkEggType,
	0,
	0,
	0,
	0,
	// 0x0A0
	0,
	0,
	0,
	0,
	0,
	Item::I_getFamilyOfType,
	Item::I_getNpcNum,
	Item::I_getQLo,
	Item::I_getQHi,
	0, // call usecode event B
	0,
	Item::I_andStatus,
	GUIApp::I_getCurrentTimerTick,
	0,
	0,
	0,
	// 0x0B0
	0,
	0,
	0,
	0,
	0,
	0,
	Item::I_getQHi,
	Item::I_andStatus,
	0,
	Item::I_andStatus,
	0,
	Item::I_andStatus,
	0,
	0,
	0,
	0,
	// 0x0C0
	0,
	Item::I_getQHi,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	// 0x0D0
	0,
	Item::I_getQLo,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	// 0x0E0
	0,
	0,
	Item::I_andStatus,
	Item::I_getQLo,
	0,
	0,
	Item::I_popToContainer,
	0,
	0,
	Item::I_getQHi,
	0,
	0,
	Item::I_getQHi,
	Item::I_getQ,
	Item::I_setQ,
	0,
	// 0x0F0
	0,
	Item::I_getNpcNum,
	0,
	0,
	Item::I_getQHi,
	Item::I_andStatus,
	0,
	0,
	0,
	0,
	0, // FA = integer to string
	Item::I_getNpcNum,
	Item::I_andStatus,
	Item::I_getNpcNum,
	0,
	Item::I_isCrusTypeNPC,
	// 0x100
	Item::I_andStatus,
	Item::I_getNpcNum,
	Item::I_andStatus,
	Item::I_getNpcNum,
	Item::I_andStatus,
	Item::I_getNpcNum,
	Item::I_andStatus,
	Item::I_getNpcNum,
	Item::I_andStatus,
	Item::I_getNpcNum,
	Item::I_andStatus,
	Item::I_getNpcNum,
	Item::I_andStatus,
	Item::I_getNpcNum,
	0,
	Item::I_andStatus,
	// 0x110
	Item::I_getNpcNum,
	0,
	Item::I_andStatus,
	0,
	0,
	Item::I_andStatus,
	Item::I_getTypeFlag,
	Item::I_getNpcNum,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	Item::I_getNpcNum,
	// 0x120
	0,
	0,
	0,
	0,
	Item::I_andStatus,
	Item::I_getNpcNum,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	Item::I_getQHi,
	// 0x130
	Item::I_andStatus,
	0,
	0,
	0
};

} // End of namespace Ultima8

#endif
