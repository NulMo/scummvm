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

#ifndef ULTIMA8_GUMPS_READABLEGUMP_H
#define ULTIMA8_GUMPS_READABLEGUMP_H

#include "ultima8/gumps/modal_gump.h"
#include "ultima8/usecode/intrinsics.h"
#include "ultima8/misc/p_dynamic_cast.h"

namespace Ultima8 {

class ReadableGump : public ModalGump {
	uint16 shapenum;
	int fontnum;
	std::string text;
public:
	ENABLE_RUNTIME_CLASSTYPE()

	ReadableGump();
	ReadableGump(ObjId owner, uint16 shape, int font, std::string msg);
	virtual ~ReadableGump();

	// Close on mouse click on key press
	virtual Gump *OnMouseDown(int button, int mx, int my);
	virtual bool OnKeyDown(int key, int mod);

	// Init the gump, call after construction
	virtual void InitGump(Gump *newparent, bool take_focus = true);

	INTRINSIC(I_readGrave);
	INTRINSIC(I_readPlaque);

	bool loadData(IDataSource *ids, uint32 version);
protected:
	virtual void saveData(ODataSource *ods);
};

} // End of namespace Ultima8

#endif
