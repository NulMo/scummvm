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
 * along with this program.  If not, see <http://www.gnu.org/licenses/ },.
 *
 */

#include "m4/riddle/rooms/section6/room647.h"
#include "m4/graphics/gr_series.h"
#include "m4/riddle/vars.h"

namespace M4 {
namespace Riddle {
namespace Rooms {

void Room647::init() {
}

void Room647::parser() {
	Maze::parser();
	checkExitLeft(646);
	checkExitRight(648);
	checkExitUp(651);

	if (player_said("up2")) {
		_G(flags)[V206] = 1;

		switch (_G(kernel).trigger) {
		case -1:
			player_set_commands_allowed(false);
			disable_player_commands_and_fade_init(1);
			_G(player).command_ready = false;
			break;

		case 1:
			digi_stop(1);
			digi_stop(2);
			adv_kill_digi_between_rooms(false);
			digi_play_loop("950_s28", 3);
			_G(game).setRoom(651);
			_G(player).command_ready = false;
			break;

		default:
			checkExitDown(641);
			break;
		}
	} else {
		checkExitDown(641);
	}
}

} // namespace Rooms
} // namespace Riddle
} // namespace M4
