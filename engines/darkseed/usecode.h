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

#ifndef DARKSEED_USECODE_H
#define DARKSEED_USECODE_H

#include "common/scummsys.h"
#include "console.h"
#include "inventory.h"
#include "objects.h"
#include "player.h"

namespace Darkseed {
class UseCode {
private:
	Console *_console = nullptr;
	Player *_player = nullptr;
	Objects &_objectVar;
	Inventory &_inventory;

public:
	UseCode(Console *console, Player *player, Objects &objectVar, Inventory &inventory);

public:
	void useCode(int objNum);
	void useCodeGloves(int16 targetObjNum);
	void useCodeMoney(int16 targetObjNum);
	void useCodeJournal(int16 actionObjNum, int16 targetObjNum);
	void useCodeShopItems(int16 actionObjNum, int16 targetObjNum);
	void useCrowBar(int16 targetObjNum);
	void useCodeNewspaper(int16 targetObjNum);
	void useCodeLibraryCard(int16 targetObjNum);
	void useCodeBobbyPin(int16 targetObjNum);
	void useCodeKeys(int16 actionObjNum, int16 targetObjNum);
	void useCodeMirrorShard(int16 targetObjNum);
	void useCodeBinoculars(int16 targetObjNum);
	void useCodeShovel(int16 targetObjNum);
	void useCodeDelbertsCard(int16 targetObjNum);
	void useCodeStick(int16 targetObjNum);

private:
	int getHandDescriptionTosIdx(uint16 objNum);
	int16 getUseGlovesTosIdx(uint16 objNum);
	int16 getUseMoneyTosIdx(uint16 objNum);
	int16 getUseCrowbarTosIdx(uint16 objNum);
	int16 getUseJournalTosIdx(uint16 objNum);
	int16 getUseLibraryCardTosIdx(uint16 objNum);
	int16 getUseBobbyPinTosIdx(uint16 objNum);
	int16 getUseKeysTosIdx(uint16 objNum);
	int16 getUseBinocularsTosIdx(uint16 objNum);
	int16 getUseShovelTosIdx(uint16 objNum);
	int16 getUseDelbertsCardTosIdx(uint16 objNum);
	int16 getUseStickTosIdx(uint16 objNum);

	void gancanim();
	void putobjunderpillow(int objNum);
	void genericresponse(int16 useObjNum, int16 targetObjNum, int16 tosIdx);
	void startdigging(int16 targetObjNum);
};
} // namespace Darkseed

#endif // DARKSEED_USECODE_H
