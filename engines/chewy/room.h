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

#ifndef CHEWY_RAUM_H
#define CHEWY_RAUM_H

#include "chewy/objekt.h"

namespace Chewy {

#define MAX_ABLAGE 4

#define ABLAGE_BLOCK_SIZE 64000l
#define GED_BLOCK_SIZE 3000l
#define MAX_ROOM_HANDLE 4
#define GED_LOAD 1

#define R_TGPDATEI 0
#define R_VOCDATEI 1
#define R_GEPDATEI 2
#define R_SPEZTAF 3

struct RaumBlk {
	byte *LowPalMem;
	const char *InvFile;
	const char *DetFile;
	byte **InvSprAdr;

	RoomMovObjekt *Rmo;

	RoomStaticInventar *Rsi;

	char RoomDir[9];
	int16 AkAblage;
	byte **DetImage;
	int16 *DetKorrekt;
	taf_info *Fti;
	int16 AadLoad;
	int16 AtsLoad;
};

struct RaumTimer {
	int16 TimerStart;
	int16 TimerAnz;

	uint8 ObjNr[MAX_ROOM_TIMER];

	uint8 TimerNr[MAX_ROOM_TIMER];
};

class Room {
public:
	Room();
	~Room();

	// FIXME: was FILE*
	void *open_handle(const char *fname, const char *fmode, int16 mode);

	void close_handle(int16 mode);

	void load_room(RaumBlk *Rb, int16 room_nr, Spieler *player);

	void load_taf(const char *filename, byte **speicher, byte *palette, int16 sprnr);

	int16 load_tgp(int16 nr, RaumBlk *Rb, int16 tgp_idx, int16 mode);
	void load_sound();
	// FIXME: was FILE*
	void *get_sound_handle();

	byte *get_ablage(int16 nr);

	byte **get_ablage();

	byte **get_ged_mem();

	void set_timer_start(int16 timer_start);

	void add_timer_new_room();

	void del_timer_old_room();

	int16 set_timer(int16 ani_nr, int16 timer_end);

	void set_timer_status(int16 ani_nr, int16 status);

	void set_zoom(int16 zoom);

	void set_pal(const byte *src_pal, byte *dest_pal);

	void set_ak_pal(RaumBlk *Rb);

	void calc_invent(RaumBlk *Rb, Spieler *player);

	RaumTimer room_timer;
	RaumInfo *room_info;
	GedChunkHeader GedInfo[MAX_ABLAGE];
	int16 GedXAnz[MAX_ABLAGE];

	int16 GedYAnz[MAX_ABLAGE];

private:

	// FIXME: was FILE*
	void load_ged_chunk(RaumBlk *Rb, void *handle, int16 nr,
	                    char *speicher);

	void init_ablage();

	void free_ablage();

	int16 get_ablage(int16 pic_nr, uint32 pic_size);

	int16 get_ablage_g1(int16 ablage_bedarf, int16 ak_pos);
	void set_ablage_info(int16 ablagenr, int16 bildnr, uint32 pic_size);

	void load_room_music(int16 room_nr);

	GedPoolHeader ged_pool_header;
	int16 AkAblage;
	int16 LastAblageSave;

	byte *Ablage[MAX_ABLAGE];
	byte *AblagePal[MAX_ABLAGE];
	int16 AblageInfo[MAX_ABLAGE][2];

	byte *GedMem[MAX_ABLAGE];
	Stream *roomhandle[MAX_ROOM_HANDLE];
};

} // namespace Chewy

#endif
