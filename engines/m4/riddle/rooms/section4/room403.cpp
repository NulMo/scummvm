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

#include "m4/riddle/rooms/section4/room403.h"
#include "m4/riddle/rooms/section4/section4.h"
#include "m4/graphics/gr_series.h"
#include "m4/riddle/vars.h"
#include "m4/gui/gui_vmng.h"

namespace M4 {
namespace Riddle {
namespace Rooms {

static const char *const SAID[][2] = {
	{ "BROKEN HEADSTONE",    "403r11" },
	{ "POMMEE CROSS",        "403r12" },
	{  "GRAVE PLAQUE",       "403r13" },
	{ "GRAVE",               "403r18" },
	{ "1ST MARBLE MONUMENT", "403r15" },
	{ "2ND MARBLE MONUMENT", "403r26" },
	{ "MARBLE COLUMN",       "403r16" },
	{ "SMALL HEADSTONE",     "403r17" },
	{ "MARBLE HEADSTONE",    "403r27" },
	{ "TOMBSTONE",           "403r19" },
	{ "BURIAL TABLET",       "403r20" },
	{ "SMALL GRAVE MARKER",  "403r14" },
	{ "TALL HEADSTONE",      "403r22" },
	{ "GRANITE HEADSTONE",   "403r23" },
	{ "BURIAL PLAQUE",       "403r24" },
	{ "CELTIC CROSS",        "403r25" },
	{ "URN",                 "403r08" },
	{ nullptr, nullptr }
};

void Room403::preload() {
	_G(player).walker_type = 1;
	_G(player).shadow_type = 1;
	LoadWSAssets("OTHER SCRIPT");
}

void Room403::init() {
	digi_preload("403_s02");
	digi_preload("403_s02a");
	digi_preload("403_s02b");
	digi_preload("950_s23");
	digi_preload("403_s01");

	player_set_commands_allowed(false);
	_bell = series_place_sprite("ONE FRAME BELL", 0, 0, 0, 100, 0xf00);

	if (_G(game).previous_room != KERNEL_RESTORING_GAME) {
		_val1 = 0;
		_val2 = -1;
		_val3 = 0;
		_val4 = -1;
		_val5 = 0;
		_val6 = 0;
		_val7 = 0;
		_val8 = 0;
		_val9 = 0;
		_val10 = 0;
		_sound1.clear();
		_val12 = 0;

		_G(flags)[V313] = player_been_here(403) && (
			(_G(flags)[V110] && inv_player_has("TURTLE")) ||
			inv_player_has("STEP LADDER") ||
			_G(flags)[GLB_TEMP_12] ||
			!inv_object_is_here("STEP LADDER")) ? 0 : 1;
		_plank = inv_object_in_scene("PLANK", 403) ? 2 : 0;

		_ventClosed = series_show("SPRITE OF VENT CLOSED", 0x600, 16);
	}

	_safariShadow = series_load("SAFARI SHADOW 3");
	hotspot_set_active("WOLF", false);
	hotspot_set_active("STEP LADDER", false);
	hotspot_set_active("STEP LADDER ", false);
	hotspot_set_active("EDGER", false);
	hotspot_set_active("PLANK", false);
	hotspot_set_active("TURTLE TREAT", false);

	if (_G(flags)[V139] == 2) {
		_G(flags)[V139] = 0;

		if (_G(flags)[V133] && _G(flags)[V131] != 403) {
			_edger = series_place_sprite("ONE FRAME EDGER", 0, 0, 0, 100, 0xf00);
			hotspot_set_active("EDGER", true);
			inv_move_object("EDGER", 403);
		}

		MoveScreenDelta(-640, 0);
		ws_demand_location(1172, 322, 3);
		ws_walk(1172, 322, nullptr, 400, 1);

	} else if (_G(flags)[V139] == 4) {
		_G(flags)[V139] = 0;
		_ladder = series_place_sprite("LADDER LEANS AGAINST WALL", 0, 0, 0, 100, 0xf00);
		hotspot_set_active("STEP LADDER ", true);
		MoveScreenDelta(-640, 0);
		ws_demand_location(1083, 322, 3);
		ws_walk(1201, 321, nullptr, 420, 2);

	} else {
		if (inv_player_has("TURTLE"))
			_G(flags)[V313] = 0;

		switch (_G(flags)[V313]) {
		case 1:
			_ladder = series_place_sprite("LADDER LEANS AGAINST WALL", 0, 0, 0, 100, 0xf00);
			hotspot_set_active("STEP LADDER ", true);
			break;
		case 2:
			_ladder = series_place_sprite("1 sprite of ladder", 0, 0, 0, 100, 0xf00);
			hotspot_set_active("STEP LADDER", true);
			break;
		default:
			break;
		}

		if (_G(flags)[V133] && !_G(flags)[GLB_TEMP_12] && _G(flags)[V131] != 403 &&
				!inv_player_has("TURTLE") && !inv_player_has("EDGER")) {
			_edger = series_place_sprite("ONE FRAME EDGER", 0, 0, 0, 0x100, 0xf00);
			hotspot_set_active("EDGER", true);
		}

		if (inv_object_is_here("PLANK") || _plank == 2) {
			_board = series_place_sprite("1 SPRITE OF BOARD", 0, 0, 0, 100, 0xf00);
			hotspot_set_active("PLANK", true);
			_plank = true;
		}

		if (_G(game).previous_room == KERNEL_RESTORING_GAME) {
			if (_G(flags)[V131] == 403) {
				hotspot_set_active("WOLF", true);
				_wolfTurnTalk = series_load("WOLF TURN AND TALK");
				_wolfTurnHand = series_load("WOLF TURN WITH HAND OUT");
				_wolfTalkLeave = series_load("WOLF TALKS AND LEAVES");
				_ripTalkPay = series_load("RIP TALK PAY LOOP");
				_wolfEdger = series_load("WOLF EDGER LOOP");

				_wolfie = TriggerMachineByHash(1, 1, 0, 0, 0, 0, 0, 0, 100, 0x300, 0,
					triggerMachineByHashCallbackNegative, "WOLFIE");

				if (_val12) {
					sendWSMessage_10000(1, _wolfie, _wolfTurnHand, 45, 45, -1,
						_wolfTurnHand, 45, 45, 0);
				} else {
					_val8 = 2001;
					_val9 = 2300;
					sendWSMessage_10000(1, _wolfie, _wolfEdger, 1, 6, 110,
						_wolfEdger, 6, 6, 0);
				}
			}

			if (_val10) {
				ws_demand_facing(11);
				ws_hide_walker();
				_ripOnLadder = TriggerMachineByHash(1, 1, 0, 0, 0, 0, 0, 0, 100, 0x100, 0,
					triggerMachineByHashCallbackNegative, "RIP on ladder/plank");

				switch (_val10) {
				case 1:
					_ripClimbsLadder = series_load("RIPLEY CLIMBS LADDER");
					sendWSMessage_10000(1, _ripOnLadder, _ripClimbsLadder, 52, 52, -1,
						_ripClimbsLadder, 52, 52, 0);
					break;
				case 2:
					_ripClimbsLadder = series_load("RIPLEY CLIMBS LADDER");
					sendWSMessage_10000(1, _ripOnLadder, _ripClimbsLadder, 74, 74, -1,
						_ripClimbsLadder, 74, 74, 0);
					break;
				case 3:
					_ripLegUp = series_load("RIP GETS A LEG UP");
					_ventClosed = series_show("SPRITE OF VENT CLOSED", 0x600, 16);
					sendWSMessage_10000(1, _ripOnLadder, _ripLegUp, 44, 44, -1,
						_ripLegUp, 44, 44, 0);
					break;
				case 4:
					_ripLegUp = series_load("RIP GETS A LEG UP");
					_ripTurtle = series_load("RIP TURTLE SERIES");
					_noTreat = series_load("403RP06 NO TREAT");
					sendWSMessage_10000(1, _ripOnLadder, _noTreat, 28, 28, -1,
						_noTreat, 28, 28, 0);
					break;
				case 5:
					_ripLegUp = series_load("RIP GETS A LEG UP");
					_ripTurtle = series_load("RIP TURTLE SERIES");
					_noTreat = series_load("403RP06 NO TREAT");
					hotspot_set_active("GRATE", false);
					hotspot_set_active("TURTLE TREAT", true);

					if (_G(flags)[V125] == 2)
						sendWSMessage_10000(1, _ripOnLadder, _noTreat, 52, 52, -1,
						_noTreat, 52, 52, 0);
					else
						sendWSMessage_10000(1, _ripOnLadder, _noTreat, 28, 28, -1,
							_noTreat, 28, 28, 0);
					break;
				default:
					break;
				}
			} else {
				_ventClosed = series_show("SPRITE OF VENT CLOSED", 0x600, 16);
			}
		} else if (_G(flags)[V132]) {
			_G(flags)[V132] = 0;
			_G(camera_reacts_to_player) = false;
			MoveScreenDelta(-640, 0);
			ws_demand_location(620, 326, 3);
			ws_walk_load_shadow_series(S4_SHADOW_DIRS, S4_SHADOW_NAMES);
			ws_walk_load_walker_series(S4_NORMAL_DIRS, S4_NORMAL_NAMES);
			kernel_timing_trigger(1, 310);
		} else {
			if (_G(flags)[V131] == 403) {
				hotspot_set_active("WOLF", true);
				_wolfTurnTalk = series_load("WOLF TURN AND TALK");
				_wolfTurnHand = series_load("WOLF TURN WITH HAND OUT");
				_wolfTalkLeave = series_load("WOLF TALKS AND LEAVES");
				_ripTalkPay = series_load("RIP TALK PAY LOOP");
				_wolfEdger = series_load("WOLF EDGER LOOP");

				_wolfie = TriggerMachineByHash(1, 1, 0, 0, 0, 0, 0, 0, 100, 0x300, 0,
					triggerMachineByHashCallbackNegative, "WOLFIE");
				sendWSMessage_10000(1, _wolfie, _wolfEdger, 1, 6, 110,
					_wolfEdger, 6, 6, 0);
				_val8 = 2001;
				_val9 = 2300;
			}

			if (0) {
				ws_demand_location(4, 296);
				ws_walk(80, 300, nullptr, 300, 3);
			} else {
				MoveScreenDelta(-640, 0);
				ws_demand_location(1110, 322);
			}
		}
	}

	digi_play_loop("403_s01", 3, 180);
}

void Room403::daemon() {

}

#define TRIGGER _G(kernel).trigger_mode = KT_DAEMON; \
	kernel_timing_trigger(1, 102); \
	_G(kernel).trigger_mode = KT_PARSE
#define MONEY(ITEM) (player_said(ITEM, "WOLF") && inv_player_has(ITEM))

void Room403::pre_parser() {
	bool talkFlag = player_said_any("talk", "talk to");
	bool lookFlag = player_said_any("look", "look at");
	bool enterFlag = player_said("enter");
	bool useFlag = player_said_any("push", "pull", "gear", "open", "close");

	if (_val10 == 5) {
		intr_cancel_sentence();
		_G(player).need_to_walk = false;
		_G(player).ready_to_walk = true;
		_G(player).waiting_for_walk = false;

		if (player_said("TURTLE", "TURTLE TREAT")) {
			_val6 = 1020;
			_val7 = 1410;
			inv_move_object("TURTLE", 403);
		} else if (player_said("TURTLE TREATS", "TURTLE TREAT")) {
			_val6 = 1020;
			_val7 = 1400;
		} else if (player_said("EDGER", "TURTLE TREAT")) {
			if (_G(flags)[V125] != 2) {
				_val6 = 1020;
				_val7 = 1500;
			}
		} else {
			_val6 = 1020;
			_val7 = 1330;
		}

		TRIGGER;
		return;
	}

	if (_val10 == 4) {
		intr_cancel_sentence();
		_G(player).need_to_walk = false;
		_G(player).ready_to_walk = true;
		_G(player).waiting_for_walk = false;

		if (lookFlag && player_said("GRATE")) {
			_val6 = 1020;
			_val7 = 1340;
		} else if (player_said("TURTLE TREATS", "GRATE")) {
			_val6 = 1020;
			_val7 = 1400;
		} else if (player_said("TURTLE", "GRATE")) {
			_val6 = 1020;
			_val7 = 1410;
			inv_move_object("TURTLE", 403);
		} else {
			_val6 = 1020;
			_val7 = 1330;
		}

		TRIGGER;
		return;
	}

	if (_val10 == 3) {
		_G(player).need_to_walk = false;
		_G(player).ready_to_walk = true;
		_G(player).waiting_for_walk = false;

		if (lookFlag && player_said("grate")) {
			return;
		}

		intr_cancel_sentence();

		if (useFlag && player_said("GRATE")) {
			_val6 = 1020;
			_val7 = 1320;
		} else {
			_val6 = 1020;
			_val7 = 1310;
		}

		TRIGGER;
		return;
	}

	if (_val10 == 2) {
		intr_cancel_sentence();
		_G(player).need_to_walk = false;
		_G(player).ready_to_walk = true;
		_G(player).waiting_for_walk = false;

		if (lookFlag && player_said("GRATE")) {
			_val6 = 1010;
			_val7 = 1240;
		} else {
			_val6 = 1010;
			_val7 = 1230;
		}

		TRIGGER;
	}

	if (_val10 == 1) {
		_G(player).need_to_walk = false;
		_G(player).ready_to_walk = true;
		_G(player).waiting_for_walk = false;

		if (!(lookFlag && player_said("GRATE"))) {
			intr_cancel_sentence();

			if (useFlag && player_said("GRATE")) {
				_val6 = 1010;
				_val7 = 1220;
			} else {
				_val6 = 1010;
				_val7 = 1210;
			}

			TRIGGER;
		}
	}

	if (player_said("PLANK", "URN") || player_said("EDGER", "URN")) {
		_G(player).need_to_walk = false;
		_G(player).ready_to_walk = true;
		_G(player).waiting_for_walk = false;

		_G(kernel).trigger_mode = KT_PARSE;
		kernel_timing_trigger(1, 69);
		_G(kernel).trigger_mode = KT_PREPARSE;
	}

	if (!_flag1 && !player_said("WALK TO") &&
			!player_said("POMERANIAN MARKS", "WOLF"))
		_flag1 = true;

	if (_val12) {
		_val12 = 0;

		if (MONEY("POMERANIAN MARKS")) {
			player_set_commands_allowed(false);
			intr_cancel_sentence();
			_G(flags)[V114] = 1;
			_val8 = 2000;
			_val9 = 2232;
			_G(flags)[V111]++;

			_G(kernel).trigger_mode = KT_DAEMON;
			kernel_timing_trigger(1, 69);
			_G(kernel).trigger_mode = KT_PREPARSE;
		} else if (MONEY("US DOLLARS") || MONEY("CHINESE YUAN") ||
				MONEY("PERUVIAN INTI") || MONEY("SIKKIMESE RUPEE")) {
			_G(flags)[V116] = 1;
			intr_cancel_sentence();
			_G(kernel).trigger_mode = KT_DAEMON;
			kernel_timing_trigger(1, 230);
			_G(kernel).trigger_mode = KT_PREPARSE;
		} else {
			if (talkFlag && player_said("WOLF"))
				intr_cancel_sentence();

			player_set_commands_allowed(false);
			_val8 = 2000;
			_val9 = 2234;
			_G(kernel).trigger_mode = KT_DAEMON;
			kernel_timing_trigger(1, 110);
			_G(kernel).trigger_mode = KT_PREPARSE;
		}
	}

	if ((lookFlag && player_said(" ")) ||
			(enterFlag && player_said("GRAVEYARD")) ||
			(enterFlag && player_said("CASTLE GROUNDS"))) {
		_G(player).need_to_walk = false;
		_G(player).ready_to_walk = true;
		_G(player).waiting_for_walk = false;
	}
}

void Room403::parser() {
	bool lookFlag = player_said_any("look", "look at");
	bool talkFlag = player_said_any("talk", "talk to");
	bool takeFlag = player_said("take");
	bool enterFlag = player_said("enter");
	bool useFlag = player_said_any("push", "pull", "gear", "open", "close");

	if (player_said("conv403a")) {
		if (_G(kernel).trigger == 1) {
			conv403a1();
		} else {
			conv403a();
		}
	} else if (talkFlag && player_said("WOLF")) {
		player_set_commands_allowed(false);
		_val4 = -1;
		_val6 = 1000;
		_val7 = 1100;
		TRIGGER;
	} else if (enterFlag && player_said("CASTLE")) {
		switch (_G(kernel).trigger) {
		case -1:
			player_set_commands_allowed(false);
			disable_player_commands_and_fade_init(1);
			break;
		case 1:
			midi_stop();
			digi_stop(3);
			_G(game).setRoom(408);
			break;
		default:
			break;
		}
	} else if (lookFlag && player_said("grate")) {
		switch (_G(kernel).trigger) {
		case -1:
			player_set_commands_allowed(false);

			if (_G(flags)[V123]) {
				digi_play("403R06", 1, 255, 2);
			} else {
				digi_play("403R06", 1, 255, 1);
				_G(flags)[V123] = 1;
			}
			break;
		case 1:
			digi_play("403R06A", 1, 255, 2);
			break;
		case 2:
			player_set_commands_allowed(true);
			break;
		default:
			break;
		}
	} else if (lookFlag && player_said("WOLF")) {
		digi_play(_G(flags)[V111] > 0 ? "402R13" : "402R12", 1);
	} else if (lookFlag && player_said("wall")) {
		switch (_G(kernel).trigger) {
		case -1:
			player_set_commands_allowed(false);
			digi_play("403r09", 1, 255, 1);
			break;
		case 1:
			player_set_commands_allowed(true);
			digi_play("403r09a", 1);
			break;

		}
	} else if (lookFlag && player_said_any("tomb", "door")) {
		digi_play("403r05", 1);
		_G(flags)[V121] = 1;
	} else if (lookFlag && player_said("bell")) {
		digi_play("403r07", 1);
		_G(flags)[V122] = 1;
	} else if (lookFlag && _G(walker).ripley_said(SAID)) {
		// No implementation
	} else if (lookFlag && player_said("edger") && !inv_player_has("EDGER")) {
		digi_play("403r53", 1);
	} else if (lookFlag && player_said_any("STEP LADDER", "STEP LADDER ") &&
			inv_object_is_here("STEP LADDER")) {
		digi_play(_G(flags)[V131] == 403 ? "403R10" : "403R46", 1);
	} else if (useFlag && player_said("PLANK") && inv_object_is_here("PLANK")) {
		_val6 = 1020;
		_val7 = 1300;
		TRIGGER;
	} else if (useFlag && player_said("STEP LADDER") && inv_object_is_here("STEP LADDER")) {
		_val6 = 1010;
		_val7 = 1200;
		TRIGGER;
	} else if (player_said("POMERANIAN MARKS", "WOLF") &&
			inv_player_has("POMERANIAN MARKS")) {
		if (!_G(flags)[V115] || _G(flags)[V114]) {
			player_set_commands_allowed(false);
			_val4 = 200;
		} else {
			_G(flags)[V114] = 1;
			_G(flags)[V111]++;
			player_set_commands_allowed(false);
			_val4 = 210;
		}
	} else if (MONEY("US DOLLARS") || MONEY("CHINESE YUAN") ||
			MONEY("PERUVIAN INTI") || MONEY("SIKKIMESE RUPEE")) {
		if (!_G(flags)[V116]) {
			_G(flags)[V116] = 1;
			player_set_commands_allowed(false);
			_val4 = 220;
		}
	} else if (player_said("EDGER", "BELL") && inv_player_has("EDGER")) {
		edgerBell();
	} else if (player_said("EDGER", "URN")) {
		// No implementation
	} else if ((player_said("STEP LADDER", "TOMB") ||
			player_said("STEP LADDER", "STAIRS")) ||
			stepLadderTomb()) {
		// No implementation
	} else if (player_said("STEP LADDER", "WALL")) {
		digi_play("403R32", 1);
	} else if (player_said("STEP LADDER", "BELL")) {
		digi_play("403R31", 1);
	} else if (takeFlag && player_said("URN")) {
		digi_play("403R28", 1);
	} else if (takeFlag && player_said("BELL")) {
		digi_play("403R30", 1);
	} else if (takeFlag && player_said("PLANK") &&
			takePlank()) {
		// No implementation
	} else if (takeFlag && player_said("EDGER") && takeEdger()) {
		// No implementation
	} else if (takeFlag && player_said("STEP LADDER") && takeStepLadder()) {
		// No implementation
	} else if (takeFlag && player_said("STEP LADDER ")) {
		takeStepLadder_();
	} else if (takeFlag && player_said("EDGER") && inv_object_is_here("EDGER")) {
		digi_play("403R30", 1);
	} else if (takeFlag && (
		player_said_any(
			"broken headstone", "wall", "pommee cross",
			"grave plaque", "1ST MARBLE MONUMENT", "2ND MARBLE MONUMENT",
			"marble column", "small headstone") ||
		player_said_any(
			"marble headstone", "tombstone",
			"burial tablet", "small grave marker", "tall headstone",
			"granite headstone", "burial plaque", "celtic cross")
	)) {
		digi_play("403R33", 1);
	} else if (lookFlag && player_said(" ")) {
		switch (_G(kernel).trigger) {
		case -1:
			player_set_commands_allowed(false);
			digi_play("403r04", 1, 255, 2);
			break;
		case 2:
			player_set_commands_allowed(true);
			break;

		}
	} else if (player_said("journal") && (
		player_said_any(
			"broken headstone", "pommee cross", "grave plaque",
			"grave", "1ST MARBLE MONUMENT", "2ND MARBLE MONUMENT",
			"marble column", "small headstone") ||
		player_said_any(
			"marble headstone", "tombstone", "burial tablet",
			"small grave marker", "tall headstone", "granite headstone",
			"burial plaque", "celtic cross")
	)) {
		useJournal();
	} else {
		return;
	}

	_G(player).command_ready = false;
}
#undef MONEY
#undef TRIGGER

void Room403::conv403a() {
	const char *sound = conv_sound_to_play();
	int who = conv_whos_talking();
	int node = conv_current_node();
	int entry = conv_current_entry();

	if (sound) {
		if (who <= 0) {
			switch (node) {
			case 8:
				_sound1 = sound;

				if (inv_player_has("POMERANIAN MARKS")) {
					conv_resume();
				} else {
					_val9 = 2260;
				}
				break;

			case 10:
				_sound1 = sound;
				_val9 = 2270;
				_G(kernel).trigger_mode = KT_DAEMON;
				kernel_timing_trigger(1, 110);
				_G(kernel).trigger_mode = KT_PARSE;
				break;

			case 11:
				if (entry == 0) {
					_sound1 = sound;
					_val9 = 2253;
					return;
				} else {
					_val9 = 2101;
					digi_play(sound, 1, 255, 1);
				}
				break;

			case 17:
				if (entry == 1) {
					_val5 = 1;
					midi_play("DANZIG1", 255, 1, -1, 949);
				}

				_val9 = 2250;
				digi_play(sound, 1, 255, 1);
				break;

			case 19:
			case 20:
			case 21:
			case 22:
			case 23:
			case 24:
			case 28:
			case 29:
			case 30:
			case 31:
			case 32:
			case 33:
				_flag2 = true;
				_val9 = 2101;
				digi_play(sound, 1, 255, 1);
				break;

			default:
				_val9 = 2101;
				digi_play(sound, 1, 255, 1);
				break;
			}
		} else if (who == 1) {
			_val7 = 1102;
			digi_play(sound, 1, 255, 1);
		}
	} else {
		conv_resume();
	}
}

void Room403::conv403a1() {
	int who = conv_whos_talking();

	if (who <= 0) {
		_val9 = (_val9 == 2250) ? 2252 : 2102;
	} else if (who == 1) {
		_val7 = 1103;
	}

	conv_resume();
}

void Room403::edgerBell() {
	switch (_G(kernel).trigger) {
	case -1:
		player_set_commands_allowed(false);
		if (++_G(flags)[V119] < 8) {
			_ripRingsBell = series_load("RIP RINGS BELL");
			player_update_info();
			ws_hide_walker();
			_ripOnLadder = TriggerMachineByHash(1, 1, 0, 0, 0, 0, 0, 0, 100, 0x100, 0,
				triggerMachineByHashCallbackNegative, "RIP rings bell");

			terminateMachineAndNull(_bell);
			sendWSMessage_10000(1, _ripOnLadder, _ripRingsBell, 1, 19, 1,
				_ripRingsBell, 19, 19, 0);
		}
		break;

	case 1:
		digi_play("403_s12", 2);
		sendWSMessage_10000(1, _ripOnLadder, _ripRingsBell, 19, 19, 2,
			_ripRingsBell, 19, 32, 0);
		break;

	case 2:
		sendWSMessage_10000(1, _ripOnLadder, _ripRingsBell, 32, 1, 3,
			_ripRingsBell, 1, 1, 0);
		break;

	case 3:
		terminateMachineAndNull(_ripOnLadder);
		ws_unhide_walker();
		_bell = series_place_sprite("ONE FRAME BELL", 0, 0, 0, 100, 0xf00);
		series_unload(_ripRingsBell);

		if (_G(flags)[V119] >= 7) {
			ws_walk_load_shadow_series(S4_SHADOW_DIRS, S4_SHADOW_NAMES);
			ws_walk_load_walker_series(S4_NORMAL_DIRS, S4_NORMAL_NAMES);
			_wolfWalker = triggerMachineByHash_3000(8, 8, S4_NORMAL_DIRS, S4_SHADOW_DIRS, 620, 323, 3,
				triggerMachineByHashCallback3000, "wolf_walker");
			_wolfAdmonish = series_load("WOLF ADMONISHES RIP");
			kernel_timing_trigger(120, 4);
		} else {
			player_set_commands_allowed(true);
		}
		break;

	case 4:
		sendWSMessage_10000(_wolfWalker, 687, 323, 3, 5, 1);
		break;

	case 5:
		sendWSMessage_60000(_wolfWalker);
		_wolfie = TriggerMachineByHash(1, 1, 0, 0, 0, 0, 0, 0, 100, 0x100, 0,
			triggerMachineByHashCallbackNegative, "WOLFIE");
		sendWSMessage_10000(1, _wolfie, _wolfAdmonish, 1, 18, 6,
			_wolfAdmonish, 18, 18, 0);

		_ripHeadTurn = series_load("RIP TREK HEAD TURN POS1");
		setGlobals1(_ripHeadTurn, 4, 8, 8, 8, 0, 8, 4, 4, 4);
		sendWSMessage_110000(-1);
		break;

	case 6:
		sendWSMessage_10000(1, _wolfie, _wolfAdmonish, 19, 31, 7,
			_wolfAdmonish, 31, 31, 0);
		playNum1(_G(flags)[V119]);
		break;

	case 7:
		sendWSMessage_10000(1, _wolfie, _wolfAdmonish, 31, 1, -1,
			_wolfAdmonish, 1, 1, 0);
		break;

	case 8:
		if (_G(flags)[V119] == 1) {
			digi_play("403r47", 1, 255, 9);
		} else {
			kernel_timing_trigger(60, 9);
		}
		break;

	case 9:
		terminateMachineAndNull(_wolfie);
		_wolfWalker = triggerMachineByHash_3000(8, 8,
			S4_NORMAL_DIRS, S4_SHADOW_DIRS, 687, 323, 3,
			triggerMachineByHashCallback3000, "wolf_walker");
		sendWSMessage_10000(_wolfWalker, 620, 323, 3, 10, 0);
		playNum2(_G(flags)[V119]);
		break;

	case 10:
		sendWSMessage_60000(_wolfWalker);
		sendWSMessage_120000(11);
		break;

	case 11:
		series_unload(_wolfAdmonish);
		series_unload(S4_NORMAL_DIRS[1]);
		series_unload(S4_NORMAL_DIRS[0]);
		series_unload(S4_SHADOW_DIRS[1]);
		series_unload(S4_SHADOW_DIRS[0]);
		series_unload(_ripHeadTurn);
		sendWSMessage_150000(-1);
		player_set_commands_allowed(true);
		break;

	default:
		break;
	}
}

bool Room403::edgerUrn() {
	switch (_G(kernel).trigger) {
	case 1:
		if (inv_player_has("EDGER")) {
			player_set_commands_allowed(false);
			_ripMedReach = series_load("RIP MED REACH 1HAND POS2");
			setGlobals1(_ripMedReach, 1, 17, 17, 17, 0, 17, 1, 1, 1);
			sendWSMessage_110000(2);
			return true;
		}
		return false;

	case 2:
		_edger = series_place_sprite("ONE FRAME EDGER", 0, 0, 0, 100, 0xf00);
		hotspot_set_active("EDGER", true);
		inv_move_object("EDGER", 403);
		sendWSMessage_120000(3);
		return true;

	case 3:
		sendWSMessage_150000(4);
		return true;

	case 4:
		series_unload(_ripMedReach);
		player_set_commands_allowed(true);
		return true;

	case 69:
		if (inv_player_has("EDGER")) {
			player_set_commands_allowed(false);
			ws_walk(1201, 321, 0, 1, 2);
			return true;
		}
		return false;

	default:
		return true;
	}
}

bool Room403::stepLadderTomb() {
	switch (_G(kernel).trigger) {
	case -1:
		if (inv_player_has("STEP LADDER")) {
			player_set_commands_allowed(false);
			_series1 = series_load("RIP TREK MED REACH HAND POS1");
			setGlobals1(_series1, 1, 10, 10, 10, 0, 10, 1, 1, 1);
			sendWSMessage_110000(1);
			return true;
		}
		return false;

	case 1:
		digi_play("403_s04", 2);
		_ladder = series_place_sprite("1 sprite of ladder", 0, 0, 0, 100, 0xf00);
		hotspot_set_active("STEP LADDER", true);
		inv_move_object("STEP LADDER", 403);
		_G(flags)[V313] = 2;
		sendWSMessage_120000(3);
		return true;

	case 3:
		sendWSMessage_150000(4);
		return true;

	case 4:
		series_unload(_series1);
		player_set_commands_allowed(true);
		return true;

	default:
		break;
	}

	return false;
}

bool Room403::takePlank() {
	switch (_G(kernel).trigger) {
	case -1:
		if (inv_object_is_here("PLANK")) {
			_ripPutBoard = series_load("RIPLEY PUTS BOARD ON POTS");
			terminateMachineAndNull(_board);
			ws_hide_walker();

			_ripOnLadder = TriggerMachineByHash(1, 1, 0, 0, 0, 0, 0, 0, 100, 0x300, 0,
				triggerMachineByHashCallbackNegative, "RIP plants plank");
			sendWSMessage_10000(1, _ripOnLadder, _ripPutBoard, 57, 1, 2,
				_ripPutBoard, 1, 1, 0);
			return true;
		}
		return false;

	case 2:
		hotspot_set_active("PLANK", false);
		inv_give_to_player("PLANK");
		_plank = 0;
		kernel_examine_inventory_object("PING PLANK", _G(master_palette),
			5, 1, 396, 223, 3, nullptr, -1);
		return true;

	case 3:
		terminateMachineAndNull(_ripOnLadder);
		ws_unhide_walker();
		series_unload(_ripPutBoard);
		player_set_commands_allowed(true);
		return true;

	default:
		break;
	}

	return false;
}

bool Room403::takeEdger() {
	switch (_G(kernel).trigger) {
	case -1:
		if (!inv_player_has("EDGER")) {
			player_set_commands_allowed(false);
			_ripMedReach = series_load("RIP MED REACH 1HAND POS2");
			setGlobals1(_ripMedReach, 1, 17, 17, 17, 0, 17, 1, 1, 1);
			sendWSMessage_110000(1);
			return true;
		}
		return false;

	case 1:
		terminateMachineAndNull(_edger);
		hotspot_set_active("EDGER", false);
		inv_give_to_player("EDGER");
		kernel_examine_inventory_object("PING EDGER", _G(master_palette),
			5, 1, 500, 216, 2, 0, -1);
		return true;

	case 2:
		sendWSMessage_120000(3);
		return true;

	case 3:
		sendWSMessage_150000(4);
		return true;

	case 4:
		series_unload(_ripMedReach);
		player_set_commands_allowed(true);
		return true;

	case 5:
		player_set_commands_allowed(true);
		return true;

	default:
		break;
	}

	return false;
}

bool Room403::takeStepLadder() {
	switch (_G(kernel).trigger) {
	case -1:
		if (inv_object_is_here("STEP LADDER")) {
			player_set_commands_allowed(false);
			_series1 = series_load("RIP TREK MED REACH HAND POS1");
			setGlobals1(_series1, 1, 10, 10, 10, 0, 10, 1, 1, 1);
			sendWSMessage_110000(1);
			return true;
		}
		return false;

	case 1:
		if (_G(flags)[V131] == 403) {
			digi_play("403w08", 1, 255, 3);
			_val8 = 2000;
			_val9 = 2100;
			kernel_timing_trigger(15, 2);
		} else {
			digi_play("403_s03", 2);
			terminateMachineAndNull(_ladder);
			hotspot_set_active("STEP LADDER", false);
			inv_give_to_player("STEP LADDER");
			_G(flags)[V313] = 0;
			kernel_examine_inventory_object("PING STEP LADDER", _G(master_palette),
				5, 1, 429, 215, 2, nullptr, -1);

		}
		return true;

	case 2:
		sendWSMessage_120000(4);
		return true;

	case 3:
		_val9 = 2105;
		digi_play("403r47", 1);
		return true;

	case 4:
		sendWSMessage_150000(5);
		return true;

	case 5:
		series_unload(_series1);
		player_set_commands_allowed(true);
		return true;

	default:
		break;
	}

	return false;
}

void Room403::takeStepLadder_() {
	switch (_G(kernel).trigger) {
	case -1:
		if (inv_object_is_here("STEP LADDER")) {
			player_set_commands_allowed(false);
			_series1 = series_load("RIP TREK MED REACH HAND POS1");
			setGlobals1(_series1, 1, 10, 10, 10, 0, 10, 1, 1, 1);
			sendWSMessage_110000(1);
		}
		break;

	case 1:
		if (_G(flags)[V131] == 403) {
			digi_play("403w08", 1, 255, 3);
			_val8 = 2000;
			_val9 = 2100;
			kernel_timing_trigger(15, 2);
		} else {
			digi_play("403_s03", 2);
			hotspot_set_active("STEP LADDER ", false);
			inv_give_to_player("STEP LADDER");
			_G(flags)[V313] = 0;
			kernel_examine_inventory_object("PING STEP LADDER",
				_G(master_palette), 5, 1, 496, 226, 2, nullptr, -1);
			terminateMachineAndNull(_ladder);
		}
		break;

	case 2:
		sendWSMessage_120000(4);
		break;

	case 3:
		_val9 = 2105;
		digi_play("403r47", 1);
		break;

	case 4:
		sendWSMessage_150000(5);
		break;

	case 5:
		series_unload(_series1);
		player_set_commands_allowed(true);
		break;

	case 6:
		player_set_commands_allowed(true);
		break;

	default:
		break;
	}
}

void Room403::useJournal() {
	if (_G(flags)[kGraveyardCartoon]) {
		digi_play("403r42", 1);
	} else if (_G(flags)[kEpitaphCartoon]) {
		if (_G(kernel).trigger == 6)
			_G(flags)[kGraveyardCartoon] = 1;
		sendWSMessage_multi(nullptr);
	} else {
		if (_G(kernel).trigger == 6)
			_G(flags)[kGraveyardCartoon] = 1;
		sendWSMessage_multi("403r41");
	}
}

void Room403::playNum1(int num) {
	static const char *const NAMES[] = {
		"403w11", "403w13", "403w15", "40ew16",
		"403w17", "403w18", "40w1", "40w1"
	};
	digi_play(NAMES[num - 1], 1, 255, 8);
}

void Room403::playNum2(int num) {
	switch (num) {
	case 1:
		digi_play("403w12", 1);
		break;
	case 2:
		digi_play("403w14", 1);
		break;
	default:
		break;
	}
}

} // namespace Rooms
} // namespace Riddle
} // namespace M4
