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

#ifndef ULTIMA8_USECODE_REGRETINTRINSICS_H
#define ULTIMA8_USECODE_REGRETINTRINSICS_H

#include "ultima/ultima8/usecode/intrinsics.h"
#include "ultima/ultima8/world/item.h"

namespace Ultima {
namespace Ultima8 {

// Crusader: No Regret Intrinsics
Intrinsic RegretIntrinsics[] = {
	World::I_getAlertActive, // Intrinsic000()
	Item::I_getFrame, // Intrinsic001()
	Item::I_setFrame, // Intrinsic002()
	Item::I_getMapArray, // Intrinsic003()
	Item::I_getStatus, // Intrinsic004()
	Item::I_orStatus, // Intrinsic005()
	Item::I_equip, // Intrinsic006()
	Item::I_isPartlyOnScreen, // Intrinsic007()
	Actor::I_isNPC, // Intrinsic008()
	Item::I_getZ, // Intrinsic009()
	World::I_gameDifficulty, // Intrinsic00A()
	Item::I_getQLo, // Intrinsic00B()
	Item::I_destroy, // Intrinsic00C()
	Actor::I_getUnkByte, // Intrinsic00D()
	Item::I_getX, // Intrinsic00E()
	Item::I_getY, // Intrinsic00F()
	// 0010
	AudioProcess::I_playSFXCru, // Intrinsic010()
	Item::I_getShape, // Intrinsic011()
	Item::I_explode, // Intrinsic012()
	UCMachine::I_rndRange, // Intrinsic013()
	Item::I_legalCreateAtCoords, // Intrinsic014()
	Item::I_andStatus, // Intrinsic015()
	World::I_getControlledNPCNum, // Intrinsic016()
	Actor::I_getDir, // Intrinsic017()
	Actor::I_getLastAnimSet, // Intrinsic018()
	Item::I_fireWeapon, // Intrinsic019()
	Item::I_create, // Intrinsic01A()
	Item::I_popToCoords, // Intrinsic01B()
	Actor::I_setDead, // Intrinsic01C()
	Item::I_push, // Intrinsic01D()
	Item::I_getEtherealTop, // Intrinsic01E()
	Item::I_getQLo, // Intrinsic01F()
	// 0020
	Item::I_setQLo, // Intrinsic020()
	Item::I_getQHi, // Intrinsic021()
	Item::I_setQHi, // Intrinsic022()
	Item::I_getClosestDirectionInRange, // Intrinsic023()
	Item::I_hurl, // Intrinsic024()
	Item::I_getCY, // Intrinsic025()
	Item::I_getCX, // Intrinsic026()
	SpriteProcess::I_createSprite, // Intrinsic027()
	Item::I_setNpcNum, // Intrinsic028()
	AudioProcess::I_playSFXCru, // Intrinsic029() TODO: Not exactly the same.
	Item::I_setShape, // Intrinsic02A()
	Item::I_pop, // Intrinsic02B()
	AudioProcess::I_stopSFXCru, // Intrinsic02C(), I_stopAllSFXForItem
	Item::I_isCompletelyOn, // Intrinsic02D()
	Item::I_popToContainer, // Intrinsic02E()
	Actor::I_getHp, // Intrinsic02F()
	// 0030
	MainActor::I_getMana, // Intrinsic030()
	Item::I_getFamily, // Intrinsic031()
	Actor::I_destroyContents, // Intrinsic032()
	0, // Intrinsic033() Item::I_SetVolumeForItemSFX
	Item::I_getDirToItem, // Intrinsic034()
	0, // Intrinsic035()
	Item::I_getRangeIfVisible, // Intrinsic036()
	AudioProcess::I_playSFXCru, // Intrinsic037() TODO: Not exactly the same.
	Item::I_andStatus, // Intrinsic038()
	Kernel::I_resetRef, // Intrinsic039()
	Item::I_touch, // Intrinsic03A()
	Egg::I_getEggId, // Intrinsic03B()
	MainActor::I_addItemCru, // Intrinsic03C()
	Actor::I_getMap, // Intrinsic03D()
	Item::I_cast, // Intrinsic03E()
	0, // Intrinsic03F()
	// 0040
	AudioProcess::I_stopSFXCru, // Intrinsic040()
	Item::I_isOn, // Intrinsic041()
	Item::I_getQHi, // Intrinsic042()
	Item::I_isOn, // Intrinsic043()
	Item::I_getQHi, // Intrinsic044()
	Item::I_isOn, // Intrinsic045()
	Item::I_getQHi, // Intrinsic046()
	Item::I_isOn, // Intrinsic047()
	Item::I_getQHi, // Intrinsic048()
	Item::I_isOn, // Intrinsic049()
	Item::I_getQHi, // Intrinsic04A()
	Item::I_isOn, // Intrinsic04B()
	Item::I_getQHi, // Intrinsic04C()
	Actor::I_getCurrentActivityNo, // Intrinsic04D()
	Actor::I_isDead, // Intrinsic04E()
	Actor::I_clrInCombat, // Intrinsic04F()
	// 0050
	Actor::I_setDefaultActivity0, // Intrinsic050()
	Actor::I_setDefaultActivity1, // Intrinsic051()
	Actor::I_setDefaultActivity2, // Intrinsic052()
	Actor::I_setActivity, // Intrinsic053()
	World::I_setControlledNPCNum, // Intrinsic054()
	Item::I_receiveHit, // Intrinsic055()
	UCMachine::I_true, // Actually Game::I_isReleaseBuild
	MainActor::I_setMana, // Intrinsic057()
	Item::I_use, // Intrinsic058()
	Item::I_setUnkEggType, // Intrinsic059()
	MusicProcess::I_playMusic, // Intrinsic05A()
	Item::I_getSurfaceWeight, // Intrinsic05B()
	Item::I_isCentreOn, // Intrinsic05C()
	Item::I_setFrame, // Intrinsic05D()
	Actor::I_getLastAnimSet, // Intrinsic05E()
	Ultima8Engine::I_setAvatarInStasis, // Intrinsic05F()
	// 0060
	Actor::I_isBusy, // Intrinsic060()
	0, // Actor::I_getField0x13Flag2()
	Actor::I_doAnim, // Intrinsic062()
	Item::I_legalCreateAtPoint, // Intrinsic063()
	Item::I_getPoint, // Intrinsic064()
	Item::I_legalMoveToPoint, // Intrinsic065()
	Item::I_fall, // Intrinsic066()
	Item::I_hurl, // Intrinsic067()
	Kernel::I_getNumProcesses, // Intrinsic068()
	Item::I_getCY, // Intrinsic069()
	0, // Intrinsic06A() I_isAnimDisabled
	0, // Intrinsic06B()
	MusicProcess::I_pauseMusic, // Intrinsic06C()
	MovieGump::I_playMovieCutsceneRegret, // Intrinsic06D()
	MusicProcess::I_unpauseMusic, // Intrinsic06E()
	Item::I_isInNpc, // Intrinsic06F()
	// 0070
	Ultima8Engine::I_setCruStasis, // Intrinsic070()
	Ultima8Engine::I_clrCruStasis, // Intrinsic071()
	0, // Intrinsic072() PaletteFaderProcess::I_jumpToColor
	PaletteFaderProcess::I_fadeFromBlack, // Intrinsic073()
	Actor::I_isDead, // Intrinsic074()
	Actor::I_getNpcNum, // Intrinsic075()
	0, // Intrinsic076() - null intrinsic
	UCMachine::I_true, // Actually Game::I_isViolenceEnabled
	Item::I_unequip, // Intrinsic078()
	Item::I_andStatus, // Intrinsic079()
	Item::I_move, // Intrinsic07A()
	Ultima8Engine::I_getUnkCrusaderFlag, // Intrinsic07B()
	Ultima8Engine::I_setUnkCrusaderFlag, // Intrinsic07C()
	Ultima8Engine::I_clrUnkCrusaderFlag, // Intrinsic07D()
	Actor::I_turnToward, // Intrinsic07E()
	PaletteFaderProcess::I_fadeToBlack, // Intrinsic07F()
	// 0080
	MainActor::I_clrKeycards, // Intrinsic080()
	MusicProcess::I_stopMusic, // Intrinsic081()
	PaletteFaderProcess::I_jumpToAllBlack, // Intrinsic082()
	0, // I_setUnkFlagA4()
	0, // I_clearUnkFlagA4()
	0, // Intrinsic085()
	MainActor::I_teleportToEgg, // Intrinsic086()
	PaletteFaderProcess::I_fadeFromBlack, // Intrinsic087()
	Actor::I_clrImmortal, // Intrinsic088()
	Actor::I_setActivity, // Intrinsic089()
	Item::I_getQuality, // Intrinsic08A()
	Item::I_setQuality, // Intrinsic08B()
	MainActor::I_getMaxEnergy, // Intrinsic08C()
	CameraProcess::I_moveTo, // Intrinsic08D()
	Actor::I_setImmortal, // Intrinsic08E()
	CameraProcess::I_getCameraX, // Intrinsic08F()
	// 0090
	CameraProcess::I_getCameraY, // Intrinsic090()
	Item::I_setMapArray, // Intrinsic091()
	Actor::I_getNpcNum, // Intrinsic092()
	Item::I_shoot, // Intrinsic093()
	CameraProcess::I_setCenterOn, // Intrinsic094()
	Item::I_enterFastArea, // Intrinsic095()
	Item::I_setBroken, // Intrinsic096()
	Item::I_hurl, // Intrinsic097()
	Actor::I_getNpcNum, // Intrinsic098()
	Ultima8Engine::I_moveKeyDownRecently, // Intrinsic099()
	MainActor::I_teleportToEgg, // Intrinsic09A()
	Actor::I_createActor, // Intrinsic09B()
	Actor::I_clrInCombat, // Intrinsic09C()
	PaletteFaderProcess::I_jumpToGreyScale, // Intrinsic09D()
	PaletteFaderProcess::I_jumpToNormalPalette, // Intrinsic09E()
	CruStatusGump::I_showStatusGump, // Intrinsic09F(), same as Int05F in Remorse
	// 00A0
	Item::I_andStatus, // Intrinsic0A0()
	Egg::I_getUnkEggType, // Intrinsic0A1()
	Egg::I_setEggXRange, // Intrinsic0A2()
	Item::I_setFrame, // Intrinsic0A3()
	Item::I_overlaps, // Intrinsic0A4()
	Item::I_isOn, // Intrinsic0A5()
	Item::I_getQHi, // Intrinsic0A6()
	Actor::I_getLastAnimSet, // Intrinsic0A7()
	Item::I_getCY, // Intrinsic0A8()
	CurrentMap::I_canExistAt, // Intrinsic0A9()
	Item::I_isOn, // Intrinsic0AA()
	Actor::I_isDead, // Intrinsic0AB()
	Item::I_hurl, // Intrinsic0AC()
	Item::I_inFastArea, // Intrinsic0AD()
	Item::I_getQHi, // Intrinsic0AE()
	Item::I_andStatus, // Intrinsic0AF()
	// 00B0
	Item::I_hurl, // Intrinsic0B0()
	Item::I_andStatus, // Intrinsic0B1()
	Item::I_hurl, // Intrinsic0B2()
	Item::I_andStatus, // Intrinsic0B3()
	Item::I_getDirToCoords, // Intrinsic0B4()
	0, // MainActor::I_addItemSimple()
	0, // I_updateInventoryUI() - probably not needed?
	Actor::I_getNpcNum, // Intrinsic0B7()
	Item::I_getCY, // Intrinsic0B8()
	Item::I_isOn, // Intrinsic0B9()
	Item::I_getFootpadData, // Intrinsic0BA()
	Actor::I_isDead, // Intrinsic0BB()
	Actor::I_createActorCru, // Intrinsic0BC()
	Actor::I_setActivity, // Intrinsic0BD()
	KeypadGump::I_showKeypad, // Intrinsic0BE()
	Item::I_andStatus, // Intrinsic0BF()
	// 00C0
	ComputerGump::I_readComputer, // Intrinsic0C0()
	UCMachine::I_numToStr, // Intrinsic0C1()
	0, // Intrinsic0C2() - return 0
	Actor::I_getDir, // Intrinsic0C3()
	Item::I_getQHi, // Intrinsic0C4()
	Item::I_setQuality, // Intrinsic0C5()
	Item::I_hurl, // Intrinsic0C6()
	Actor::I_addHp, // Intrinsic0C7()
	CruHealerProcess::I_create, // Intrinsic0C8()
	Item::I_equip, // Intrinsic0C9()
	Item::I_setBroken, // Intrinsic0CA()
	Item::I_isOn, // Intrinsic0CB()
	Actor::I_teleport, // Intrinsic0CC()
	Item::I_getDirFromTo16, // Intrinsic0CD()
	Item::I_getQHi, // Intrinsic0CE()
	Item::I_isOn, // Intrinsic0CF()
	// 00D0
	Actor::I_isInCombat, // Intrinsic0D0()
	0, // Actor::I_getNPCDataField0x4()
	Actor::I_setCombatTactic, // Intrinsic0D2()
	Actor::I_setDead, // Intrinsic0D3()
	CameraProcess::I_getCameraY, // Intrinsic0D4()
	Actor::I_getEquip, // Intrinsic0D5()
	Actor::I_setEquip, // Intrinsic0D6()
	Actor::I_getDefaultActivity0, // Intrinsic0D7()
	Actor::I_getDefaultActivity1, // Intrinsic0D8()
	Actor::I_getDefaultActivity2, // Intrinsic0D9()
	Actor::I_getLastAnimSet, // Intrinsic0DA()
	Actor::I_isFalling, // Actor::I_isFalling()
	Item::I_getQLo, // Intrinsic0DC()
	Item::I_getQHi, // Intrinsic0DD()
	Actor::I_getNpcNum, // Intrinsic0DE()
	Actor::I_setUnkByte, // Intrinsic0DF() Item::I_setField0x81
	// 00E0
	Item::I_hurl, // Intrinsic0E0()
	Actor::I_setDead, // Intrinsic0E1()
	Item::I_getQLo, // Intrinsic0E2()
	Item::I_getCY, // Intrinsic0E3()
	Actor::I_getNpcNum, // Intrinsic0E4()
	Item::I_hurl, // Intrinsic0E5()
	Actor::I_getNpcNum, // Intrinsic0E6()
	Item::I_hurl, // Intrinsic0E7()
	Actor::I_getNpcNum, // Intrinsic0E8()
	Item::I_hurl, // Intrinsic0E9()
	Actor::I_getNpcNum, // Intrinsic0EA()
	Item::I_hurl, // Intrinsic0EB()
	Actor::I_getNpcNum, // Intrinsic0EC()
	Item::I_hurl, // Intrinsic0ED()
	Actor::I_getNpcNum, // Intrinsic0EE()
	Item::I_hurl, // Intrinsic0EF()
	// 00F0
	Actor::I_getNpcNum, // Intrinsic0F0()
	Item::I_hurl, // Intrinsic0F1()
	Actor::I_getNpcNum, // Intrinsic0F2()
	Item::I_hurl, // Intrinsic0F3()
	Actor::I_getNpcNum, // Intrinsic0F4()
	Item::I_hurl, // Intrinsic0F5()
	Actor::I_getNpcNum, // Intrinsic0F6()
	Item::I_andStatus, // Intrinsic0F7()
	Actor::I_setDead, // Intrinsic0F8()
	Item::I_getQLo, // Intrinsic0F9()
	Actor::I_setDead, // Intrinsic0FA()
	Actor::I_getMaxHp, // DTable::I_getMaxHPForNPC()
	Actor::I_setHp, // Intrinsic0FC()
	Item::I_getQLo, // Intrinsic0FD()
	BatteryChargerProcess::I_create, // Intrinsic0FE()
	Item::I_hurl, // Intrinsic0FF()
	// 0100
	Item::I_andStatus, // Intrinsic100()
	Item::I_isOn, // Intrinsic101()
	Actor::I_isDead, // Intrinsic102()
	Actor::I_setActivity, // Intrinsic103()
	Item::I_getQHi, // Intrinsic104()
	Actor::I_getLastAnimSet, // Intrinsic105()
	Actor::I_setDead, // Intrinsic106()
	Item::I_getQLo, // Intrinsic107()
	Item::I_isOn, // Intrinsic108()
	Item::I_getQHi, // Intrinsic109()
	Item::I_isOn, // Intrinsic10A()
	Item::I_getQHi, // Intrinsic10B()
	Item::I_hurl, // Intrinsic10C()
	Actor::I_getNpcNum, // Intrinsic10D()
	Item::I_getCY, // Intrinsic10E()
	Item::I_hurl, // Intrinsic10F()
	// 0110
	Item::I_isOn, // Intrinsic110()
	MainActor::I_hasKeycard, // Intrinsic111()
	0, // Intrinsic112() - null function (return 0)
	Actor::I_isDead, // Intrinsic113()
	Actor::I_clrImmortal, // Intrinsic114()
	UCMachine::I_numToStr, // Intrinsic115()
	Item::I_getQHi, // Intrinsic116()
	Actor::I_setActivity, // Intrinsic117()
	Item::I_andStatus, // Intrinsic118()
	Actor::I_getNpcNum, // Intrinsic119()
	Item::I_andStatus, // Intrinsic11A()
	Actor::I_getNpcNum, // Intrinsic11B()
	Item::I_isCrusTypeNPC, // Intrinsic11C()
	Item::I_andStatus, // Intrinsic11D()
	Actor::I_getNpcNum, // Intrinsic11E()
	Item::I_avatarStoleSomething, // Intrinsic11F()
	// 0120
	Item::I_andStatus, // Intrinsic120()
	Actor::I_getNpcNum, // Intrinsic121()
	Item::I_getQ, // Intrinsic122()
	Item::I_setQ, // Intrinsic123()
	Item::I_andStatus, // Intrinsic124()
	Actor::I_getNpcNum, // Intrinsic125()
	Item::I_andStatus, // Intrinsic126()
	Actor::I_getNpcNum, // Intrinsic127()
	Item::I_andStatus, // Intrinsic128()
	Actor::I_getNpcNum, // Intrinsic129()
	Item::I_andStatus, // Intrinsic12A()
	Actor::I_getNpcNum, // Intrinsic12B()
	Item::I_andStatus, // Intrinsic12C()
	Actor::I_getNpcNum, // Intrinsic12D()
	Actor::I_getDir, // Intrinsic12E()
	Item::I_andStatus, // Intrinsic12F()
	// 0130
	Actor::I_getNpcNum, // Intrinsic130()
	Item::I_fireDistance, // Intrinsic131() - same as Intrinsic 116 in No Remorse
	Item::I_andStatus, // Intrinsic132()
	Item::I_hurl, // Intrinsic133()
	Item::I_andStatus, // Intrinsic134()
	CameraProcess::I_getCameraY, // Intrinsic135()
	CameraProcess::I_getCameraZ, // Intrinsic136()
	CruStatusGump::I_hideStatusGump, // Intrinsic137(), same as Int05D in Remorse
	Actor::I_clrInCombat, // Intrinsic138()
	Item::I_getTypeFlag, // Intrinsic139()
	Actor::I_getNpcNum, // Intrinsic13A()
	Item::I_hurl, // Intrinsic13B()
	Item::I_getCY, // Intrinsic13C()
	Item::I_getCZ, // Intrinsic13D()
	Item::I_setFrame, // Intrinsic13E()
	AudioProcess::I_playSFX, // Intrinsic13F()
	// 0140
	AudioProcess::I_isSFXPlaying, // Intrinsic140()
	0, // TODO: World::I_clrAlertActive, but not the same as remorse
	PaletteFaderProcess::I_fadeToGivenColor, // Intrinsic142()
	Actor::I_isDead, // Intrinsic143()
	Actor::I_setDead, // Intrinsic144()
	0, // Intrinsic145() - I_maybeShowCredits
	PaletteFaderProcess::I_jumpToAllGrey, // Intrinsic146()
	Item::I_getFamilyOfType, // Intrinsic147()
	Actor::I_getNpcNum, // Intrinsic148()
	Item::I_getQLo, // Intrinsic149()
	Item::I_andStatus, // Intrinsic14A()
	Ultima8Engine::I_getCurrentTimerTick, // Intrinsic14B()
	0, // TODO: World::I_setAlertActive, but not the same as remorse
	Ultima8Engine::I_getAvatarInStasis, // Intrinsic14D()
	MainActor::I_addItemCru, // Intrinsic14E()
	Egg::I_getEggXRange, // Intrinsic14F()
	// 0150
	Actor::I_clrInCombat, // Intrinsic150()
	0, // Intrinsic151() PaletteFaderProcess::I_jumpToColor
	Item::I_setFrame, // Intrinsic152()
	UCMachine::I_numToStr, // Intrinsic153()
	Actor::I_getDir, // Intrinsic154()
	UCMachine::I_numToStr, // Intrinsic155()
	Item::I_isOn, // Intrinsic156()
	Actor::I_getDir, // Intrinsic157()
	Actor::I_setDead, // Intrinsic158()
	Item::I_getQHi, // Intrinsic159()
	Item::I_getQLo, // Intrinsic15A()
	UCMachine::I_numToStr, // Intrinsic15B()
	Actor::I_getDir, // Intrinsic15C()
	0  // Intrinsic15D()
};

} // End of namespace Ultima8
} // End of namespace Ultima

#endif
