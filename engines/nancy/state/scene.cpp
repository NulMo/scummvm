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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "engines/nancy/state/scene.h"
#include "engines/nancy/nancy.h"
#include "engines/nancy/resource.h"
#include "engines/nancy/iff.h"
#include "engines/nancy/action/actionmanager.h"
#include "engines/nancy/input.h"
#include "engines/nancy/audio.h"
#include "engines/nancy/graphics.h"
#include "engines/nancy/cursor.h"
#include "engines/nancy/time.h"

#include "common/memstream.h"
#include "common/rect.h"
#include "common/func.h"

#include "graphics/surface.h"

namespace Nancy {
namespace State{

void Scene::process() {
    switch (_state) {
    case kInit:
        init();
        _state = kLoad;
        // fall through
    case kLoadNew:
        _state = kLoad;
        // fall through
    case kLoad:
        load();
        // fall through
    case kStartSound:
        _state = kRun;
        if (!_sceneState._doNotStartSound) {
            _engine->sound->stopAllSounds();
            _engine->sound->loadSound(_sceneState.summary.audioFile, _sceneState.summary.audioID, 0, _sceneState.summary.audioVolume);
            _engine->sound->pauseSound(_sceneState.summary.audioID, false);
        }
        // fall through
    case kRun:
        run();
        break;
    
    }
}

void Scene::changeScene(uint16 id, uint16 frame, uint16 verticalOffset, bool noSound) {
    _sceneState.nextScene.sceneID = id;
    _sceneState.nextScene.frameID = frame;
    _sceneState.nextScene.verticalOffset = verticalOffset;
    _sceneState._doNotStartSound = noSound;
    _state = kLoadNew;
}

void Scene::pushScene() {
    _sceneState.pushedScene = _sceneState.currentScene;
    _sceneState.isScenePushed = true;
}

void Scene::popScene() {
    changeScene(_sceneState.pushedScene.sceneID, _sceneState.pushedScene.frameID, _sceneState.pushedScene.verticalOffset, true);
    _sceneState.isScenePushed = false;
}

void Scene::addItemToInventory(uint16 id) {
    _flags.items[id] = kTrue;
    if (_flags.heldItem == id) {
        setHeldItem(-1);
    }

    _inventoryBox.addItem(id);
}

void Scene::removeItemFromInventory(uint16 id, bool pickUp) {
    _flags.items[id] = kFalse;

    if (pickUp) {
        setHeldItem(id);
    }

    _inventoryBox.removeItem(id, pickUp);
}

void Scene::registerGraphics() {
    _frame.registerGraphics();
    _viewport.registerGraphics();
    _textbox.registerGraphics();
    _inventoryBox.registerGraphics();

    // Used to clear the map label
    _textbox.setVisible(false);
}

void Scene::init() {
    for (uint i = 0; i < 168; ++i) {
        _flags.eventFlags[i] = kFalse;
    }

    // Does this ever get used?
    for (uint i = 0; i < 1000; ++i) {
        _sceneState.sceneHitCount[i] = 0;
    }

    for (uint i = 0; i < 11; ++i) {
        _flags.items[i] = kFalse;
    }

    _timers.playerTime = _engine->startTimeHours * 3600000;
    _timers.sceneTime = 0;
    _timers.timerTime = 0;
    _timers.timerIsActive = false;
    _timers.tickCount = 0;
    _timers.playerTimeNextMinute = 0;
    _timers.pushedPlayTime = 0;
    _timers.timeOfDay = Timers::kDay;

    _sceneState.nextScene.sceneID = _engine->firstSceneID;

    _frame.init();
    _viewport.init();
    _textbox.init();
    _inventoryBox.init();
    _engine->cursorManager->showCursor(true);

    _state = kLoad;
}

void Scene::load() {
    clearSceneData();

    // Scene IDs are prefixed with S inside the cif tree; e.g 100 -> S100                                                                                    
    Common::String sceneName = Common::String::format("S%u", _sceneState.nextScene.sceneID);
    IFF sceneIFF(_engine, sceneName);
	if (!sceneIFF.load()) {
		error("Faled to load IFF %s", sceneName.c_str());
	}

    Common::SeekableReadStream *sceneSummaryChunk = sceneIFF.getChunkStream("SSUM");
    if (!sceneSummaryChunk) {
        error("Invalid IFF Chunk SSUM");
    }

    readSceneSummary(*sceneSummaryChunk);

    // Search for Action Records, maximum for a scene is 30
    Common::SeekableReadStream *actionRecordChunk = nullptr;

    while (actionRecordChunk = sceneIFF.getChunkStream("ACT", _actionManager._records.size()), actionRecordChunk != nullptr)
    {
        if (_actionManager._records.size() >= 30) {
            error("Invalid number of Action Records");
        }

        _actionManager.addNewActionRecord(*actionRecordChunk);
    }

    _viewport.loadVideo(_sceneState.summary.videoFile, _sceneState.nextScene.frameID, _sceneState.nextScene.verticalOffset);

    // TODO TEMPORARY
    _viewport.setEdgesSize(25, 25, 25, 25);

    if (_viewport.getFrameCount() <= 1) {
        _viewport.disableEdges(kLeft | kRight);
    }

    if (!hasLoadedFromSavefile) {
        _sceneState.currentScene.verticalOffset = _sceneState.nextScene.verticalOffset;
        _sceneState.currentScene.frameID = _sceneState.nextScene.frameID;

        if (_sceneState.summary.videoFormat == 1) {
            // TODO not sure this ever gets hit
        } else if (_sceneState.summary.videoFormat == 2) {
            // always start from the bottom
            _sceneState.currentScene.verticalOffset = _viewport.getMaxScroll();
        } else {
            error("Unrecognized Scene summary chunk video file format");
        }

        // Some checks against rFrame

        if (_sceneState.summary.videoFormat == 1) {
            // TODO not sure this ever gets hit
        } else if (_sceneState.summary.videoFormat == 2) {
            if (_viewport.getMaxScroll() == 0) {
                _viewport.disableEdges(kUp | kDown);
            }
        }
    }

    _sceneState.currentScene = _sceneState.nextScene;
    _timers.sceneTime = 0;

    if (_engine->getGameState() != _engine->getPreviousGameState()) {
        registerGraphics();
    }

    _state = kStartSound;
}

void Scene::run() {
    if (isComingFromMenu) {
        // TODO
    }
    isComingFromMenu = false;

    Time playTime = _engine->getTotalPlayTime();

    if (changeGameState()) {
        return;
    }

    // Do some work if we're coming from a different game state
    if (_engine->getGameState() != _engine->getPreviousGameState()) {
        if (hasLoadedFromSavefile) {
            if (playTime > _timers.pushedPlayTime) {
                playTime -= _timers.pushedPlayTime;
                _timers.totalTime -= playTime;
                _timers.sceneTime -= playTime;
                if (_timers.timerIsActive)
                    _timers.timerTime -= playTime;
            }
        }

        registerGraphics();

        return;
    }

    Time deltaTime = 0;

    if (_timers.tickCount < playTime) {
        deltaTime = playTime - _timers.tickCount;
        _timers.tickCount = playTime;
    }

    _timers.totalTime += deltaTime;

    if (_timers.timerIsActive)
        _timers.timerTime += deltaTime;
    _timers.sceneTime += deltaTime;

    // Calculate the in-game time (playerTime)
    if (playTime > _timers.playerTimeNextMinute) {
        _timers.playerTime += 60000; // Add a minute
        _timers.playerTimeNextMinute = playTime + playerTimeMinuteLength; // Set when we're going to add the next minute
    }

    // Set the time of day according to playerTime
    if (_timers.playerTime.getHours() >= 7 && _timers.playerTime.getHours() < 18) {
        _timers.timeOfDay = Timers::kDay;
    } else if (_timers.playerTime.getHours() >= 19 || _timers.playerTime.getHours() < 6) {
        _timers.timeOfDay = Timers::kNight;
    } else {
        _timers.timeOfDay = Timers::kDuskDawn;
    }


    // Update the UI elements and handle input
    NancyInput input = _engine->input->getInput();
    _viewport.handleInput(input);
    _textbox.handleInput(input);
    _inventoryBox.handleInput(input);
    _actionManager.handleInput(input);

    _actionManager.processActionRecords();
}

void Scene::readSceneSummary(Common::SeekableReadStream &stream) {
    char *buf = new char[0x32];

    stream.seek(0);
    stream.read(buf, 0x31);
    buf[32] = 0;
    _sceneState.summary.description = Common::String(buf);

    stream.seek(1, SEEK_CUR);
    stream.read(buf, 9);
    buf[9] = 0;
    _sceneState.summary.videoFile = Common::String(buf);

    // skip 1 extra byte & 2 unknown bytes
    stream.seek(3, SEEK_CUR);
    _sceneState.summary.videoFormat = stream.readUint16LE();

    stream.read(buf, 10);
    buf[9] = 0;
    _sceneState.summary.audioFile = Common::String(buf);
    _sceneState.summary.audioID = stream.readSint16LE();
    stream.skip(0xE);
    _sceneState.summary.audioVolume = stream.readUint16LE();

    stream.seek(0x72);
    _sceneState.summary.verticalScrollDelta = stream.readUint16LE();
    _sceneState.summary.horizontalEdgeSize = stream.readUint16LE();
    _sceneState.summary.verticalEdgeSize = stream.readUint16LE();
    _sceneState.summary.slowMoveTimeDelta = stream.readUint16LE();
    _sceneState.summary.fastMoveTimeDelta = stream.readUint16LE();

    if (_engine->overrideMovementTimeDeltas) {
        _sceneState.summary.slowMoveTimeDelta = _engine->slowMovementTimeDelta;
        _sceneState.summary.fastMoveTimeDelta = _engine->fastMovementTimeDelta;
    }

    delete[] buf;
}

bool Scene::changeGameState() {
    if (_gameStateRequested != NancyEngine::kScene) {
        _timers.pushedPlayTime = _engine->getTotalPlayTime();
        _engine->setGameState(_gameStateRequested);
        _gameStateRequested = NancyEngine::kScene;

        return true;
    }

    return false;
}

void Scene::clearSceneData() {
    // only clear select flags
    for (uint i = 44; i < 54; ++i) {
        _flags.eventFlags[i] = kFalse;
    }
    for (uint i = 63; i < 74; ++i) {
        _flags.eventFlags[i] = kFalse;
    }
    for (uint i = 75; i < 85; ++i) {
        _flags.eventFlags[i] = kFalse;
    }

    clearLogicConditions();
    _actionManager.clearActionRecords();
}

void Scene::setEventFlag(int16 label, NancyFlag flag) {
    if (label > -1) {
        _flags.eventFlags[label] = flag;
    }
}

bool Scene::getEventFlag(int16 label, NancyFlag flag) {
    if (label > -1) {
        return _flags.eventFlags[label] == flag;
    } else {
        return false;
    }
}

void Scene::setLogicCondition(int16 label, NancyFlag flag) {
    if (label > -1) {
        _flags.logicConditions[label].flag = flag;
        _flags.logicConditions[label].timestamp = _engine->getTotalPlayTime();
    }
}

bool Scene::getLogicCondition(int16 label, NancyFlag flag) {
    if (label > -1) {
        return _flags.logicConditions[label].flag == flag;
    } else {
        return false;
    }
}

void Scene::clearLogicConditions() {
    for (auto &cond : _flags.logicConditions) {
        cond.flag = kFalse;
        cond.timestamp = 0;
    }
}

} // End of namespace State
} // End of namespace Nancy
