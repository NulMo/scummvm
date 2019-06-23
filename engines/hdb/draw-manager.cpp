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

#include "hdb/hdb.h"

namespace HDB {

DrawMan::DrawMan() {
	_tLookupArray = NULL;
	_gfxCache = new Common::Array<GfxCache *>;
	_globalSurface.create(kScreenWidth, kScreenHeight, g_hdb->_format);
	_systemInit = false;
}

DrawMan::~DrawMan() {
	delete _gfxCache;
	_globalSurface.free();
}

bool DrawMan::init() {

	// Read total number of tiles in game
	_numTiles = g_hdb->_fileMan->getCount("t32_", TYPE_TILE32);
	if (!_numTiles) {
		return false;
	}

	// Setup Tile Lookup Array
	_tLookupArray = new TileLookup[_numTiles];
	Common::Array<const char *> tileData = *g_hdb->_fileMan->findFiles("t32_", TYPE_TILE32);

	assert((uint)_numTiles == tileData.size());

	int index = 0, skyIndex = 0;
	for (; index < _numTiles; index++) {
		_tLookupArray[index].filename = tileData[index];
		_tLookupArray[index].tData = NULL;
		_tLookupArray[index].skyIndex = 0;
		_tLookupArray[index].animIndex = index;
		// Check if the loaded Tile is a Sky Tile
		if (((Common::String)tileData[index]).contains("sky") && (skyIndex < kMaxSkies)) {
			_tLookupArray[index].skyIndex = skyIndex + 1;
			_skyTiles[skyIndex] = index;
			skyIndex++;
		}
	}

	/*
		TODO: Add Animating Tile Info
	*/

	// Init Sky Data
	_currentSky = 0;
	_tileSkyStars = getTileIndex("t32_sky_stars");
	_tileSkyStarsLeft = getTileIndex("t32_sky_stars_left_slow");
	_tileSkyClouds = getTileIndex("t32_sky_clouds"); // Not completely sure about this filename.
	_skyClouds = NULL;

	/*
		TODO: Setup Gamma Table
	*/

	/*
		TODO: Load Mouse Pointer and Display Cursor
	*/

	//Load all 4 levels of star colors
	_starField[0] = getPicture("pic_star64");
	_starField[1] = getPicture("pic_star128");
	_starField[2] = getPicture("pic_star192");
	_starField[3] = getPicture("pic_star256");

	/*
		TODO: Load the snowflake
	*/

	_systemInit = true;
	return true;
}

void DrawMan::fillScreen(uint32 color) {
	_globalSurface.fillRect(Common::Rect(kScreenWidth, kScreenHeight), color);
	g_system->fillScreen(color);
}

void DrawMan::updateVideo() {
	updateFade();
	debug(9, "STUB: DrawMan::updateVideo incomplete");
}

void DrawMan::setFade(bool fadeIn, bool black, int steps) {
	_fadeInfo.isFadeIn = fadeIn;
	_fadeInfo.isBlack = black;

	if (!steps) {
		steps = 1;
	}
	_fadeInfo.speed = steps;

	if (fadeIn) {
		_fadeInfo.curStep = 0;
	} else {
		_fadeInfo.curStep = 255;
	}

	_fadeInfo.active = true;
}

void DrawMan::updateFade() {
	_fadeInfo.active = false;
	debug(9, "STUB: DrawMan::updateFade incomplete");
}

Picture *DrawMan::loadPic(const char *picName) {
	Picture *pic = new Picture;
	Common::SeekableReadStream *stream = g_hdb->_fileMan->findFirstData(picName, TYPE_PIC);
	if (!stream)
		return NULL;
	pic->load(stream);
	return pic;
}

Tile *DrawMan::getTile(int index) {

	if (index < 0 || index > _numTiles) {
		if (index != 0xFFFF)
			debug(6, "getTile(%d): wrong index > %d", index, _numTiles);
		return NULL;
	}
	if (_tLookupArray[index].skyIndex) {
		debug(6, "getTile(%d): sky tile (%d)", index, _tLookupArray[index].skyIndex);
		// We don't draw Sky Tiles, so return NULL
		return NULL;
	}

	if (_tLookupArray[index].tData == NULL) {
		Common::SeekableReadStream *stream = g_hdb->_fileMan->findFirstData(_tLookupArray[index].filename, TYPE_TILE32);
		Tile *tile = new Tile;
		tile->load(stream);
		_tLookupArray[index].tData = tile;
	}

	return _tLookupArray[index].tData;
}

void DrawMan::cacheTileSequence(int tileIndex, int count) {
	for (int i = tileIndex;i < tileIndex + count;i++)
		getTile(i);
}

int DrawMan::getTileIndex(const char *name) {
	if (!name) {
		return -1;
	}
	for (int i = 0; i < _numTiles; i++) {
		if (Common::matchString(_tLookupArray[i].filename, name)) {
			return i;
		}
	}
	return -1;
}

Picture *DrawMan::getPicture(const char *name) {
	Common::SeekableReadStream *stream = g_hdb->_fileMan->findFirstData(name, TYPE_PIC);
	Picture *picture = new Picture;
	picture->load(stream);
	return picture;
}

// Returns: true->Tile, false->Pic
bool DrawMan::selectGfxType(const char *name) {
	// Check for Pic types
	if (Common::matchString(name, "clubup1"))
		return false;
	if (Common::matchString(name, "clubup2"))
		return false;
	if (Common::matchString(name, "clubup3"))
		return false;
	if (Common::matchString(name, "clubup4"))
		return false;
	if (Common::matchString(name, "clubdown1"))
		return false;
	if (Common::matchString(name, "clubdown2"))
		return false;
	if (Common::matchString(name, "clubdown3"))
		return false;
	if (Common::matchString(name, "clubdown4"))
		return false;
	if (Common::matchString(name, "clubleft1"))
		return false;
	if (Common::matchString(name, "clubleft2"))
		return false;
	if (Common::matchString(name, "clubleft3"))
		return false;
	if (Common::matchString(name, "clubleft4"))
		return false;
	if (Common::matchString(name, "clubright1"))
		return false;
	if (Common::matchString(name, "clubright2"))
		return false;
	if (Common::matchString(name, "clubright3"))
		return false;
	if (Common::matchString(name, "clubright4"))
		return false;
	if (Common::matchString(name, "slug_shot1"))
		return false;
	if (Common::matchString(name, "slug_shot2"))
		return false;
	if (Common::matchString(name, "slug_shot3"))
		return false;
	if (Common::matchString(name, "slug_shot4"))
		return false;

	return true;
}

Tile *DrawMan::getTileGfx(const char *name, uint32 size) {
	// Try to find graphic
	for (Common::Array<GfxCache *>::iterator it = _gfxCache->begin(); it != _gfxCache->end(); it++) {
		if (Common::matchString((*it)->name, name)) {
			if ((*it)->loaded == -1) {	// Marked for Deletetion?
				(*it)->loaded = 1;		// Reactivate it
				return (*it)->tileGfx;
			}
		}
	}

	GfxCache *gc = new GfxCache;
	strcpy(gc->name, name);

	Common::SeekableReadStream *stream = g_hdb->_fileMan->findFirstData(name, TYPE_TILE32);

	Tile *gfxTile = new Tile;
	gfxTile->load(stream);

	gc->tileGfx = gfxTile;
	gc->size = size;
	gc->loaded = 1;

	_gfxCache->push_back(gc);

	return gc->tileGfx;
}

Picture *DrawMan::getPicGfx(const char *name, uint32 size) {
	// Try to find graphic
	for (Common::Array<GfxCache *>::iterator it = _gfxCache->begin(); it != _gfxCache->end(); it++) {
		if (Common::matchString((*it)->name, name)) {
			if ((*it)->loaded == -1) {	// Marked for Deletetion?
				(*it)->loaded = 1;		// Reactivate it
				return (*it)->picGfx;
			}
		}
	}

	GfxCache *gc = new GfxCache;
	strcpy(gc->name, name);

	Common::SeekableReadStream *stream = g_hdb->_fileMan->findFirstData(name, TYPE_TILE32);

	Picture *gfxPic = new Picture;
	gfxPic->load(stream);

	gc->picGfx = gfxPic;
	gc->size = size;
	gc->loaded = 1;

	_gfxCache->push_back(gc);

	return gc->picGfx;
}

int DrawMan::isSky(int index) {
	if (!index) {
		return 0;
	}

	for (int i = 0; i < kMaxSkies; i++) {
		if(_skyTiles[i] == index) {
			return i + 1; // The skyTiles are indexed from 1. 0 => No Sky tile
		}
	}

	return 0;
}

void DrawMan::setSky(int skyIndex) {
	int tileIndex = _skyTiles[skyIndex - 1];
	_currentSky = skyIndex;

	// Clear memory used by last sky
	if (tileIndex != _tileSkyClouds && _skyClouds) {
		delete _skyClouds;
		_skyClouds = NULL;
	}

	// Setup current sky
	if (tileIndex == _tileSkyStars) {
		setup3DStars();
		return;
	} else if (skyIndex == _tileSkyStarsLeft) {
		setup3DStarsLeft();
		return;
	} else if (skyIndex == _tileSkyStars) {
		_skyClouds = getPicture("cloudy_skies");
		return;
	}
}

void DrawMan::setup3DStars() {
	for (int i = 0; i < kNum3DStars; i++) {
		_stars3D[i].x = g_hdb->_rnd->getRandomNumber(kScreenWidth);
		_stars3D[i].y = g_hdb->_rnd->getRandomNumber(kScreenHeight);
		_stars3D[i].speed = g_hdb->_rnd->getRandomNumber(256);
		_stars3D[i].speed >>= 1;
		_stars3D[i].color = _stars3D[i].speed / 64;
	}
}

void DrawMan::setup3DStarsLeft() {
	for (int i = 0; i < kNum3DStars; i++) {
		_stars3DSlow[i].x = g_hdb->_rnd->getRandomNumber(kScreenWidth);
		_stars3DSlow[i].y = g_hdb->_rnd->getRandomNumber(kScreenHeight);
		_stars3DSlow[i].speed = ((double) (1 + g_hdb->_rnd->getRandomNumber(5))) / 6.0;
		_stars3DSlow[i].color = (int) (_stars3DSlow[i].speed * 4.00);
	}
}

void DrawMan::draw3DStars() {
	fillScreen(0);
	for (int i = 0; i < kNum3DStars; i++) {
		_starField[_stars3D[i].color]->drawMasked((int)_stars3D[i].x, (int)_stars3D[i].y);
		_stars3D[i].y += (_stars3D[i].speed >> 5) + 1;
		if (_stars3D[i].y > kScreenHeight) {
			_stars3D[i].y = 0;
		}
	}
}

void DrawMan::draw3DStarsLeft() {
	fillScreen(0);
	for (int i = 0; i < kNum3DStars; i++) {
		_starField[_stars3DSlow[i].color]->drawMasked((int)_stars3DSlow[i].x, (int)_stars3DSlow[i].y);
		_stars3DSlow[i].x -= _stars3DSlow[i].speed;
		if (_stars3DSlow[i].x < kScreenWidth) {
			_stars3DSlow[i].x = kScreenWidth - 1;
		}
	}
}

void DrawMan::drawSky() {
	int tile = _skyTiles[_currentSky - 1];

	if (tile == _tileSkyStars) {
		draw3DStars();
	}
	else if (tile == _tileSkyStarsLeft) {
		draw3DStarsLeft();
	}
	else if (tile == _tileSkyClouds) {
		static int offset = 0, wait = 0;
		for (int j = -64; j < kScreenHeight; j += 64) {
			for (int i = -64; i < kScreenWidth; i += 64) {
				_skyClouds->draw(i + offset, j + offset);
			}
		}
		wait--;
		if (wait < 1) {
			offset = (offset + 1) & 63;
			wait = 5;
		}
	}
}

int DrawMan::animateTile(int tileIndex) {
	return _tLookupArray[tileIndex].animIndex;
}

// Calculates pixel width of a string
void DrawMan::getDimensions(const char *string, int *pixelsWide, int *lines) {
	if (!string) {
		*pixelsWide = kFontSpace;
		*lines = 1;
		return;
	}

	int width, maxWidth, height;
	unsigned char c;
	maxWidth = 0;
	width = _eLeft;
	height = 1;

	for (int i = 0; i < (int)strlen(string); i++) {
		c = string[i];
		width += _charInfoBlocks[i]->width + _fontHeader.kerning + kFontIncrement;
		if (c == ' ')
			width += kFontSpace;

		if (c == '\n') {
			height++;
			if (width > maxWidth)
				maxWidth = width;
			width = _eLeft;
		} else if (width > _eRight) {
			int oldWidth = width;
			i--;
			while (string[i] != ' ' && i > 0) {
				c = string[i];
				width -= _charInfoBlocks[c]->width + _fontHeader.kerning + kFontIncrement;
				i--;
			}
			if (!i) {
				maxWidth = oldWidth;
				break;
			}
			height++;
			if (width > maxWidth)
				maxWidth = width;
			width = _eLeft;
		}
	}

	if (width > maxWidth)
		maxWidth = width;

	// If its one line, add 8 pixels
	if (height == 1)
		maxWidth += 8;

	*pixelsWide = maxWidth - _eLeft;
	*lines = height;
}

void DrawMan::setTextEdges(int left, int right, int top, int bottom) {
	_eLeft = left;
	_eRight = right;
	_eTop = top;
	_eBottom = bottom;
}

void DrawMan::getTextEdges(int *left, int *right, int *top, int *bottom) {
	*left = _eLeft;
	*right = _eRight;
	*top = _eTop;
	*bottom = _eBottom;
}

void DrawMan::setKernLead(int kern, int lead) {
	_fontHeader.kerning = kern;
	_fontHeader.leading = lead;
}

void DrawMan::getKernLead(int *kern, int *lead) {
	*kern = _fontHeader.kerning;
	*lead = _fontHeader.leading;
}

void DrawMan::setCursor(int x, int y) {
	_cursorX = x;
	_cursorY = y;
}

void DrawMan::getCursor(int *x, int *y) {
	*x = _cursorX;
	*y = _cursorY;
}

Picture::Picture() : _width(0), _height(0), _name("") {
	_surface.create(_width, _height, g_hdb->_format);
}

Picture::~Picture() {
	_surface.free();
}

Graphics::Surface Picture::load(Common::SeekableReadStream *stream) {
	_width = stream->readUint32LE();
	_height = stream->readUint32LE();
	stream->read(_name, 64);

	debug(8, "Picture: _width: %d, _height: %d", _width, _height);
	debug(8, "Picture: _name: %s", _name);

	_surface.create(_width, _height, g_hdb->_format);
	stream->readUint32LE(); // Skip Win32 Surface

	uint16 *ptr;

	for (uint y = 0; y < _height; y++) {
		ptr = (uint16 *)_surface.getBasePtr(0, y);
		for (uint x = 0; x < _width; x++) {
			*ptr = TO_LE_16(stream->readUint16LE());
			ptr++;
		}
	}

	return _surface;
}

void Picture::draw(int x, int y) {
	g_hdb->_drawMan->_globalSurface.transBlitFrom(_surface, Common::Point(x, y));

	Common::Rect clip(_surface.getBounds());
	clip.moveTo(x, y);
	clip.clip(g_hdb->_drawMan->_globalSurface.getBounds());
	if (!clip.isEmpty()) {
		g_system->copyRectToScreen(g_hdb->_drawMan->_globalSurface.getBasePtr(clip.left, clip.top), g_hdb->_drawMan->_globalSurface.pitch, clip.left, clip.top, clip.width(), clip.height());
	}
}

void Picture::drawMasked(int x, int y) {
	g_hdb->_drawMan->_globalSurface.transBlitFrom(_surface, Common::Point(x, y), 0xf81f);

	Common::Rect clip(_surface.getBounds());
	clip.moveTo(x, y);
	clip.clip(g_hdb->_drawMan->_globalSurface.getBounds());
	if (!clip.isEmpty()) {
		g_system->copyRectToScreen(g_hdb->_drawMan->_globalSurface.getBasePtr(clip.left, clip.top), g_hdb->_drawMan->_globalSurface.pitch, clip.left, clip.top, clip.width(), clip.height());
	}
}

Tile::Tile() : _flags(0), _name("") {
	_surface.create(32, 32, g_hdb->_format);
}

Tile::~Tile() {
	_surface.free();
}

Graphics::Surface Tile::load(Common::SeekableReadStream *stream) {
	_flags = stream->readUint32LE();
	stream->read(_name, 64);

	_surface.create(32, 32, g_hdb->_format);
	stream->readUint32LE(); // Skip Win32 Surface

	uint16 *ptr;

	for (uint y = 0; y < 32; y++) {
		ptr = (uint16 *)_surface.getBasePtr(0, y);
		for (uint x = 0; x < 32; x++) {
			*ptr = TO_LE_16(stream->readUint16LE());
			ptr++;
		}
	}

	return _surface;
}

void Tile::draw(int x, int y) {
	g_hdb->_drawMan->_globalSurface.transBlitFrom(_surface, Common::Point(x, y));

	Common::Rect clip(_surface.getBounds());
	clip.moveTo(x, y);
	clip.clip(g_hdb->_drawMan->_globalSurface.getBounds());
	if (!clip.isEmpty()) {
		g_system->copyRectToScreen(g_hdb->_drawMan->_globalSurface.getBasePtr(clip.left, clip.top), g_hdb->_drawMan->_globalSurface.pitch, clip.left, clip.top, clip.width(), clip.height());
	}
}

void Tile::drawMasked(int x, int y) {
	g_hdb->_drawMan->_globalSurface.transBlitFrom(_surface, Common::Point(x, y), 0xf81f);

	Common::Rect clip(_surface.getBounds());
	clip.moveTo(x, y);
	clip.clip(g_hdb->_drawMan->_globalSurface.getBounds());
	if (!clip.isEmpty()) {
		g_system->copyRectToScreen(g_hdb->_drawMan->_globalSurface.getBasePtr(clip.left, clip.top), g_hdb->_drawMan->_globalSurface.pitch, clip.left, clip.top, clip.width(), clip.height());
	}
}

}
