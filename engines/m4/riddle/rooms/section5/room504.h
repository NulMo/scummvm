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

#ifndef M4_RIDDLE_ROOMS_SECTION5_ROOM504_H
#define M4_RIDDLE_ROOMS_SECTION5_ROOM504_H

#include "m4/riddle/rooms/room.h"

namespace M4 {
namespace Riddle {
namespace Rooms {

class Room504 : public Room {
private:
	machine *_ripley = nullptr;
	machine *_waterfall = nullptr;
	int _downSteps = 0;
	int _volume = 0;

	machine *_vines1 = nullptr;
	machine *_vines2 = nullptr;
	machine *_vines3 = nullptr;
	machine *_vines4 = nullptr;
	machine *_rope = nullptr;
	machine *_ladder = nullptr;
	machine *_toy = nullptr;
	machine *_shovel = nullptr;
	machine *_driftwood = nullptr;
	machine *_pole = nullptr;
	int _xyzzy2 = 0;

	int _val1 = 0;

	void setVines();
	void freeVines();
	void setVinesRope();
	void setMiscItems();
	void disableVinesRope();
	void deleteHotspot(const char *hotspotName);
	void addRope();
	void addGreenVine();
	void addBrownVine();
	void addHotspot(int x1, int y1, int x2, int y2,
		const char *verb, const char *vocab);
	void addShovel();

public:
	Room504() : Room() {}
	~Room504() override {}

	void init() override;
	void daemon() override;
	void pre_parser() override;
	void parser() override;
};

} // namespace Rooms
} // namespace Riddle
} // namespace M4

#endif
