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

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * $URL$
 * $Id$
 *
 */

#ifndef ASYLUM_WORLDSTATS_H
#define ASYLUM_WORLDSTATS_H

#include "asylum/system/sound.h"

#include "common/rect.h"
#include "common/array.h"

namespace Asylum {

#define ACTORS_MAX_COUNT   50
#define OBJECTS_MAX_COUNT 400
#define ACTIONS_MAX_COUNT 400

#define ACTORS_SIZE       2468
#define ACTORDATA_SIZE    1448
#define OBJECTS_SIZE      1704
#define ACTIONS_SIZE      180

class Actor;
class Object;
class AsylumEngine;

struct ActionArea;

class WorldStats {
public:
	WorldStats(AsylumEngine *engine);
	virtual ~WorldStats();

	/**
	 * Loads the data
	 *
	 * @param stream If non-null, the Common::SeekableReadStream * to load from
	 */
	void load(Common::SeekableReadStream *stream);

	int32 size;
	int32 numEntries;
	ChapterIndex chapter;
	int32 xLeft; // scene start x position
	int32 yTop;  // scene start y position
	Common::Rect boundingRect;
	ResourceId backgroundImage;
	ResourceId curScrollUp;
	ResourceId curScrollUpLeft;
	ResourceId curScrollLeft;
	ResourceId curScrollDownLeft;
	ResourceId curScrollDown;
	ResourceId curScrollDownRight;
	ResourceId curScrollRight;
	ResourceId curScrollUpRight;
	ResourceId curHand;
	ResourceId curMagnifyingGlass;
	ResourceId curTalkNPC;
	ResourceId curGrabPointer;
	ResourceId curTalkNPC2;
	ResourceId font1;
	ResourceId font2;
	ResourceId font3;
	int32 currentPaletteId;
	int32 cellShadeMask1;
	int32 cellShadeMask2;
	int32 cellShadeMask3;
	int32 unused;
	int32 smallCurUp;
	int32 smallCurDown;
	int32 encounterFrameBg;
	int32 width; // field_80
	int32 height;
	int32 motionStatus;
	int32 field_8C;
	//uint32 numActions;
	//uint32 numObjects;
	int32 coordinates[7];
	//uint32 numActors;
	int32 reverseStereo;
	Common::Rect sceneRects[6]; // including scene size rect
	uint8 sceneRectIdx;
	uint8 field_11D[3];
	int32 field_120;
	int32 scriptIndex;	 // actionList start index
	ResourceId graphicResourceIds[100];
	ResourceId sceneTitleGraphicResourceId;
	ResourceId sceneTitlePaletteResourceId;
	int32 actorType;
	ResourceId soundResourceIds[50];
	AmbientSoundItem ambientSounds[15];
	int32 numAmbientSound;
	int32 musicStatus;
	uint32 musicCurrentResourceIndex;
	int32 musicFlag;
	ResourceId musicResourceId;
	int32 musicStatusExt;
	Common::Array<Object*>     objects;   // maxsize 400
	Common::Array<Actor*>      actors;   // maxsize 50
	// ActorData is stored in each actor instance
	int32 numActionLists;
	int32 numPolygons;
	Common::Array<ActionArea*> actions;  // maxsize 400
	// TODO Add missing fields
	int32 field_E848C;
	int32 field_E8490;
	int32 field_E8494;
	int32 field_E8498;
	int32 field_E849C;
	int32 tickValueArray[30];
	int32 field_E8518;
	int32 field_E851C[30];
	int32 field_E8594[30];
	ActorIndex nextPlayer;
	uint32 field_E8610[6];
	uint32 field_E8628[6];
	Object *wheels[7];
	int32 tickCount1;
	uint32 field_E8660[6];

	void setWheelObjects();

	int32 getActionAreaIndexById(int32 id);
	int32 getRandomActionAreaIndexById(int32 id);
	ActionArea* getActionAreaById(int32 id);

	Object* getObjectById(ObjectId id);

	Common::String toString();

private:
	AsylumEngine *_vm;
};

} // end of namespace Asylum

#endif
