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

#include "audio/audiostream.h"
#include "audio/decoders/raw.h"
#include "audio/decoders/wave.h"
#include "common/debug.h"
#include "common/system.h"

#include "qdengine/qdengine.h"
#include "qdengine/qd_fwd.h"
#include "qdengine/system/sound/wav_sound.h"
#include "qdengine/qdcore/qd_file_manager.h"


namespace QDEngine {

wavSound::wavSound() {}

wavSound::~wavSound() {}

bool wavSound::wav_file_load(const Common::Path fpath) {
	debugC(3, kDebugSound, "[%d] Loading Wav: %s", g_system->getMillis(), transCyrillic(fpath.toString()));

	if (fpath.empty()) {
		return false;
	}

	_fname = fpath;

	Common::SeekableReadStream *stream;

	if (_fname.baseName().hasSuffixIgnoreCase(".ogg")) {
	}

	if (qdFileManager::instance().open_file(&stream, fpath.toString().c_str(), false)) {
		_audioStream = Audio::makeWAVStream(stream, DisposeAfterUse::NO);

		_length = (float)_audioStream->getLength().msecs() / 1000.0;
	}

	return true;
}

} // namespace QDEngine
