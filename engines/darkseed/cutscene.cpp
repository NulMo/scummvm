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

#include "cutscene.h"
#include "darkseed.h"

Darkseed::Cutscene::~Cutscene() {
	delete titleFont;
}

void Darkseed::Cutscene::play(char cutsceneId) {
	_cutsceneId = cutsceneId;
	_movieStep = 1;
	g_engine->_sound->waitForSpeech();
	g_engine->_cursor.showCursor(false);
	g_engine->fadeOut();
}


void Darkseed::Cutscene::update() {
	switch(_cutsceneId) {
	case 'B' : _movieStep = 9999; break;
	case 'C' : _movieStep = 9999; break;
	case 'D' : _movieStep = 9999; break;
	case 'E' : _movieStep = 9999; break;
	case 'G' : _movieStep = 9999; break;
	case 'H' : _movieStep = 9999; break;
	case 'I' : introScene(); break;
	case 'J' : _movieStep = 9999; break;
	case 'Y' : _movieStep = 9999; break;
	case 'Z' : _movieStep = 9999; break;
	}

	if (_movieStep == 9999) {
		if (_cutsceneId == 'E') {
			g_engine->_previousRoomNumber = 38;
			g_engine->changeToRoom(7);
		} else if (_cutsceneId == 'Z') {
			g_engine->restartGame();
		} else if (_cutsceneId == 'I') {
			delete titleFont;
			titleFont = nullptr;
			g_engine->newGame();
		}
	}
}

static constexpr int _CREDITS_DELAY = 25;

