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

// Intro sequence scene procedures

#include "saga/saga.h"
#include "saga/gfx.h"

#include "saga/animation.h"
#include "saga/events.h"
#include "saga/font.h"
#include "saga/itedata.h"
#include "saga/sndres.h"
#include "saga/palanim.h"
#include "saga/music.h"

#include "saga/scene.h"
#include "saga/resource.h"

namespace Saga {

using Common::UNK_LANG;
using Common::EN_ANY;
using Common::DE_DEU;
using Common::IT_ITA;

#define INTRO_FRAMETIME 90
#define INTRO_CAPTION_Y 170
#define INTRO_DE_CAPTION_Y 160
#define INTRO_IT_CAPTION_Y 160
#define INTRO_VOICE_PAD 50
#define INTRO_VOICE_LETTERLEN 90

#define DISSOLVE_DURATION 3000
#define LOGO_DISSOLVE_DURATION 1000

// Intro scenes
#define RID_ITE_INTRO_ANIM_SCENE 1538
#define RID_ITE_CAVE_SCENE_1 1542
#define RID_ITE_CAVE_SCENE_2 1545
#define RID_ITE_CAVE_SCENE_3 1548
#define RID_ITE_CAVE_SCENE_4 1551
#define RID_ITE_VALLEY_SCENE 1556
#define RID_ITE_TREEHOUSE_SCENE 1560
#define RID_ITE_FAIREPATH_SCENE 1564
#define RID_ITE_FAIRETENT_SCENE 1567

// ITE intro music
#define MUSIC_INTRO 9
#define MUSIC_TITLE_THEME 10

LoadSceneParams ITE_IntroList[] = {
	{RID_ITE_INTRO_ANIM_SCENE, kLoadByResourceId, Scene::SC_ITEIntroAnimProc, false, kTransitionNoFade, 0, NO_CHAPTER_CHANGE},
	{RID_ITE_CAVE_SCENE_1, kLoadByResourceId, Scene::SC_ITEIntroCave1Proc, false, kTransitionFade, 0, NO_CHAPTER_CHANGE},
	{RID_ITE_CAVE_SCENE_2, kLoadByResourceId, Scene::SC_ITEIntroCave2Proc, false, kTransitionNoFade, 0, NO_CHAPTER_CHANGE},
	{RID_ITE_CAVE_SCENE_3, kLoadByResourceId, Scene::SC_ITEIntroCave3Proc, false, kTransitionNoFade, 0, NO_CHAPTER_CHANGE},
	{RID_ITE_CAVE_SCENE_4, kLoadByResourceId, Scene::SC_ITEIntroCave4Proc, false, kTransitionNoFade, 0, NO_CHAPTER_CHANGE},
	{RID_ITE_VALLEY_SCENE, kLoadByResourceId, Scene::SC_ITEIntroValleyProc, false, kTransitionFade, 0, NO_CHAPTER_CHANGE},
	{RID_ITE_TREEHOUSE_SCENE, kLoadByResourceId, Scene::SC_ITEIntroTreeHouseProc, false, kTransitionNoFade, 0, NO_CHAPTER_CHANGE},
	{RID_ITE_FAIREPATH_SCENE, kLoadByResourceId, Scene::SC_ITEIntroFairePathProc, false, kTransitionNoFade, 0, NO_CHAPTER_CHANGE},
	{RID_ITE_FAIRETENT_SCENE, kLoadByResourceId, Scene::SC_ITEIntroFaireTentProc, false, kTransitionNoFade, 0, NO_CHAPTER_CHANGE}
};

int Scene::ITEStartProc() {
	size_t scenesCount;
	size_t i;

	LoadSceneParams firstScene;
	LoadSceneParams tempScene;

	scenesCount = ARRAYSIZE(ITE_IntroList);

	for (i = 0; i < scenesCount; i++) {
		tempScene = ITE_IntroList[i];
		tempScene.sceneDescriptor = _vm->_resource->convertResourceId(tempScene.sceneDescriptor);
		_vm->_scene->queueScene(tempScene);
	}


	firstScene.loadFlag = kLoadBySceneNumber;
	firstScene.sceneDescriptor = _vm->getStartSceneNumber();
	firstScene.sceneSkipTarget = true;
	firstScene.sceneProc = NULL;
	firstScene.transitionType = kTransitionFade;
	firstScene.actorsEntrance = 0;
	firstScene.chapter = -1;

	_vm->_scene->queueScene(firstScene);

	return SUCCESS;
}

EventColumns *Scene::ITEQueueDialogue(EventColumns *eventColumns, int n_dialogues, const IntroDialogue dialogue[]) {
	TextListEntry textEntry;
	TextListEntry *entry;
	Event event;
	int voice_len;
	int i;

	// Queue narrator dialogue list
	textEntry.knownColor = kKnownColorSubtitleTextColor;
	textEntry.effectKnownColor = kKnownColorTransparent;
	textEntry.useRect = true;
	textEntry.rect.left = 0;
	textEntry.rect.right = _vm->getDisplayInfo().width;
	if (_vm->getLanguage() == Common::DE_DEU) {
		textEntry.rect.top = INTRO_DE_CAPTION_Y;
	} else if (_vm->getLanguage() == Common::IT_ITA) {
		textEntry.rect.top = INTRO_IT_CAPTION_Y;
	} else {
		textEntry.rect.top = INTRO_CAPTION_Y;
	}
	textEntry.rect.bottom = _vm->getDisplayInfo().height;
	textEntry.font = kKnownFontMedium;
	textEntry.flags = (FontEffectFlags)(kFontOutline | kFontCentered);

	for (i = 0; i < n_dialogues; i++) {
		textEntry.text = dialogue[i].i_str;
		entry = _vm->_scene->_textList.addEntry(textEntry);

		if (_vm->_subtitlesEnabled) {
			// Display text
			event.type = kEvTOneshot;
			event.code = kTextEvent;
			event.op = kEventDisplay;
			event.data = entry;
			event.time = (i == 0) ? 0 : INTRO_VOICE_PAD;
			eventColumns = _vm->_events->chain(eventColumns, event);
		}

		if (_vm->_voicesEnabled) {
			// Play voice
			event.type = kEvTOneshot;
			event.code = kVoiceEvent;
			event.op = kEventPlay;
			event.param = dialogue[i].i_voice_rn;
			event.time = 0;
			_vm->_events->chain(eventColumns, event);
		}

		voice_len = _vm->_sndRes->getVoiceLength(dialogue[i].i_voice_rn);
		if (voice_len < 0) {
			voice_len = strlen(dialogue[i].i_str) * INTRO_VOICE_LETTERLEN;
		}

		// Remove text
		event.type = kEvTOneshot;
		event.code = kTextEvent;
		event.op = kEventRemove;
		event.data = entry;
		event.time = voice_len;
		_vm->_events->chain(eventColumns, event);
	}

	return eventColumns;
}

enum {
	kCHeader,
	kCText
};

enum {
	kITEPC           = (1 << 0),
	kITEPCCD         = (1 << 1),
	kITEMac          = (1 << 2),
	kITEWyrmKeep     = (1 << 3),
	kITEAny          = 0xffff,
	kITENotWyrmKeep  = kITEAny & ~kITEWyrmKeep
};

// Queue a page of credits text. The original interpreter did word-wrapping
// automatically. We currently don't.

EventColumns *Scene::ITEQueueCredits(int delta_time, int duration, int n_credits, const IntroCredit credits[]) {
	int game;
	Common::Language lang;
	bool hasWyrmkeepCredits = (Common::File::exists("credit3n.dlt") ||	// PC
							   Common::File::exists("credit3m.dlt"));	// Mac

	// The assumption here is that all WyrmKeep versions have the same
	// credits, regardless of which operating system they're for.

	lang = _vm->getLanguage();

	if (hasWyrmkeepCredits)
		game = kITEWyrmKeep;
	else if (_vm->getPlatform() == Common::kPlatformMacintosh)
		game = kITEMac;
	else if (_vm->getFeatures() & GF_EXTRA_ITE_CREDITS)
		game = kITEPCCD;
	else
		game = kITEPC;

	int line_spacing = 0;
	int paragraph_spacing;
	KnownFont font = kKnownFontSmall;
	int i;

	int n_paragraphs = 0;
	int credits_height = 0;

	for (i = 0; i < n_credits; i++) {
		if (credits[i].lang != lang && credits[i].lang != UNK_LANG) {
			continue;
		}

		if (!(credits[i].game & game)) {
			continue;
		}

		switch (credits[i].type) {
		case kCHeader:
			font = kKnownFontSmall;
			line_spacing = 4;
			n_paragraphs++;
			break;
		case kCText:
			font = kKnownFontMedium;
			line_spacing = 2;
			break;
		default:
			error("Unknown credit type");
		}

		credits_height += (_vm->_font->getHeight(font) + line_spacing);
	}

	paragraph_spacing = (200 - credits_height) / (n_paragraphs + 3);
	credits_height += (n_paragraphs * paragraph_spacing);

	int y = paragraph_spacing;

	TextListEntry textEntry;
	TextListEntry *entry;
	Event event;
	EventColumns *eventColumns = NULL;

	textEntry.knownColor = kKnownColorSubtitleTextColor;
	textEntry.effectKnownColor = kKnownColorTransparent;
	textEntry.flags = (FontEffectFlags)(kFontOutline | kFontCentered);
	textEntry.point.x = 160;

	for (i = 0; i < n_credits; i++) {
		if (credits[i].lang != lang && credits[i].lang != UNK_LANG) {
			continue;
		}

		if (!(credits[i].game & game)) {
			continue;
		}

		switch (credits[i].type) {
		case kCHeader:
			font = kKnownFontSmall;
			line_spacing = 4;
			y += paragraph_spacing;
			break;
		case kCText:
			font = kKnownFontMedium;
			line_spacing = 2;
			break;
		default:
			break;
		}

		textEntry.text = credits[i].string;
		textEntry.font = font;
		textEntry.point.y = y;

		entry = _vm->_scene->_textList.addEntry(textEntry);

		// Display text
		event.type = kEvTOneshot;
		event.code = kTextEvent;
		event.op = kEventDisplay;
		event.data = entry;
		event.time = delta_time;
		eventColumns = _vm->_events->queue(event);

		// Remove text
		event.type = kEvTOneshot;
		event.code = kTextEvent;
		event.op = kEventRemove;
		event.data = entry;
		event.time = duration;
		_vm->_events->chain(eventColumns, event);

		y += (_vm->_font->getHeight(font) + line_spacing);
	}

	return eventColumns;
}

int Scene::SC_ITEIntroAnimProc(int param, void *refCon) {
	return ((Scene *)refCon)->ITEIntroAnimProc(param);
}

// Handles the introductory Dreamer's Guild / NWC logo animation scene.
int Scene::ITEIntroAnimProc(int param) {
	Event event;
	EventColumns *eventColumns;
	bool isMac = _vm->getPlatform() == Common::kPlatformMacintosh;
	bool isMultiCD = _vm->getPlatform() == Common::kPlatformUnknown;
	bool hasWyrmkeepCredits = (Common::File::exists("credit3n.dlt") ||	// PC
							   Common::File::exists("credit3m.dlt"));	// Mac
	bool isDemo = Common::File::exists("scriptsd.rsc");

	switch (param) {
	case SCENE_BEGIN:{
		// Background for intro scene is the first frame of the
		// intro animation; display it and set the palette
		event.type = kEvTOneshot;
		event.code = kBgEvent;
		event.op = kEventDisplay;
		event.param = kEvPSetPalette;
		event.time = 0;
		eventColumns = _vm->_events->queue(event);

		debug(3, "Intro animation procedure started.");
		debug(3, "Linking animation resources...");

		_vm->_anim->setFrameTime(0, INTRO_FRAMETIME);

		// Link this scene's animation resources for continuous
		// playback
		int lastAnim;

		if (hasWyrmkeepCredits || isMultiCD || isDemo)
			lastAnim = isMac ? 3 : 2;
		else
			lastAnim = isMac ? 4 : 5;

		for (int i = 0; i < lastAnim; i++)
			_vm->_anim->link(i, i+1);

		_vm->_anim->setFlag(lastAnim, ANIM_FLAG_ENDSCENE);

		debug(3, "Beginning animation playback.");

		// Begin the animation
		event.type = kEvTOneshot;
		event.code = kAnimEvent;
		event.op = kEventPlay;
		event.param = 0;
		event.time = 0;
		_vm->_events->chain(eventColumns, event);

		// Queue intro music playback
		_vm->_events->chainMusic(eventColumns, MUSIC_INTRO, true);
		}
		break;
	case SCENE_END:
		break;
	default:
		warning("Illegal scene procedure parameter");
		break;
	}

	return 0;
}

int Scene::SC_ITEIntroCave1Proc(int param, void *refCon) {
	return ((Scene *)refCon)->ITEIntroCave1Proc(param);
}

// Handles first introductory cave painting scene
int Scene::ITEIntroCave1Proc(int param) {
	Event event;
	EventColumns *eventColumns;
	int lang = 0;

	if (_vm->getLanguage() == Common::DE_DEU)
		lang = 1;
	else if (_vm->getLanguage() == Common::IT_ITA)
		lang = 2;

	int n_dialogues = ARRAYSIZE(introDialogueCave1[lang]);

	switch (param) {
	case SCENE_BEGIN:
		// Begin palette cycling animation for candles
		event.type = kEvTOneshot;
		event.code = kPalAnimEvent;
		event.op = kEventCycleStart;
		event.time = 0;
		eventColumns = _vm->_events->queue(event);

		// Queue narrator dialogue list
		ITEQueueDialogue(eventColumns, n_dialogues, introDialogueCave1[lang]);

		// End scene after last dialogue over
		event.type = kEvTOneshot;
		event.code = kSceneEvent;
		event.op = kEventEnd;
		event.time = INTRO_VOICE_PAD;
		_vm->_events->chain(eventColumns, event);

		break;
	case SCENE_END:
		break;

	default:
		warning("Illegal scene procedure parameter");
		break;
	}

	return 0;
}

int Scene::SC_ITEIntroCave2Proc(int param, void *refCon) {
	return ((Scene *)refCon)->ITEIntroCave2Proc(param);
}

// Handles second introductory cave painting scene
int Scene::ITEIntroCave2Proc(int param) {
	Event event;
	EventColumns *eventColumns;
	int lang = 0;

	if (_vm->getLanguage() == Common::DE_DEU)
		lang = 1;
	else if (_vm->getLanguage() == Common::IT_ITA)
		lang = 2;

	int n_dialogues = ARRAYSIZE(introDialogueCave2[lang]);

	switch (param) {
	case SCENE_BEGIN:
		// Start 'dissolve' transition to new scene background
		event.type = kEvTContinuous;
		event.code = kTransitionEvent;
		event.op = kEventDissolve;
		event.time = 0;
		event.duration = DISSOLVE_DURATION;
		eventColumns = _vm->_events->queue(event);

		// Begin palette cycling animation for candles
		event.type = kEvTOneshot;
		event.code = kPalAnimEvent;
		event.op = kEventCycleStart;
		event.time = 0;
		_vm->_events->chain(eventColumns, event);

		// Queue narrator dialogue list
		ITEQueueDialogue(eventColumns, n_dialogues, introDialogueCave2[lang]);

		// End scene after last dialogue over
		event.type = kEvTOneshot;
		event.code = kSceneEvent;
		event.op = kEventEnd;
		event.time = INTRO_VOICE_PAD;
		_vm->_events->chain(eventColumns, event);

		break;
	case SCENE_END:
		break;
	default:
		warning("Illegal scene procedure parameter");
		break;
	}

	return 0;
}

int Scene::SC_ITEIntroCave3Proc(int param, void *refCon) {
	return ((Scene *)refCon)->ITEIntroCave3Proc(param);
}

// Handles third introductory cave painting scene
int Scene::ITEIntroCave3Proc(int param) {
	Event event;
	EventColumns *eventColumns;
	int lang = 0;

	if (_vm->getLanguage() == Common::DE_DEU)
		lang = 1;
	else if (_vm->getLanguage() == Common::IT_ITA)
		lang = 2;

	int n_dialogues = ARRAYSIZE(introDialogueCave3[lang]);

	switch (param) {
	case SCENE_BEGIN:
		// Start 'dissolve' transition to new scene background
		event.type = kEvTContinuous;
		event.code = kTransitionEvent;
		event.op = kEventDissolve;
		event.time = 0;
		event.duration = DISSOLVE_DURATION;
		eventColumns = _vm->_events->queue(event);

		// Begin palette cycling animation for candles
		event.type = kEvTOneshot;
		event.code = kPalAnimEvent;
		event.op = kEventCycleStart;
		event.time = 0;
		_vm->_events->chain(eventColumns, event);

		// Queue narrator dialogue list
		ITEQueueDialogue(eventColumns, n_dialogues, introDialogueCave3[lang]);

		// End scene after last dialogue over
		event.type = kEvTOneshot;
		event.code = kSceneEvent;
		event.op = kEventEnd;
		event.time = INTRO_VOICE_PAD;
		_vm->_events->chain(eventColumns, event);

		break;
	case SCENE_END:
		break;
	default:
		warning("Illegal scene procedure parameter");
		break;
	}

	return 0;
}

int Scene::SC_ITEIntroCave4Proc(int param, void *refCon) {
	return ((Scene *)refCon)->ITEIntroCave4Proc(param);
}

// Handles fourth introductory cave painting scene
int Scene::ITEIntroCave4Proc(int param) {
	Event event;
	EventColumns *eventColumns;
	int lang = 0;

	if (_vm->getLanguage() == Common::DE_DEU)
		lang = 1;
	else if (_vm->getLanguage() == Common::IT_ITA)
		lang = 2;

	int n_dialogues = ARRAYSIZE(introDialogueCave4[lang]);

	switch (param) {
	case SCENE_BEGIN:
		// Start 'dissolve' transition to new scene background
		event.type = kEvTContinuous;
		event.code = kTransitionEvent;
		event.op = kEventDissolve;
		event.time = 0;
		event.duration = DISSOLVE_DURATION;
		eventColumns = _vm->_events->queue(event);

		// Begin palette cycling animation for candles
		event.type = kEvTOneshot;
		event.code = kPalAnimEvent;
		event.op = kEventCycleStart;
		event.time = 0;
		_vm->_events->chain(eventColumns, event);

		// Queue narrator dialogue list
		ITEQueueDialogue(eventColumns, n_dialogues, introDialogueCave4[lang]);

		// End scene after last dialogue over
		event.type = kEvTOneshot;
		event.code = kSceneEvent;
		event.op = kEventEnd;
		event.time = INTRO_VOICE_PAD;
		_vm->_events->chain(eventColumns, event);

		break;
	case SCENE_END:
		break;
	default:
		warning("Illegal scene procedure parameter");
		break;
	}

	return 0;
}

int Scene::SC_ITEIntroValleyProc(int param, void *refCon) {
	return ((Scene *)refCon)->ITEIntroValleyProc(param);
}

// Handles intro title scene (valley overlook)
int Scene::ITEIntroValleyProc(int param) {
	Event event;
	EventColumns *eventColumns;

	static const IntroCredit credits[] = {
		{EN_ANY, kITEAny, kCHeader, "Producer"},
		{DE_DEU, kITEAny, kCHeader, "Produzent"},
		{IT_ITA, kITEAny, kCHeader, "Produttore"},
		{UNK_LANG, kITEAny, kCText, "Walter Hochbrueckner"},
		{EN_ANY, kITEAny, kCHeader, "Executive Producer"},
		{DE_DEU, kITEAny, kCHeader, "Ausf\201hrender Produzent"},
		{IT_ITA, kITEAny, kCHeader, "Produttore Esecutivo"},
		{UNK_LANG, kITEAny, kCText, "Robert McNally"},
		{UNK_LANG, kITEWyrmKeep, kCHeader, "2nd Executive Producer"},
		{EN_ANY, kITENotWyrmKeep, kCHeader, "Publisher"},
		{DE_DEU, kITENotWyrmKeep, kCHeader, "Herausgeber"},
		{IT_ITA, kITENotWyrmKeep, kCHeader, "Editore"},
		{UNK_LANG, kITEAny, kCText, "Jon Van Caneghem"}
	};

	int n_credits = ARRAYSIZE(credits);

	switch (param) {
	case SCENE_BEGIN:
		// Begin title screen background animation
		_vm->_anim->setCycles(0, -1);

		event.type = kEvTOneshot;
		event.code = kAnimEvent;
		event.op = kEventPlay;
		event.param = 0;
		event.time = 0;
		eventColumns = _vm->_events->queue(event);

		// Begin ITE title theme music
		_vm->_music->stop();

		_vm->_events->chainMusic(eventColumns, MUSIC_TITLE_THEME);

		// Pause animation before logo
		event.type = kEvTOneshot;
		event.code = kAnimEvent;
		event.op = kEventStop;
		event.param = 0;
		event.time = 3000;
		_vm->_events->chain(eventColumns, event);

		// Display logo
		event.type = kEvTContinuous;
		event.code = kTransitionEvent;
		event.op = kEventDissolveBGMask;
		event.time = 0;
		event.duration = LOGO_DISSOLVE_DURATION;
		_vm->_events->chain(eventColumns, event);

		// Remove logo
		event.type = kEvTContinuous;
		event.code = kTransitionEvent;
		event.op = kEventDissolve;
		event.time = 3000;
		event.duration = LOGO_DISSOLVE_DURATION;
		_vm->_events->chain(eventColumns, event);

		// Unpause animation before logo
		event.type = kEvTOneshot;
		event.code = kAnimEvent;
		event.op = kEventPlay;
		event.time = 0;
		event.param = 0;
		_vm->_events->chain(eventColumns, event);

		// Queue game credits list
		eventColumns = ITEQueueCredits(9000, CREDIT_DURATION1, n_credits, credits);

		// End scene after credit display
		event.type = kEvTOneshot;
		event.code = kSceneEvent;
		event.op = kEventEnd;
		event.time = 1000;
		_vm->_events->chain(eventColumns, event);

		break;
	case SCENE_END:
		break;
	default:
		warning("Illegal scene procedure parameter");
		break;
	}

	return 0;
}

int Scene::SC_ITEIntroTreeHouseProc(int param, void *refCon) {
	return ((Scene *)refCon)->ITEIntroTreeHouseProc(param);
}

// Handles second intro credit screen (treehouse view)
int Scene::ITEIntroTreeHouseProc(int param) {
	Event event;
	EventColumns *eventColumns;

	static const IntroCredit credits1[] = {
		{EN_ANY, kITEAny, kCHeader, "Game Design"},
		{DE_DEU, kITEAny, kCHeader, "Spielentwurf"},
		{IT_ITA, kITEAny, kCHeader, "Progetto"},
		{UNK_LANG, kITEAny, kCText, "Talin, Joe Pearce, Robert McNally"},
		{EN_ANY, kITEAny, kCText, "and Carolly Hauksdottir"},
		{DE_DEU, kITEAny, kCText, "und Carolly Hauksdottir"},
		{IT_ITA, kITEAny, kCText, "e Carolly Hauksdottir"},
		{EN_ANY, kITEAny, kCHeader, "Screenplay and Dialog"},
		{EN_ANY, kITEAny, kCText, "Robert Leh, Len Wein, and Bill Rotsler"},
		{DE_DEU, kITEAny, kCHeader, "Geschichte und Dialoge"},
		{DE_DEU, kITEAny, kCText, "Robert Leh, Len Wein und Bill Rotsler"},
		{IT_ITA, kITEAny, kCHeader, "Sceneggiatura e Dialoghi"},
		{IT_ITA, kITEAny, kCText, "Robert Leh, Len Wein e Bill Rotsler"}
	};

	int n_credits1 = ARRAYSIZE(credits1);

	static const IntroCredit credits2[] = {
		{UNK_LANG, kITEWyrmKeep, kCHeader, "Art Direction"},
		{UNK_LANG, kITEWyrmKeep, kCText, "Allison Hershey"},
		{EN_ANY, kITEAny, kCHeader, "Art"},
		{DE_DEU, kITEAny, kCHeader, "Grafiken"},
		{IT_ITA, kITEAny, kCHeader, "Grafica"},
		{UNK_LANG, kITEWyrmKeep, kCText, "Ed Lacabanne, Glenn Price, April Lee,"},
		{UNK_LANG, kITENotWyrmKeep, kCText, "Edward Lacabanne, Glenn Price, April Lee,"},
		{UNK_LANG, kITEWyrmKeep, kCText, "Lisa Sample, Brian Dowrick, Reed Waller,"},
		{EN_ANY, kITEWyrmKeep, kCText, "Allison Hershey and Talin"},
		{DE_DEU, kITEWyrmKeep, kCText, "Allison Hershey und Talin"},
		{IT_ITA, kITEWyrmKeep, kCText, "Allison Hershey e Talin"},
		{EN_ANY, kITENotWyrmKeep, kCText, "Lisa Iennaco, Brian Dowrick, Reed"},
		{EN_ANY, kITENotWyrmKeep, kCText, "Waller, Allison Hershey and Talin"},
		{DE_DEU, kITEAny, kCText, "Waller, Allison Hershey und Talin"},
		{IT_ITA, kITEAny, kCText, "Waller, Allison Hershey e Talin"},
		{EN_ANY, kITENotWyrmKeep, kCHeader, "Art Direction"},
		{DE_DEU, kITENotWyrmKeep, kCHeader, "Grafische Leitung"},
		{IT_ITA, kITENotWyrmKeep, kCHeader, "Direzione Grafica"},
		{UNK_LANG, kITENotWyrmKeep, kCText, "Allison Hershey"}
	};

	int n_credits2 = ARRAYSIZE(credits2);

	switch (param) {
	case SCENE_BEGIN:
		// Start 'dissolve' transition to new scene background
		event.type = kEvTContinuous;
		event.code = kTransitionEvent;
		event.op = kEventDissolve;
		event.time = 0;
		event.duration = DISSOLVE_DURATION;
		eventColumns = _vm->_events->queue(event);

		if (_vm->_anim->hasAnimation(0)) {
			// Begin title screen background animation
			_vm->_anim->setFrameTime(0, 100);

			event.type = kEvTOneshot;
			event.code = kAnimEvent;
			event.op = kEventPlay;
			event.param = 0;
			event.time = 0;
			_vm->_events->chain(eventColumns, event);
		}

		// Queue game credits list
		ITEQueueCredits(DISSOLVE_DURATION + 2000, CREDIT_DURATION1, n_credits1, credits1);
		eventColumns = ITEQueueCredits(DISSOLVE_DURATION + 7000, CREDIT_DURATION1, n_credits2, credits2);

		// End scene after credit display
		event.type = kEvTOneshot;
		event.code = kSceneEvent;
		event.op = kEventEnd;
		event.time = 1000;
		_vm->_events->chain(eventColumns, event);

		break;
	case SCENE_END:
		break;
	default:
		warning("Illegal scene procedure parameter");
		break;
	}

	return 0;
}

int Scene::SC_ITEIntroFairePathProc(int param, void *refCon) {
	return ((Scene *)refCon)->ITEIntroFairePathProc(param);
}

// Handles third intro credit screen (path to puzzle tent)
int Scene::ITEIntroFairePathProc(int param) {
	Event event;
	EventColumns *eventColumns;

	static const IntroCredit credits1[] = {
		{EN_ANY, kITEAny, kCHeader, "Programming"},
		{DE_DEU, kITEAny, kCHeader, "Programmiert von"},
		{IT_ITA, kITEAny, kCHeader, "Programmazione"},
		{UNK_LANG, kITEAny, kCText, "Talin, Walter Hochbrueckner,"},
		{EN_ANY, kITEAny, kCText, "Joe Burks and Robert Wiggins"},
		{DE_DEU, kITEAny, kCText, "Joe Burks und Robert Wiggins"},
		{IT_ITA, kITEAny, kCText, "Joe Burks e Robert Wiggins"},
		{EN_ANY, kITEPCCD | kITEWyrmKeep, kCHeader, "Additional Programming"},
		{EN_ANY, kITEPCCD | kITEWyrmKeep, kCText, "John Bolton"},
		{UNK_LANG, kITEMac, kCHeader, "Macintosh Version"},
		{UNK_LANG, kITEMac, kCText, "Michael McNally and Robert McNally"},
		{EN_ANY, kITEAny, kCHeader, "Music and Sound"},
		{DE_DEU, kITEAny, kCHeader, "Musik und Sound"},
		{IT_ITA, kITEAny, kCHeader, "Musica e Sonoro"},
		{UNK_LANG, kITEAny, kCText, "Matt Nathan"}
	};

	int n_credits1 = ARRAYSIZE(credits1);

	static const IntroCredit credits2[] = {
		{EN_ANY, kITEAny, kCHeader, "Directed by"},
		{DE_DEU, kITEAny, kCHeader, "Regie"},
		{IT_ITA, kITEAny, kCHeader, "Regia"},
		{UNK_LANG, kITEAny, kCText, "Talin"}
	};

	int n_credits2 = ARRAYSIZE(credits2);

	switch (param) {
	case SCENE_BEGIN:
		// Start 'dissolve' transition to new scene background
		event.type = kEvTContinuous;
		event.code = kTransitionEvent;
		event.op = kEventDissolve;
		event.time = 0;
		event.duration = DISSOLVE_DURATION;
		eventColumns = _vm->_events->queue(event);

		// Begin title screen background animation
		_vm->_anim->setCycles(0, -1);

		event.type = kEvTOneshot;
		event.code = kAnimEvent;
		event.op = kEventPlay;
		event.param = 0;
		event.time = 0;
		_vm->_events->chain(eventColumns, event);

		// Queue game credits list
		ITEQueueCredits(DISSOLVE_DURATION + 2000, CREDIT_DURATION1, n_credits1, credits1);
		eventColumns = ITEQueueCredits(DISSOLVE_DURATION + 7000, CREDIT_DURATION1, n_credits2, credits2);

		// End scene after credit display
		event.type = kEvTOneshot;
		event.code = kSceneEvent;
		event.op = kEventEnd;
		event.time = 1000;
		_vm->_events->chain(eventColumns, event);

		break;
	case SCENE_END:
		break;
	default:
		warning("Illegal scene procedure parameter");
		break;
	}

	return 0;
}

int Scene::SC_ITEIntroFaireTentProc(int param, void *refCon) {
	return ((Scene *)refCon)->ITEIntroFaireTentProc(param);
}

// Handles fourth intro credit screen (treehouse view)
int Scene::ITEIntroFaireTentProc(int param) {
	Event event;
	EventColumns *eventColumns;

	switch (param) {
	case SCENE_BEGIN:

		// Start 'dissolve' transition to new scene background
		event.type = kEvTContinuous;
		event.code = kTransitionEvent;
		event.op = kEventDissolve;
		event.time = 0;
		event.duration = DISSOLVE_DURATION;
		eventColumns = _vm->_events->queue(event);

		// End scene after momentary pause
		event.type = kEvTOneshot;
		event.code = kSceneEvent;
		event.op = kEventEnd;
		event.time = 5000;
		_vm->_events->chain(eventColumns, event);

		break;
	case SCENE_END:
		break;
	default:
		warning("Illegal scene procedure parameter");
		break;
	}

	return 0;
}

} // End of namespace Saga
