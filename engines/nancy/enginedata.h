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

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef NANCY_ENGINEDATA_H
#define NANCY_ENGINEDATA_H

#include "engines/nancy/commontypes.h"

namespace Nancy {

// Data types corresponding to chunks found inside BOOT

struct BSUM {
	BSUM(Common::SeekableReadStream *chunkStream);

	byte header[90];

	// Game start section
	SceneChangeDescription firstScene;
	uint16 startTimeHours;
	uint16 startTimeMinutes;

	// UI
	Common::Rect mapButtonHotspot;
	Common::Rect textboxScreenPosition;
	Common::Rect menuButtonSrc;
	Common::Rect helpButtonSrc;
	Common::Rect menuButtonDest;
	Common::Rect helpButtonDest;

	uint16 horizontalEdgesSize;
	uint16 verticalEdgesSize;

	uint16 playerTimeMinuteLength;
	byte overrideMovementTimeDeltas;
	uint16 slowMovementTimeDelta;
	uint16 fastMovementTimeDelta;
};

struct VIEW {
	VIEW(Common::SeekableReadStream *chunkStream);

	Common::Rect screenPosition;
	Common::Rect bounds;
};

struct INV {
	struct ItemDescription {
		Common::String name;
		byte keepItem;
		Common::Rect sourceRect;
	};

	INV(Common::SeekableReadStream *chunkStream);

	Common::Rect scrollbarSrcBounds;
	Common::Point scrollbarDefaultPos;
	uint16 scrollbarMaxScroll;

	Common::Array<Common::Rect> curtainAnimationSrcs;
	Common::Rect inventoryScreenPosition;
	uint16 curtainsFrameTime;

	Common::String inventoryBoxIconsImageName;
	Common::String inventoryCursorsImageName;

	Common::Array<ItemDescription> itemDescriptions;
};

struct TBOX {
	TBOX(Common::SeekableReadStream *chunkStream);

	Common::Rect scrollbarSrcBounds;
	Common::Rect innerBoundingBox;
	Common::Point scrollbarDefaultPos;
	uint16 scrollbarMaxScroll;

	uint16 firstLineOffset;
	uint16 lineHeight;
	uint16 borderWidth;
	uint16 maxWidthDifference;

	Common::Array<Common::Rect> ornamentSrcs;
	Common::Array<Common::Rect> ornamentDests;

	uint16 fontID;
};

} // End of namespace Nancy

#endif // NANCY_ENGINEDATA_H