bool Darkseed::Cutscene::introScene() {
	switch (_movieStep) {
	case 1: g_engine->fadeOut(); break;
	case 2: if (g_engine->fadeStep()) { return true; } break;
	case 3: {
		g_engine->_screen->clear();
		_palette.load("art/house.pal");
		if (titleFont == nullptr) {
			titleFont = new TitleFont();
		}
		titleFont->displayString(68,160, "DEVELOPING NEW WAYS TO AMAZE");
		g_engine->fadeIn(_palette);
	}
		break;
	case 4: if (g_engine->fadeStep()) { return true; } break;
	case 5: g_engine->fadeOut(); break;
	case 6: if (g_engine->fadeStep()) { return true; } break;
	case 7:
		g_engine->_screen->clear();
		titleFont->displayString(222,160, "CYBERDREAMS");
		g_engine->fadeIn(_palette);
		break;
	case 8: if (g_engine->fadeStep()) { return true; } break;
	case 9: g_engine->fadeOut(); break;
	case 10: if (g_engine->fadeStep()) { return true; } break;
	case 11:
		g_engine->_screen->clear();
		titleFont->displayString(250,160, "PRESENTS");
		g_engine->fadeIn(_palette);
		break;
	case 12: if (g_engine->fadeStep()) { return true; } break;
	case 13: g_engine->fadeOut(); break;
	case 14: if (g_engine->fadeStep()) { return true; } break;
	case 15: {
		g_engine->_screen->clear();
		g_engine->_screen->clearPalette();
		g_engine->_screen->makeAllDirty();
		_palette.load("art/ship.pal", false);
		Img left00Img;
		left00Img.load("art/left00.img");
		left00Img.draw();
		Img left01Img;
		left01Img.load("art/left01.img");
		left01Img.draw();
		Img i001Img;
		i001Img.load("art/i001.img");
		i001Img.draw(1);
		break;
	}
	case 16: g_engine->fadeIn(_palette); break;
	case 17: if (g_engine->fadeStep()) { return true; } break;
	case 18: _animation.load("art/shipin.anm");
		_animIdx = 0;
		_animCount = 47;
		break;
	case 19:
		if (stepAnim()) {
			return true;
		}
		break;
	case 20:
		_animIdx = 47;
		_animCount = _animIdx + 29;
		break;
	case 21:
		if (stepAnim()) {
			return true;
		}
		break;
	case 22: _animation.load("art/t2.anm");
		_animIdx = 0;
		_animCount = 50;
		break;
	case 23:
		if (stepAnim()) {
			return true;
		}
		break;
	case 24: {
		g_engine->_screen->clear();
		g_engine->_screen->makeAllDirty();
		_palette.load("art/house.pal");
		Img tmImg;
		tmImg.load("art/tm.img");
		tmImg.draw();
		Img versionImg;
		versionImg.load("art/version.img");
		versionImg.draw();
		putHouse();
		Img titleImg;
		titleImg.load("art/title.img");
		titleImg.draw(1);
		versionImg.load("art/version.img");
		versionImg.draw();
		registTime();
		break;
	}
	case 25: if (waitTime(_CREDITS_DELAY)) {
			return true;
		}
		break;
	case 26:
		putHouse();
		registTime();
		g_engine->_screen->makeAllDirty();
		break;
	case 27: if (waitTime(_CREDITS_DELAY)) {
			return true;
		}
		break;
	case 28:
		putHouse();
		registTime();
		titleFont->displayString(155, 90, "EXECUTIVE PRODUCERS");
		titleFont->displayString(200, 130, "PATRICK KETCHUM");
		titleFont->displayString(236, 160, "ROLF KLUG");
		titleFont->displayString(236, 190, "JEAN KLUG");
		g_engine->_screen->makeAllDirty();
		break;
	case 29: if (waitTime(_CREDITS_DELAY)) {
			return true;
		}
		break;
	case 30:
		putHouse();
		registTime();
		titleFont->displayString(236, 95, "PRODUCERS");
		titleFont->displayString(200, 135, "HARALD SEELEY");
		titleFont->displayString(218, 165, "MIKE DAWSON");
		g_engine->_screen->makeAllDirty();
		break;
	case 31: if (waitTime(_CREDITS_DELAY)) {
			return true;
		}
		break;
	case 32:
		putHouse();
		registTime();
		titleFont->displayString(245, 95, "DESIGNERS");
		titleFont->displayString(209, 135, "MIKE CRANFORD");
		titleFont->displayString(227, 165, "MIKE DAWSON");
		g_engine->_screen->makeAllDirty();
		break;
	case 33: if (waitTime(_CREDITS_DELAY)) {
			return true;
		}
		break;
	case 34:
		putHouse();
		registTime();
		titleFont->displayString(227, 95, "PROGRAMMERS");
		titleFont->displayString(164, 135, "LENNARD FEDDERSEN");
		titleFont->displayString(227, 165, "JOHN KRAUSE");
		titleFont->displayString(245, 195, "GARY VICK");
		g_engine->_screen->makeAllDirty();
		break;
	case 35: if (waitTime(_CREDITS_DELAY)) {
			return true;
		}
		break;
	case 36:
		putHouse();
		registTime();
		titleFont->displayString(200, 100, "MUSICAL SCORE");
		titleFont->displayString(200, 140, "GREGORY ALPER");
		g_engine->_screen->makeAllDirty();
		break;
	case 37: if (waitTime(_CREDITS_DELAY)) {
			return true;
		}
		break;
	case 38:
		putHouse();
		registTime();
		titleFont->displayString(119, 100, "MUSIC AND SOUND EFFECTS");
		titleFont->displayString(200, 140, "DAVID A. BEAN");
		g_engine->_screen->makeAllDirty();
		break;
	case 39: if (waitTime(_CREDITS_DELAY)) {
			return true;
		}
		break;
	case 40:
		putHouse();
		registTime();
		titleFont->displayString(218, 100, "ART DIRECTOR");
		titleFont->displayString(236, 140, "BRUMMBAER");
		g_engine->_screen->makeAllDirty();
		break;
	case 41: if (waitTime(_CREDITS_DELAY)) {
			return true;
		}
		break;
	case 42:
		putHouse();
		registTime();
		titleFont->displayString(164, 100, "ASST. ART DIRECTOR");
		titleFont->displayString(191, 140, "PAUL DRZEWIECKI");
		g_engine->_screen->makeAllDirty();
		break;
	case 43: if (waitTime(_CREDITS_DELAY)) {
			return true;
		}
		break;
	case 44:
		putHouse();
		registTime();
		titleFont->displayString(200, 100, "DARK WORLD ART");
		titleFont->displayString(245, 140, "H.R. GIGER");
		g_engine->_screen->makeAllDirty();
		break;
	case 45: if (waitTime(_CREDITS_DELAY)) {
			return true;
		}
		break;
	case 46:
		putHouse();
		registTime();
		titleFont->displayString(182, 90, "COMPUTER ARTISTS");
		titleFont->displayString(227, 130, "JULIA ULANO");
		titleFont->displayString(191, 160, "JOBY ROME-OTERO");
		titleFont->displayString(236, 190, "PAUL RYAN");
		g_engine->_screen->makeAllDirty();
		break;
	case 47: if (waitTime(_CREDITS_DELAY)) {
			return true;
		}
		break;
	case 48:
		putHouse();
		registTime();
		titleFont->displayString(236, 100, "GAME TEXT");
		titleFont->displayString(209, 140, "MICHEL HORVAT");
		g_engine->_screen->makeAllDirty();
		break;
	case 49: if (waitTime(_CREDITS_DELAY)) {
			return true;
		}
		break;
	case 50: // TODO wait for music.
		break;
	case 51: g_engine->fadeOut(); break;
	case 52: if (g_engine->fadeStep()) { return true; } break;
	default: _movieStep = 9999; return false;
	}
	_movieStep++;
	return true;
}

bool Darkseed::Cutscene::stepAnim() {
	Img animFrame;
	_animation.getImg(_animIdx, animFrame);
	animFrame.draw(1);
	_animIdx++;
	if (_animIdx < _animCount) {
		return true;
	}
	return false;
}

void Darkseed::Cutscene::putHouse() {
	Img dollImg;
	dollImg.load("art/bdoll0.img");
	dollImg.draw();
	Img doll1Img;
	doll1Img.load("art/bdoll1.img");
	doll1Img.draw();
	Img houseFrameImg;
	houseFrameImg.load("art/house.img");
	houseFrameImg.draw(1);
}

void Darkseed::Cutscene::registTime() {
	_startTime = g_system->getMillis();
}

bool Darkseed::Cutscene::waitTime(int16 duration) {
	return g_system->getMillis() < _startTime + (duration * 100);
}

