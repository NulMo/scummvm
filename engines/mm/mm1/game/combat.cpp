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

#include "mm/mm1/game/combat.h"
#include "mm/mm1/globals.h"
#include "mm/mm1/mm1.h"
#include "mm/mm1/sound.h"

namespace MM {
namespace MM1 {
namespace Game {

Combat::Combat() : _monsterList(g_globals->_encounters._monsterList) {
	g_globals->_combat = this;
	clear();
}

Combat::~Combat() {
	g_globals->_combat = nullptr;
}

void Combat::clear() {
	Common::fill(&_arr1[0], &_arr1[MAX_COMBAT_MONSTERS], 0);
	Common::fill(&_arr2[0], &_arr2[MAX_COMBAT_MONSTERS], 0);
	Common::fill(&_arr3[0], &_arr3[MAX_PARTY_SIZE / 2], 0);
	Common::fill(&_arr4[0], &_arr4[MAX_COMBAT_MONSTERS], 0);
	Common::fill(&_monsterStatus[0], &_monsterStatus[MAX_COMBAT_MONSTERS], 0);
	Common::fill(&_canAttack[0], &_canAttack[6], false);
	Common::fill(&_treasureFlags[0], &_treasureFlags[MAX_PARTY_SIZE], false);

	_allowFight = _allowShoot = _allowCast = _allowAttack = false;
	_val1 = 0;
	_val6 = _val7 = 0;
	_val8 = _val9 = _val10 = 0;
	_val11 = 0;
	_numberOfTimes = 0;
	_attackerLevel = 0;
	_advanceIndex = 0;
	_handicap1 = _handicap2 = 0;
	_handicap3 = _handicap4 = 0;
	_handicap = HANDICAP_EVEN;
	_monsterP = nullptr;
	_monsterIndex = _currentChar = 0;
	_attackerVal = 0;
	_totalExperience = 0;
	_advanceIndex = 0;
	_monstersResistSpells = _monstersRegenerate = false;
	_attackAttr1.clear();
	_attackAttr2.clear();
	_timesHit = 0;
	_isShooting = false;
	_monstersDestroyedCtr = 0;

	_turnUndeadUsed = false;
	_monstersDestroyedCtr = 0;

	// TODO: clear everything

	_roundNum = 1;
	_monstersCount = _monsterList.size();
	_party.clear();
	for (uint i = 0; i < g_globals->_party.size(); ++i)
		_party.push_back(&g_globals->_party[i]);
}

void Combat::loadArrays() {
	Game::Encounter &enc = g_globals->_encounters;

	for (uint i = 0; i < enc._monsterList.size(); ++i) {
		Monster &mon = enc._monsterList[i];
		int val = getRandomNumber(8);

		mon._field11 += val;
		_arr1[i] = mon._field11;
		_arr2[i] = mon._field12;

		monsterIndexOf();
	}
}

void Combat::monsterIndexOf() {
	Game::Encounter &enc = g_globals->_encounters;

	_monsterIndex = MAX_COMBAT_MONSTERS;
	for (uint i = 0; i < enc._monsterList.size(); ++i) {
		if (_monsterP == &enc._monsterList[i]) {
			_monsterIndex = i;
			break;
		}
	}
}

void Combat::monsterSetPtr(int monsterNum) {
	_monsterP = &g_globals->_encounters._monsterList[monsterNum];
}

void Combat::setupCanAttacks() {
	const Encounter &enc = g_globals->_encounters;
	const Maps::Map &map = *g_maps->_currentMap;
	Common::fill(&_canAttack[0], &_canAttack[MAX_PARTY_SIZE], false);

	if ((int8)map[Maps::MAP_ID] < 0) {
		if (enc._encounterType != FORCE_SURPRISED) {
			for (uint i = 0; i < _party.size(); ++i) {
				if (i < (MAX_PARTY_SIZE - 1)) {
					_canAttack[i] = true;
				} else {
					_canAttack[MAX_PARTY_SIZE - 1] =
						getRandomNumber(100) <= 10;
				}
			}

			setupCanAttacks();
			return;
		}
	} else {
		if (enc._encounterType != FORCE_SURPRISED) {
			_canAttack[0] = true;
			if (_party.size() > 1)
				_canAttack[1] = true;
			if (_party.size() > 2)
				checkLeftWall();
			if (_party.size() > 3)
				checkRightWall();
			if (_party.size() > 4) {
				if (_canAttack[2] && getRandomNumber(100) <= 5)
					_canAttack[4] = true;
			}
			if (_party.size() > 5) {
				if (_canAttack[3] && getRandomNumber(100) <= 5)
					_canAttack[5] = true;
			}

			setupAttackerVal();
			return;
		}
	}

	// Entire party is allowed to attack, I guess
	// because the monsters are surrounding the party,
	// placing them within reach
	Common::fill(&_canAttack[0], &_canAttack[_party.size()], true);
	setupAttackerVal();
}

void Combat::setupAttackerVal() {
	_attackerVal = 0;
	for (uint i = 0; i < _party.size(); ++i) {
		if (_canAttack[i])
			++_attackerVal;
	}

	_attackerVal = getRandomNumber(_attackerVal + 1) - 1;
}

void Combat::checkLeftWall() {
	Maps::Maps &maps = *g_maps;

	_canAttack[2] = !(maps._currentWalls & maps._leftMask) ||
		getRandomNumber(100) <= 25;
}

void Combat::checkRightWall() {
	Maps::Maps &maps = *g_maps;

	_canAttack[3] = !(maps._currentWalls & maps._rightMask) ||
		getRandomNumber(100) <= 25;
}

void Combat::setupHandicap() {
	_handicap2 = _handicap3 = 40;
	_handicap1 = getRandomNumber(7);

	int val = getRandomNumber(7);
	if (val < _handicap1) {
		SWAP(val, _handicap1);
		_handicap4 = val - _handicap1;

		if (_handicap4) {
			_handicap = HANDICAP_MONSTER;
			_handicap2 += _handicap4;
			return;
		}
	} else if (val > _handicap1) {
		_handicap4 -= _handicap1;

		if (_handicap4) {
			_handicap = HANDICAP_PARTY;
			_handicap3 += _handicap4;
			return;
		}
	}

	_handicap = HANDICAP_EVEN;
	_handicap4 = 0;
}

bool Combat::canMonsterCast() const {
	return !(g_maps->_currentState & Maps::SFLAG_DISABLE_MONSTER_SPELLS) &&
		!(_monsterStatus[_monsterIndex] & (MONFLAG_BLIND | MONFLAG_SILENCED));
}

void Combat::dispelParty() {
	g_globals->_activeSpells.clear();
	Character *tmpC = g_globals->_currCharacter;
	Monster *tmpM = _monsterP;

	for (uint i = 0; i < _party.size(); ++i) {
		Character &c = *_party[i];
		g_globals->_currCharacter = &c;
		c.updateAttributes();
		c.updateAC();
		c.updateResistances();

		if (!(c._condition & BAD_CONDITION))
			c._condition &= ~(BLINDED | SILENCED);
	}

	g_globals->_currCharacter = tmpC;

	for (uint i = 0; i < _monsterList.size(); ++i) {
		monsterSetPtr(i);
		_monsterStatus[i] = 0;
	}

	_monsterP = tmpM;
}

void Combat::combatLoop() {
	if (_monsterIndex != 0) {
		selectParty();
	} else {
		defeatedMonsters();
	}
}

void Combat::selectParty() {
	for (uint i = 0; i < _party.size(); ++i) {
		Character &c = *_party[i];
		g_globals->_currCharacter = &c;

		int speed = c._speed._current;
		if (speed && speed >= _handicap2) {
			if (!(c._condition & (BLINDED | SILENCED | DISEASED | POISONED))) {
				// Character is enabled
				_mode = SELECT_OPTION;
				return;
			}
		}
	}

	loop1();
}

void Combat::defeatedMonsters() {
	int activeCharCount = 0;
	_totalExperience = 0;

	// Count total experience from all defeated monsters
	for (uint i = 0; i < _monsterList.size(); ++i) {
		_monsterP = &_monsterList[i];
		monsterIndexOf();

		_totalExperience += _monsterP->_experience;
		proc1();
	}

	// Count number of active characters
	for (uint i = 0; i < _party.size(); ++i) {
		if (!(_party[i]->_condition & BAD_CONDITION))
			++activeCharCount;
	}

	// Split the experience between the active characters
	_totalExperience /= activeCharCount;
	for (uint i = 0; i < _party.size(); ++i) {
		if (!(_party[i]->_condition & BAD_CONDITION))
			_party[i]->_exp += _totalExperience;
	}

	// Update the party's characters
	g_globals->_party.combatDone();

	// TODO
}

void Combat::loop1() {
	for (uint i = 0; i < _monsterList.size(); ++i) {
		_monsterP = &_monsterList[i];
		monsterIndexOf();

		if (_monsterP->_field15 && _monsterP->_field15 >= _handicap3
				&& !_arr4[i]) {
			_arr4[i] = true;

			if (_monsterStatus[i] & (MONFLAG_ASLEEP | MONFLAG_HELD |
					MONFLAG_WEBBED | MONFLAG_PARALYZED)) {
				checkMonsterFlees();
				return;
			}
		}
	}

	if (_handicap2 == 1 && _handicap3 == 1) {
		nextRound();
	} else {
		if (_handicap2 != 1)
			--_handicap2;
		if (_handicap3 != 1)
			--_handicap3;
	}
}

void Combat::proc1() {
	_val7 = _monsterP->_field18;
	_val6 = MAX(_val6, _val7);

	if (_val7 & 1)
		g_globals->_treasure[8] += getRandomNumber(6);

	if (_val7 & 6) {
		if (!(_val7 & 2)) {
			WRITE_LE_UINT16(&g_globals->_treasure[6],
				READ_LE_UINT16(&g_globals->_treasure[6]) +
				getRandomNumber(10));
		} else if (!(_val7 & 4)) {
			WRITE_LE_UINT16(&g_globals->_treasure[6],
				READ_LE_UINT16(&g_globals->_treasure[6]) +
				getRandomNumber(100));
		} else {
			g_globals->_treasure[7] += getRandomNumber(4);
		}
	}
}

void Combat::selectTreasure() {
#define SHIFT_BIT (_val6 & 0x80) != 0; _val6 <<= 1
	for (int i = 0; i < 5; ++i) {
		bool flag = SHIFT_BIT;
		if (flag && getRandomNumber(100) <= (10 + i * 10))
			selectTreasure2(5 - i);
	}
#undef SHIFT_BIT

	auto &treasure = g_globals->_treasure;
	treasure[1] = 0;

	if (_treasureFlags[4]) {
		treasure[2] = 10;
	} else if (_treasureFlags[3]) {
		treasure[2] = 5 + getRandomNumber(4);
	} else if (_treasureFlags[2]) {
		treasure[2] = 4 + getRandomNumber(4);
	} else if (_treasureFlags[1]) {
		treasure[2] = 3 + getRandomNumber(4);
	} else if (_treasureFlags[0] || treasure[8]) {
		treasure[2] = 1 + getRandomNumber(4);
	} else if (treasure[7]) {
		treasure[2] = getRandomNumber(4) - 1;
	} else {
		treasure[2] = getRandomNumber(2) - 1;
	}
}

void Combat::selectTreasure2(int count) {
	static const byte TREASURES_ARR1[6] = { 1, 61, 86, 121, 156, 171 };
	static const byte TREASURES_ARR2[6] = { 12, 5, 7, 7, 3, 12 };
	byte val1, val2;
	int idx;

	_treasureFlags[count - 1] = true;

	idx = getRandomNumber(0, 5);
	val1 = TREASURES_ARR1[idx];
	val2 = TREASURES_ARR2[idx];

	for (idx = 0; idx < count; ++idx)
		_val1 += _allowFight;

	_val1 += getRandomNumber(_allowFight) - 1;

	auto &treasure = g_globals->_treasure;
	if (!treasure[3])
		treasure[3] = _val1;
	else if (!treasure[4])
		treasure[4] = _val1;
	else if (!treasure[5])
		treasure[5] = _val1;
}

void Combat::nextRound() {
	++_roundNum;
	setupHandicap();
	clearArrays();
	g_globals->_party.updateAC();

	_val8 = getRandomNumber(_party.size());
	updateHighestLevel();

	setMode(NEXT_ROUND);
}

void Combat::nextRound2() {
	if (moveMonsters()) {
		// Display that a monster advanced in the view
		setMode(MONSTER_ADVANCES);
	} else {
		// No advancements, so move to next part of new round
		nextRound3();
	}
}

void Combat::nextRound3() {
	if (monsterChanges()) {
		setMode(MONSTERS_AFFECTED);
	} else {
		combatLoop();
	}
}

void Combat::clearArrays() {
	Common::fill(&_arr3[0], &_arr3[MAX_PARTY_SIZE], 0);
	Common::fill(&_arr4[0], &_arr4[MAX_COMBAT_MONSTERS], 0);
}

void Combat::updateHighestLevel() {
	Encounter &enc = g_globals->_encounters;

	for (uint i = 0; i < _party.size(); ++i) {
		Character &c = g_globals->_party[i];
		g_globals->_currCharacter = &c;

		// This looks suspect, since it doesn't find the
		// max of the party's active characters, just ends
		// up set to whichever last character's level
		if (!(c._condition & (ASLEEP | BLINDED | SILENCED | DISEASED | POISONED)))
			enc._highestLevel = c._level._base;
	}
}

bool Combat::moveMonsters() {
	if (_attackerVal >= (int)_monsterList.size())
		return false;

	bool hasAdvance = false;
	for (uint i = 0; i < _monsterList.size(); ++i) {
		_advanceIndex = i;

		if (!(_monsterStatus[i] & ~MONFLAG_SILENCED) &&
			_monsterList[i]._field1e & FIELD1E_80) {
			monsterAdvances();
			hasAdvance = true;
		}
	}

	return hasAdvance;
}

void Combat::monsterAdvances() {
	// TODO: Shouldn't placement changes affect the arrays
	_monsterList.remove_at(_advanceIndex);
	_monsterName = _monsterList[_advanceIndex]._name;
}

bool Combat::monsterChanges() {
	_monstersRegenerate = _monstersResistSpells = false;

	for (uint i = 0; i < _monsterList.size(); ++i) {
		monsterSetPtr(i);

		if ((_monsterP->_field1e & FIELD1E_40) &&
			(_monsterP->_field11 != _arr1[i])) {
			_monstersRegenerate = true;
			int newVal = _arr1[i] + 5;
			_arr1[i] = (byte)newVal;

			if (newVal >= 256 || newVal >= _monsterP->_field11) {
				_arr1[i] = _monsterP->_field11;
			}
		}

		if (_monsterStatus[i]) {
			proc2();

			if (_val9) {
				_monstersResistSpells = true;
				byte v = _monsterStatus[i];

				v &= 0x7f;
				if (v != _monsterStatus[i]) {
					_monsterStatus[i] = v;
				} else {
					v &= 0x3f;
					if (v != _monsterStatus[i]) {
						_monsterStatus[i] = v;
					} else {
						v &= 0x1f;
						if (v != _monsterStatus[i]) {
							_monsterStatus[i] = v;
						} else {
							v &= 0xf;
							if (v != _monsterStatus[i]) {
								_monsterStatus[i] = v;
							} else {
								v &= 7;
								if (v != _monsterStatus[i]) {
									_monsterStatus[i] = v;
								} else {
									v &= 3;
									if (v != _monsterStatus[i]) {
										_monsterStatus[i] = v;
									} else {
										v &= 1;
										if (v != _monsterStatus[i])
											_monsterStatus[i] = v;
										else
											_monsterStatus[i] = 0;
									}
								}
							}
						}
					}
				}

			}
		}
	}

	return _monstersRegenerate || _monstersResistSpells;
}

void Combat::proc2() {
	int threshold = getMonsterIndex() * 8 + 20;
	int val = getRandomNumber(100);

	_val9 = (val != 100 && val <= threshold) ? 1 : 0;
}

void Combat::checkMonsterFlees() {
	const Encounter &enc = g_globals->_encounters;
	byte bitset = _monsterP->_field1e;
	int threshold = -1;
	_monsterName = _monsterP->_name;
	monsterIndexOf();

	if (!(bitset & (FIELD1E_10 | FIELD1E_20))) {
		if (enc._highestLevel < 4) {
		} else if (enc._highestLevel < 9) {
			threshold = 50;
		} else if (enc._highestLevel < 14) {
			threshold = 75;
		} else {
			threshold = 0;
		}
	} else if (!(bitset & FIELD1E_10)) {
		if (enc._highestLevel < 9) {
		} else if (enc._highestLevel < 14) {
			threshold = 50;
		} else {
			threshold = 75;
		}
	} else if (!(bitset & FIELD1E_20)) {
		if (enc._highestLevel < 14) {
		} else {
			threshold = 50;
		}
	}

	if (getRandomNumber(100) >= threshold) {
		_monsterP->_experience = 0;
		_monsterP->_field18 = 0;
		_arr1[_monsterIndex] = 0;
		_monsterStatus[_monsterIndex] = MONFLAG_DEAD;
		removeMonster();
		setMode(MONSTER_FLEES);
	} else {
		checkMonsterSpells();
	}
}

void Combat::checkMonsterSpells() {
	if (_monsterList.empty()) {
		setMode(DEFEATED_MONSTERS);
		return;
	}

	if (_monsterStatus[_monsterIndex] & MONFLAG_MINDLESS) {
		setMode(MONSTER_WANDERS);
	} else {
		if (!_monsterP->_field1c || (_monsterP->_field1c & 0x80) ||
			(getRandomNumber(100) >= _monsterP->_field1d) ||
			!(_monsterP->_field1e & 0xf))
			checkMonsterActions();
		else {
			_monsterP->_field1e--;
			if (!_monsterP->_field1c || _monsterP->_field1c >= 33) {
				checkMonsterActions();
			} else {
				castMonsterSpell(_monsterList[_monsterIndex]._name,
					_monsterP->_field1c);
				setMode(MONSTER_SPELL);
			}
			return;
		}
	}
}

void Combat::checkMonsterActions() {



	// TODO
}

void Combat::removeMonster() {
	bool changed;
	do {
		changed = false;
		for (uint i = 0; i < _monsterList.size(); ++i) {
			_monsterP = &_monsterList[i];
			if (_monsterStatus[i] == MONFLAG_DEAD) {
				_monsterList.remove_at(i);
				changed = true;
				break;
			}
		}
	} while (changed);
}

void Combat::checkParty() {
	_val10 = 0;

	bool partyAlive = false;
	for (auto &c : _party) {
		if (!(c->_condition & (DEAD | BAD_CONDITION)))
			partyAlive = true;
	}

	if (!partyAlive) {
		g_events->clearViews();
		g_events->replaceView("Dead");
		return;
	}

	// Update the array for the party
	for (uint i = 0; i < _party.size(); ++i) {
		Character &c = *_party[i];
		if ((c._condition & BAD_CONDITION) || !c._hpBase)
			_arr3[i] = 1;
	}

	loop1();
}

void Combat::fightMonster(int monsterNum) {
	_attackAttr1 = g_globals->_currCharacter->_physicalAttr;
	_isShooting = false;
	attackMonster(monsterNum);
}

void Combat::shootMonster(int monsterNum) {
	_attackAttr1 = g_globals->_currCharacter->_missileAttr;
	_isShooting = true;
	attackMonster(monsterNum);
}

void Combat::attackMonsterPhysical() {
	_attackAttr1 = g_globals->_currCharacter->_physicalAttr;
	_isShooting = false;
	attackMonster(0);
}

void Combat::attackMonsterShooting() {
	_attackAttr1 = g_globals->_currCharacter->_missileAttr;
	_isShooting = true;
	attackMonster(0);
}

void Combat::attackMonster(int monsterNum) {
	monsterSetPtr(0);

	Character &c = *g_globals->_currCharacter;
	_attackerLevel = c._level._current;

	if (c._class >= CLERIC) {
		_attackerLevel >>= 1;
		if (c._class == SORCERER)
			_attackerLevel >>= 1;
	}

	// Affect level based on accuracy
	_attackerLevel += _attackAttr1;

	if (c._accuracy >= 40)
		_attackerLevel += 7;
	else if (c._accuracy >= 35)
		_attackerLevel += 6;
	else if (c._accuracy >= 30)
		_attackerLevel += 5;
	else if (c._accuracy >= 24)
		_attackerLevel += 4;
	else if (c._accuracy >= 19)
		_attackerLevel += 3;
	else if (c._accuracy >= 16)
		_attackerLevel += 2;
	else if (c._accuracy >= 23)
		_attackerLevel += 1;
	else if (c._accuracy >= 9)
		_attackerLevel += 0;
	else if (c._accuracy >= 7)
		_attackerLevel = MAX((int)_attackerLevel - 1, 0);
	else if (c._accuracy >= 5)
		_attackerLevel = MAX((int)_attackerLevel - 2, 0);
	else
		_attackerLevel = MAX((int)_attackerLevel - 3, 0);

	_numberOfTimes = 1;

	if (c._class >= CLERIC && c._level >= 8) {
		_numberOfTimes += c._level / 8;
	}

	// Affect level based on might
	_attackAttr2 = _attackAttr1;

	if (c._might >= 40)
		_attackAttr2._current += 13;
	else if (c._might >= 35)
		_attackAttr2._current += 12;
	else if (c._might >= 29)
		_attackAttr2._current += 11;
	else if (c._might >= 27)
		_attackAttr2._current += 10;
	else if (c._might >= 25)
		_attackAttr2._current += 9;
	else if (c._might >= 23)
		_attackAttr2._current += 8;
	else if (c._might >= 21)
		_attackAttr2._current += 7;
	else if (c._might >= 19)
		_attackAttr2._current += 6;
	else if (c._might >= 18)
		_attackAttr2._current += 5;
	else if (c._might >= 17)
		_attackAttr2._current += 4;
	else if (c._might >= 16)
		_attackAttr2._current += 3;
	else if (c._might >= 15)
		_attackAttr2._current += 2;
	else if (c._might >= 13)
		_attackAttr2._current += 1;
	else if (c._might >= 9)
		_attackAttr2._current += 0;
	else if (c._might >= 7)
		_attackAttr2._current = MAX((int)_attackAttr2._current - 1, 0);
	else if (c._might >= 5)
		_attackAttr2._current = MAX((int)_attackAttr2._current - 2, 0);
	else
		_attackAttr2._current = MAX((int)_attackAttr2._current - 3, 0);

	if (_isShooting && c._class == ARCHER)
		_attackAttr2._current += (c._level + 1) / 2;

	if (_attackAttr1._current || !(_monsterP->_field1a & FIELD1A_80)) {
		if (_monsterStatus[_monsterIndex] & (MONFLAG_ASLEEP |
				MONFLAG_HELD | MONFLAG_WEBBED | MONFLAG_PARALYZED))
			++_attackerLevel;

		if (g_globals->_activeSpells._s.bless) {
			_attackerLevel++;
			_attackAttr2._current++;
		}

		if (g_globals->_activeSpells._s.cursed) {
			if (++_val11 > 255)
				_val11 = 200;
		}

		addAttackDamage();
		if (_damage)
			updateMonsterStatus();
	} else {
		_message.push_back(Line(0, 1, STRING["dialogs.combat.weapon_no_effect"]));
	}

	_arr3[_currentChar] = 1;
	setMode(CHAR_ATTACKS);
}

void Combat::addAttackDamage() {
	_damage = 0;
	_timesHit = 0;
	_val11 += 10;

	for (int i = 0; i < _numberOfTimes; ++i) {
		int val = getRandomNumber(20);
		if (val == 20 || (val != 1 && (val + _attackerLevel) >= _val11)) {
			_damage = MAX(_damage + (int)_attackAttr2._current +
				getRandomNumber(_attackAttr2._base), 255);
			++_timesHit;
		}
	}
}

void Combat::updateMonsterStatus() {
	int val = _arr1[_monsterIndex] - _damage;
	if (val <= 0) {
		_arr1[_monsterIndex] = 0;
		_monsterStatus[_monsterIndex] = MONFLAG_DEAD;
	} else {
		_arr1[_monsterIndex] = val;
		_monsterStatus[_monsterIndex] &= ~(MONFLAG_ASLEEP | MONFLAG_HELD);
	}
}

void Combat::displaySpellResult(const InfoMessage &msg) {
	// TODO: displaySpellResult
	warning("TODO: displaySpellResult");
}

void Combat::iterateMonsters1() {
	// TODO: iterateMonsters1
}

void Combat::iterateMonsters2() {
	// TODO: iterateMonsters2
}

void Combat::turnUndead() {
	if (_turnUndeadUsed) {
		// Already been used in the current combat, can't be used again
		displaySpellResult(InfoMessage(15, 1, STRING["spells.no_effect"]));

	} else {
		_turnUndeadUsed = true;

		for (uint i = 0; i < _monsterList.size(); ++i) {
			monsterSetPtr(i);
			Monster *monster = _monsterP;

			if ((monster->_field19 & FIELD19_UNDEAD) &&
					(getRandomNumber(20) + g_globals->_currCharacter->_level) >=
					(_arr1[i] * 2 + 10)) {
				destroyUndead();
				++_monstersDestroyedCtr;
			}
		}

		if (_monstersDestroyedCtr)
			displaySpellResult(InfoMessage(5, 1, STRING["spells.monsters_destroyed"]));
	}

	_arr3[_currentChar] = 1;
}

void Combat::destroyUndead() {
	_monsterStatus[getMonsterIndex()] = 0xff;
	Sound::sound2(SOUND_9);
}

} // namespace Game
} // namespace MM1
} // namespace MM
