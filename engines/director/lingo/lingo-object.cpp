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

#include "common/endian.h"

#include "graphics/macgui/mactext.h"

#include "director/director.h"
#include "director/movie.h"
#include "director/window.h"
#include "director/lingo/lingo-ast.h"
#include "director/lingo/lingo-code.h"
#include "director/lingo/lingo-the.h"

#include "director/lingo/xlibs/aiff.h"
#include "director/lingo/xlibs/applecdxobj.h"
#include "director/lingo/xlibs/askuser.h"
#include "director/lingo/xlibs/backdrop.h"
#include "director/lingo/xlibs/barakeobj.h"
#include "director/lingo/xlibs/batqt.h"
#include "director/lingo/xlibs/blitpict.h"
#include "director/lingo/xlibs/cdromxobj.h"
#include "director/lingo/xlibs/closebleedwindowxcmd.h"
#include "director/lingo/xlibs/colorxobj.h"
#include "director/lingo/xlibs/colorcursorxobj.h"
#include "director/lingo/xlibs/consumer.h"
#include "director/lingo/xlibs/cursorxobj.h"
#include "director/lingo/xlibs/darkenscreen.h"
#include "director/lingo/xlibs/developerStack.h"
#include "director/lingo/xlibs/dialogsxobj.h"
#include "director/lingo/xlibs/dirutil.h"
#include "director/lingo/xlibs/dllglue.h"
#include "director/lingo/xlibs/dpwavi.h"
#include "director/lingo/xlibs/dpwqtw.h"
#include "director/lingo/xlibs/draw.h"
#include "director/lingo/xlibs/ednox.h"
#include "director/lingo/xlibs/eventq.h"
#include "director/lingo/xlibs/fadegammadownxcmd.h"
#include "director/lingo/xlibs/fadegammaupxcmd.h"
#include "director/lingo/xlibs/fadegammaxcmd.h"
#include "director/lingo/xlibs/fedracul.h"
#include "director/lingo/xlibs/feimasks.h"
#include "director/lingo/xlibs/feiprefs.h"
#include "director/lingo/xlibs/fileexists.h"
#include "director/lingo/xlibs/fileio.h"
#include "director/lingo/xlibs/findereventsxcmd.h"
#include "director/lingo/xlibs/findfolder.h"
#include "director/lingo/xlibs/findsys.h"
#include "director/lingo/xlibs/findwin.h"
#include "director/lingo/xlibs/flushxobj.h"
#include "director/lingo/xlibs/fplayxobj.h"
#include "director/lingo/xlibs/getscreenrectsxfcn.h"
#include "director/lingo/xlibs/getscreensizexfcn.h"
#include "director/lingo/xlibs/gpid.h"
#include "director/lingo/xlibs/hitmap.h"
#include "director/lingo/xlibs/instobj.h"
#include "director/lingo/xlibs/jwxini.h"
#include "director/lingo/xlibs/iscd.h"
#include "director/lingo/xlibs/ispippin.h"
#include "director/lingo/xlibs/jitdraw3.h"
#include "director/lingo/xlibs/labeldrvxobj.h"
#include "director/lingo/xlibs/maniacbg.h"
#include "director/lingo/xlibs/mapnavigatorxobj.h"
#include "director/lingo/xlibs/memcheckxobj.h"
#include "director/lingo/xlibs/memoryxobj.h"
#include "director/lingo/xlibs/misc.h"
#include "director/lingo/xlibs/miscx.h"
#include "director/lingo/xlibs/mmaskxobj.h"
#include "director/lingo/xlibs/mmovie.h"
#include "director/lingo/xlibs/moovxobj.h"
#include "director/lingo/xlibs/movemousexobj.h"
#include "director/lingo/xlibs/movieidxxobj.h"
#include "director/lingo/xlibs/movutils.h"
#include "director/lingo/xlibs/openbleedwindowxcmd.h"
#include "director/lingo/xlibs/orthoplayxobj.h"
#include "director/lingo/xlibs/paco.h"
#include "director/lingo/xlibs/palxobj.h"
#include "director/lingo/xlibs/panel.h"
#include "director/lingo/xlibs/popupmenuxobj.h"
#include "director/lingo/xlibs/porta.h"
#include "director/lingo/xlibs/portaxcmd.h"
#include "director/lingo/xlibs/prefpath.h"
#include "director/lingo/xlibs/printomatic.h"
#include "director/lingo/xlibs/processxobj.h"
#include "director/lingo/xlibs/qtmovie.h"
#include "director/lingo/xlibs/qtcatmovieplayerxobj.h"
#include "director/lingo/xlibs/qtvr.h"
#include "director/lingo/xlibs/quicktime.h"
#include "director/lingo/xlibs/registercomponent.h"
#include "director/lingo/xlibs/remixxcmd.h"
#include "director/lingo/xlibs/serialportxobj.h"
#include "director/lingo/xlibs/soundjam.h"
#include "director/lingo/xlibs/spacemgr.h"
#include "director/lingo/xlibs/stagetc.h"
#include "director/lingo/xlibs/unittest.h"
#include "director/lingo/xlibs/valkyrie.h"
#include "director/lingo/xlibs/videodiscxobj.h"
#include "director/lingo/xlibs/vmisonxfcn.h"
#include "director/lingo/xlibs/volumelist.h"
#include "director/lingo/xlibs/widgetxobj.h"
#include "director/lingo/xlibs/window.h"
#include "director/lingo/xlibs/wininfo.h"
#include "director/lingo/xlibs/winxobj.h"
#include "director/lingo/xlibs/xcmdglue.h"
#include "director/lingo/xlibs/xio.h"
#include "director/lingo/xlibs/xplayanim.h"
#include "director/lingo/xlibs/xsoundxfcn.h"
#include "director/lingo/xlibs/xwin.h"
#include "director/lingo/xlibs/yasix.h"
#include "director/lingo/xtras/scrnutil.h"

namespace Director {

static struct PredefinedProto {
	const char *name;
	void (*func)(int);
	int minArgs;	// -1 -- arglist
	int maxArgs;
	int type;
	int version;
} predefinedMethods[] = {
	// all except window
	{ "new",					LM::m_new,					-1, 0,	kAllObj,				200 },	// D2

	// factory and XObject
	{ "describe",				LM::m_describe,				 0, 0,	kXObj,					200 },	// D2
	{ "dispose",				LM::m_dispose,				 0, 0,	kFactoryObj | kXObj,	200 },	// D2
	{ "get",					LM::m_get,					 1, 1,	kFactoryObj,			200 },	// D2
	{ "instanceRespondsTo",		LM::m_instanceRespondsTo,	 1, 1,	kXObj,					300 },	// D3
	{ "messageList",			LM::m_messageList,			 0, 0,	kXObj,					300 },	// D3
	{ "name",					LM::m_name,					 0, 0,	kXObj,					300 },	// D3
	{ "perform",				LM::m_perform,				-1, 0,	kFactoryObj | kXObj,	300 },	// D3
	{ "put",					LM::m_put,					 2, 2,	kFactoryObj,			200 },	// D2
	{ "respondsTo",				LM::m_respondsTo,			 1, 1,	kXObj,					200 },	// D2

	// script object and Xtra
	{ "birth",					LM::m_new,					-1, 0,	kScriptObj | kXtraObj,	400 },	// D4

	{ nullptr, nullptr, 0, 0, 0, 0 }
};

static MethodProto windowMethods[] = {
	// window / stage
	{ "close",					LM::m_close,				 0, 0,	400 },			// D4
	{ "forget",					LM::m_forget,				 0, 0,	400 },			// D4
	{ "open",					LM::m_open,					 0, 0,	400 },			// D4
	{ "moveToBack",				LM::m_moveToBack,			 0, 0,	400 },			// D4
	{ "moveToFront",			LM::m_moveToFront,			 0, 0,	400 },			// D4
	{ nullptr, nullptr, 0, 0, 0 }
};

void Lingo::initMethods() {
	for (PredefinedProto *mtd = predefinedMethods; mtd->name; mtd++) {
		if (mtd->version > _vm->getVersion())
			continue;

		Symbol sym;
		sym.name = new Common::String(mtd->name);
		sym.type = HBLTIN;
		sym.nargs = mtd->minArgs;
		sym.maxArgs = mtd->maxArgs;
		sym.targetType = mtd->type;
		sym.u.bltin = mtd->func;
		_methods[mtd->name] = sym;
	}
	Window::initMethods(windowMethods);
}

void Lingo::cleanupMethods() {
	_methods.clear();
	Window::cleanupMethods();
}

static struct XLibProto {
	const char **names;
	XLibOpenerFunc opener;
	XLibCloserFunc closer;
	int type;
	int version;
} xlibs[] = {
	{ AiffXObj::fileNames,				AiffXObj::open,				AiffXObj::close,			kXObj,					400 },	// D4
	{ AppleCDXObj::fileNames,			AppleCDXObj::open,			AppleCDXObj::close,			kXObj,					300 },	// D3
	{ AskUser::fileNames,				AskUser::open,				AskUser::close,				kXObj,					400 },	// D4
	{ BackdropXObj::fileNames,			BackdropXObj::open,			BackdropXObj::close,		kXObj,					400 },	// D4
	{ BarakeObj::fileNames,				BarakeObj::open,			BarakeObj::close,			kXObj,					400 },	// D4
	{ BatQT::fileNames,					BatQT::open,				BatQT::close,				kXObj,					400 },	// D4
	{ BlitPictXObj::fileNames,			BlitPictXObj::open,			BlitPictXObj::close,		kXObj,					400 },	// D4
	{ CDROMXObj::fileNames,				CDROMXObj::open,			CDROMXObj::close,			kXObj,					200 },	// D2
	{ CloseBleedWindowXCMD::fileNames,	CloseBleedWindowXCMD::open,	CloseBleedWindowXCMD::close,kXObj,					300 },	// D3
	{ ColorXObj::fileNames,				ColorXObj::open,			ColorXObj::close,			kXObj,					400 },	// D4
	{ ColorCursorXObj::fileNames,		ColorCursorXObj::open,		ColorCursorXObj::close,		kXObj,					400 },	// D4
	{ ConsumerXObj::fileNames,			ConsumerXObj::open,			ConsumerXObj::close,		kXObj,					400 },	// D4
	{ CursorXObj::fileNames,			CursorXObj::open,			CursorXObj::close,			kXObj,					400 },	// D4
	{ DLLGlueXObj::fileNames,			DLLGlueXObj::open,			DLLGlueXObj::close,		kXObj,					400 },	// D4
	{ DPWAVIXObj::fileNames,			DPWAVIXObj::open,			DPWAVIXObj::close,			kXObj,					300 },	// D3
	{ DPWQTWXObj::fileNames,			DPWQTWXObj::open,			DPWQTWXObj::close,			kXObj,					300 },	// D3
	{ DarkenScreen::fileNames,			DarkenScreen::open,			DarkenScreen::close,		kXObj,					300 },	// D3
	{ DeveloperStack::fileNames,		DeveloperStack::open,		DeveloperStack::close,		kXObj,					300 },	// D3
	{ DialogsXObj::fileNames,			DialogsXObj::open,			DialogsXObj::close,			kXObj,					400 },	// D4
	{ DirUtilXObj::fileNames,			DirUtilXObj::open,			DirUtilXObj::close,			kXObj,					400 },	// D4
	{ DrawXObj::fileNames,				DrawXObj::open,				DrawXObj::close,			kXObj,					400 },	// D4
	{ Ednox::fileNames,					Ednox::open,				Ednox::close,				kXObj,					300 },	// D3
	{ EventQXObj::fileNames,			EventQXObj::open,			EventQXObj::close,			kXObj,					400 },	// D4
	{ FEDraculXObj::fileNames,			FEDraculXObj::open,			FEDraculXObj::close,		kXObj,					400 },	// D4
	{ FEIMasksXObj::fileNames,			FEIMasksXObj::open,			FEIMasksXObj::close,		kXObj,					400 },	// D4
	{ FEIPrefsXObj::fileNames,			FEIPrefsXObj::open,			FEIPrefsXObj::close,		kXObj,					400 },	// D4
	{ FadeGammaDownXCMD::fileNames,		FadeGammaDownXCMD::open,	FadeGammaDownXCMD::close,	kXObj,					400 },	// D4
	{ FadeGammaUpXCMD::fileNames,		FadeGammaUpXCMD::open,		FadeGammaUpXCMD::close,		kXObj,					400 },	// D4
	{ FadeGammaXCMD::fileNames,			FadeGammaXCMD::open,		FadeGammaXCMD::close,		kXObj,					400 },	// D4
	{ FileExists::fileNames,			FileExists::open,			FileExists::close,			kXObj,					300 },	// D3
	{ FileIO::fileNames,				FileIO::open,				FileIO::close,				kXObj | kXtraObj,		200 },	// D2
	{ FindFolder::fileNames,			FindFolder::open,			FindFolder::close,			kXObj,					300 },	// D3
	{ FindSys::fileNames,				FindSys::open,				FindSys::close,				kXObj,					400 },	// D4
	{ FindWin::fileNames,				FindWin::open,				FindWin::close,				kXObj,					400 },	// D4
	{ FinderEventsXCMD::fileNames,		FinderEventsXCMD::open,		FinderEventsXCMD::close,	kXObj,					400 },	// D4
	{ FlushXObj::fileNames,				FlushXObj::open,			FlushXObj::close,			kXObj,					300 },	// D3
	{ FPlayXObj::fileNames,				FPlayXObj::open,			FPlayXObj::close,			kXObj,					200 },	// D2
	{ GetScreenRectsXFCN::fileNames,	GetScreenRectsXFCN::open,	GetScreenRectsXFCN::close,	kXObj,					300 },	// D3
	{ GetScreenSizeXFCN::fileNames,		GetScreenSizeXFCN::open,	GetScreenSizeXFCN::close,	kXObj,					300 },	// D3
	{ GpidXObj::fileNames,				GpidXObj::open,				GpidXObj::close,			kXObj,					400 },	// D4
	{ HitMap::fileNames,				HitMap::open,				HitMap::close,				kXObj,					400 },	// D4
	{ InstObjXObj::fileNames,			InstObjXObj::open,			InstObjXObj::close,		kXObj,					400 },	// D4
	{ IsCD::fileNames,					IsCD::open,					IsCD::close,				kXObj,					300 },	// D3
	{ IsPippin::fileNames,				IsPippin::open,				IsPippin::close,			kXObj,					400 },	// D4
	{ JITDraw3XObj::fileNames,			JITDraw3XObj::open,			JITDraw3XObj::close,		kXObj,					400 },	// D4
	{ JourneyWareXINIXObj::fileNames,	JourneyWareXINIXObj::open,	JourneyWareXINIXObj::close,	kXObj,					400 },	// D4
	{ LabelDrvXObj::fileNames,			LabelDrvXObj::open,			LabelDrvXObj::close,		kXObj,					400 },	// D4
	{ MMovieXObj::fileNames,			MMovieXObj::open,			MMovieXObj::close,		kXObj,					400 },	// D4
	{ ManiacBgXObj::fileNames,			ManiacBgXObj::open,			ManiacBgXObj::close,		kXObj,					300 },	// D3
	{ MapNavigatorXObj::fileNames,		MapNavigatorXObj::open,		MapNavigatorXObj::close,	kXObj,					400 },	// D4
	{ MemCheckXObj::fileNames,			MemCheckXObj::open,			MemCheckXObj::close,		kXObj,					400 },	// D4
	{ MemoryXObj::fileNames,			MemoryXObj::open,			MemoryXObj::close,			kXObj,					300 },	// D3
	{ Misc::fileNames,					Misc::open,					Misc::close,				kXObj,					400 },	// D4
	{ MiscX::fileNames,					MiscX::open,				MiscX::close,				kXObj,					400 },	// D4
	{ MMaskXObj::fileNames,				MMaskXObj::open,			MMaskXObj::close,			kXObj,					400 },	// D4
	{ MoovXObj::fileNames, 				MoovXObj::open, 			MoovXObj::close,			kXObj,					300 },  // D3
	{ MovUtilsXObj::fileNames,			MovUtilsXObj::open,			MovUtilsXObj::close,		kXObj,					400 },	// D4
	{ MoveMouseXObj::fileNames,			MoveMouseXObj::open,		MoveMouseXObj::close,		kXObj,					400 },	// D4
	{ MovieIdxXObj::fileNames,			MovieIdxXObj::open,			MovieIdxXObj::close,		kXObj,					400 },	// D4
	{ MovUtilsXObj::fileNames,			MovUtilsXObj::open,			MovUtilsXObj::close,		kXObj,					400 },	// D4
	{ OpenBleedWindowXCMD::fileNames,	OpenBleedWindowXCMD::open,	OpenBleedWindowXCMD::close,	kXObj,					300 },	// D3
	{ OrthoPlayXObj::fileNames,			OrthoPlayXObj::open,		OrthoPlayXObj::close,		kXObj,					400 },	// D4
	{ PACoXObj::fileNames,				PACoXObj::open,				PACoXObj::close,			kXObj,					300 },	// D3
	{ PalXObj::fileNames,				PalXObj::open,				PalXObj::close,				kXObj,					400 },	// D4
	{ PanelXObj::fileNames,				PanelXObj::open,			PanelXObj::close,			kXObj,					200 },	// D2
	{ PopUpMenuXObj::fileNames,			PopUpMenuXObj::open,		PopUpMenuXObj::close,		kXObj,					200 },	// D2
	{ Porta::fileNames,					Porta::open,				Porta::close,				kXObj,					300 },	// D3
	{ PortaXCMD::fileNames,				PortaXCMD::open,			PortaXCMD::close,			kXObj,					300 },	// D3
	{ PrefPath::fileNames,				PrefPath::open,				PrefPath::close,			kXObj,					400 },	// D4
	{ PrintOMaticXObj::fileNames,		PrintOMaticXObj::open,		PrintOMaticXObj::close,		kXObj,					400 },	// D4
	{ ProcessXObj::fileNames,			ProcessXObj::open,			ProcessXObj::close,			kXObj,					400 },	// D4
	{ QTCatMoviePlayerXObj::fileNames,	QTCatMoviePlayerXObj::open,	QTCatMoviePlayerXObj::close,kXObj,					400 },	// D4
	{ QTMovie::fileNames,				QTMovie::open,				QTMovie::close,				kXObj,					400 },	// D4
	{ QTVR::fileNames,					QTVR::open,					QTVR::close,				kXObj,					400 },	// D4
	{ Quicktime::fileNames,				Quicktime::open,			Quicktime::close,			kXObj,					300 },	// D3
	{ RearWindowXObj::fileNames,		RearWindowXObj::open,		RearWindowXObj::close,		kXObj,					400 },	// D4
	{ RegisterComponent::fileNames,		RegisterComponent::open,	RegisterComponent::close,	kXObj,					400 },	// D4
	{ RemixXCMD::fileNames,				RemixXCMD::open,			RemixXCMD::close,			kXObj,					300 },	// D3
	{ ScrnUtilXtra::fileNames,			ScrnUtilXtra::open,			ScrnUtilXtra::close,		kXtraObj,				500 },	// D5
	{ SerialPortXObj::fileNames,		SerialPortXObj::open,		SerialPortXObj::close,		kXObj,					200 },	// D2
	{ SoundJam::fileNames,				SoundJam::open,				SoundJam::close,			kXObj,					400 },	// D4
	{ SpaceMgr::fileNames,				SpaceMgr::open,				SpaceMgr::close,			kXObj,					400 },	// D4
	{ StageTCXObj::fileNames,			StageTCXObj::open,			StageTCXObj::close,			kXObj,					400 },	// D4
	{ UnitTestXObj::fileNames,			UnitTestXObj::open,			UnitTestXObj::close,		kXObj,					400 },	// D4
	{ VMisOnXFCN::fileNames,			VMisOnXFCN::open,			VMisOnXFCN::close,			kXObj,					400 },	// D4
	{ ValkyrieXObj::fileNames,			ValkyrieXObj::open,			ValkyrieXObj::close,		kXObj,					400 },	// D4
	{ VideodiscXObj::fileNames,			VideodiscXObj::open,		VideodiscXObj::close,		kXObj,					200 },	// D2
	{ VolumeList::fileNames,			VolumeList::open,			VolumeList::close,			kXObj,					300 },	// D3
	{ WinInfoXObj::fileNames,			WinInfoXObj::open,			WinInfoXObj::close,			kXObj,					400 },  // D4
	{ WidgetXObj::fileNames,			WidgetXObj::open,			WidgetXObj::close, 			kXObj,					400 },  // D4
	{ WindowXObj::fileNames,			WindowXObj::open,			WindowXObj::close,			kXObj,					200 },	// D2
	{ XCMDGlueXObj::fileNames,			XCMDGlueXObj::open,			XCMDGlueXObj::close,		kXObj,					200 },	// D2
	{ XSoundXFCN::fileNames,			XSoundXFCN::open,			XSoundXFCN::close,			kXObj,					400 },	// D4
	{ XWINXObj::fileNames,				XWINXObj::open,				XWINXObj::close,			kXObj,					300 },	// D3
	{ XioXObj::fileNames,				XioXObj::open,				XioXObj::close,				kXObj,					400 },	// D3
	{ XPlayAnim::fileNames,				XPlayAnim::open,			XPlayAnim::close,			kXObj,					300 },	// D3
	{ Yasix::fileNames,					Yasix::open,				Yasix::close,				kXObj,					300 },	// D3
	{ nullptr, nullptr, nullptr, 0, 0 }
};

void Lingo::initXLibs() {
	for (XLibProto *lib = xlibs; lib->names; lib++) {
		if (lib->version > _vm->getVersion())
			continue;

		for (uint i = 0; lib->names[i]; i++) {
			_xlibOpeners[lib->names[i]] = lib->opener;
			_xlibClosers[lib->names[i]] = lib->closer;
		}
	}
}

void Lingo::cleanupXLibs() {
	_xlibOpeners.clear();
	_xlibClosers.clear();
}

Common::String Lingo::normalizeXLibName(Common::String name) {
	// Normalize to remove machintosh path delimiters (':', '@:')
	name = convertPath(name);

	size_t pos = name.findLastOf(g_director->_dirSeparator);
	if (pos != Common::String::npos)
		name = name.substr(pos + 1, name.size());

	Common::Platform platform = _vm->getPlatform();
	if (platform == Common::kPlatformMacintosh || platform == Common::kPlatformMacintoshII) {
		if (name.hasSuffixIgnoreCase(".xlib"))
			name = name.substr(0, name.size() - 5);
	} else if (platform == Common::kPlatformWindows) {
		if (name.hasSuffixIgnoreCase(".dll"))
			name = name.substr(0, name.size() - 4);
		if (name.hasSuffixIgnoreCase(".x16"))
			name = name.substr(0, name.size() - 4);
		if (name.hasSuffixIgnoreCase(".x32"))
			name = name.substr(0, name.size() - 4);
	}

	name.trim();

	return name;
}

void Lingo::openXLib(Common::String name, ObjectType type, const Common::Path &path) {
	name = normalizeXLibName(name);

	if (_openXLibs.contains(name))
		return;

	_openXLibs[name] = type;

	if (_xlibOpeners.contains(name)) {
		(*_xlibOpeners[name])(type, path);
	} else {
		warning("Lingo::openXLib: Unimplemented xlib: '%s'", name.c_str());
	}
}

void Lingo::closeXLib(Common::String name) {
	name = normalizeXLibName(name);

	if (!_openXLibs.contains(name)) {
		warning("Lingo::closeXLib: xlib %s is not open", name.c_str());
		return;
	}

	ObjectType type = _openXLibs[name];
	_openXLibs.erase(name);

	if (_xlibClosers.contains(name)) {
		(*_xlibClosers[name])(type);
	} else {
		warning("Lingo::closeXLib: Unimplemented xlib: '%s'", name.c_str());
	}
}

void Lingo::closeOpenXLibs() {
	for (auto &it : _openXLibs) {
		closeXLib(it._key);
	}
}

void Lingo::reloadOpenXLibs() {
	OpenXLibsHash openXLibsCopy = _openXLibs;
	for (auto &it : openXLibsCopy) {
		closeXLib(it._key);
		// FIXME: keep track of where the xlib path is
		openXLib(it._key, it._value, Common::Path());
	}
}

// Initialization/disposal

void LM::m_new(int nargs) {
	// This is usually overridden by a user-defined mNew
	g_lingo->printSTUBWithArglist("m_new", nargs);
	g_lingo->push(g_lingo->_state->me);
}

void LM::m_dispose(int nargs) {
	g_lingo->_state->me.u.obj->dispose();
}

/* ScriptContext */

ScriptContext::ScriptContext(Common::String name, ScriptType type, int id)
	: Object<ScriptContext>(name), _scriptType(type), _id(id) {
	_objType = kScriptObj;
}

ScriptContext::ScriptContext(const ScriptContext &sc) : Object<ScriptContext>(sc) {
	_scriptType = sc._scriptType;
	_functionNames = sc._functionNames;
	for (auto &it : sc._functionHandlers) {
		_functionHandlers[it._key] = it._value;
		_functionHandlers[it._key].ctx = this;
	}
	for (auto &it : sc._eventHandlers) {
		_eventHandlers[it._key] = it._value;
		_eventHandlers[it._key].ctx = this;
	}
	_constants = sc._constants;
	_properties = sc._properties;
	_propertyNames = sc._propertyNames;

	_id = sc._id;
}

ScriptContext::~ScriptContext() {
}

Common::String ScriptContext::asString() {
	return Common::String::format("script: %d \"%s\" %d %p", _id, _name.c_str(), _inheritanceLevel, (void *)this);
}

Symbol ScriptContext::define(const Common::String &name, ScriptData *code, Common::Array<Common::String> *argNames, Common::Array<Common::String> *varNames) {
	Symbol sym;
	sym.name = new Common::String(name);
	sym.type = HANDLER;
	sym.u.defn = code;
	sym.nargs = argNames->size();
	sym.maxArgs = argNames->size();
	sym.argNames = argNames;
	sym.varNames = varNames;
	sym.ctx = this;

	if (debugChannelSet(1, kDebugCompile)) {
		debugC(1, kDebugCompile, "%s", g_lingo->formatFunctionBody(sym).c_str());
		debugC(1, kDebugCompile, "<end define code>");
	}

	_functionHandlers[name] = sym;
	if (g_lingo->_eventHandlerTypeIds.contains(name)) {
		_eventHandlers[g_lingo->_eventHandlerTypeIds[name]] = sym;
	}

	return sym;
}

Symbol ScriptContext::getMethod(const Common::String &methodName) {
	Symbol sym;

	if (_functionHandlers.contains(methodName)) {
		sym = _functionHandlers[methodName];
		sym.target = this;
		return sym;
	}

	sym = Object<ScriptContext>::getMethod(methodName);
	if (sym.type != VOIDSYM)
		return sym;

	if (_objType == kScriptObj) {
		if (_properties.contains("ancestor") && _properties["ancestor"].type == OBJECT
				&& (_properties["ancestor"].u.obj->getObjType() & (kScriptObj | kXtraObj))) {
			// ancestor method
			debugC(3, kDebugLingoExec, "Calling method '%s' on ancestor: <%s>", methodName.c_str(), _properties["ancestor"].asString(true).c_str());
			return _properties["ancestor"].u.obj->getMethod(methodName);
		}
	}

	return sym;
}

bool ScriptContext::hasProp(const Common::String &propName) {
	if (_disposed) {
		error("Property '%s' accessed on disposed object <%s>", propName.c_str(), Datum(this).asString(true).c_str());
	}
	if (_properties.contains(propName)) {
		return true;
	}
	if (_objType == kScriptObj) {
		if (_properties.contains("ancestor") && _properties["ancestor"].type == OBJECT
				&& (_properties["ancestor"].u.obj->getObjType() & (kScriptObj | kXtraObj))) {
			return _properties["ancestor"].u.obj->hasProp(propName);
		}
	}
	return false;
}

Datum ScriptContext::getProp(const Common::String &propName) {
	if (_disposed) {
		error("Property '%s' accessed on disposed object <%s>", propName.c_str(), Datum(this).asString(true).c_str());
	}
	if (_properties.contains(propName)) {
		return _properties[propName];
	}
	if (_objType == kScriptObj) {
		if (_properties.contains("ancestor") && _properties["ancestor"].type == OBJECT
				&& (_properties["ancestor"].u.obj->getObjType() & (kScriptObj | kXtraObj))) {
			debugC(3, kDebugLingoExec, "Getting prop '%s' from ancestor: <%s>", propName.c_str(), _properties["ancestor"].asString(true).c_str());
			return _properties["ancestor"].u.obj->getProp(propName);
		}
	}
	_propertyNames.push_back(propName);
	return _properties[propName]; // return new property
}

Common::String ScriptContext::getPropAt(uint32 index) {
	uint32 target = 1;
	for (auto &it : _propertyNames) {
		if (target == index) {
			return it;
		}
		target += 1;
	}
	return Common::String();
}

uint32 ScriptContext::getPropCount() {
	return _propertyNames.size();
}

bool ScriptContext::setProp(const Common::String &propName, const Datum &value, bool force) {
	if (_disposed) {
		error("Property '%s' accessed on disposed object <%s>", propName.c_str(), Datum(this).asString(true).c_str());
	}
	if (_properties.contains(propName)) {
		_properties[propName] = value;
		return true;
	}
	if (force) {
		// used by e.g. the script compiler to add properties
		_propertyNames.push_back(propName);
		_properties[propName] = value;
		return true;
	} else if (_objType == kScriptObj) {
		if (_properties.contains("ancestor") && _properties["ancestor"].type == OBJECT
				&& (_properties["ancestor"].u.obj->getObjType() & (kScriptObj | kXtraObj))) {
			debugC(3, kDebugLingoExec, "Getting prop '%s' from ancestor: <%s>", propName.c_str(), _properties["ancestor"].asString(true).c_str());
			return _properties["ancestor"].u.obj->setProp(propName, value, force);
		}
	} else if (_objType == kFactoryObj) {
		// D3 style anonymous objects/factories, set whatever properties you like
		_propertyNames.push_back(propName);
		_properties[propName] = value;
		return true;
	}
	return false;
}

Common::String ScriptContext::formatFunctionList(const char *prefix) {
	Common::String result;
	for (auto it = _functionHandlers.begin(); it != _functionHandlers.end(); ++it) {
		result += Common::String::format("%s%s\n", prefix, g_lingo->formatFunctionName(it->_value).c_str());
	}
	return result;
}


// Object array

void LM::m_get(int nargs) {
	ScriptContext *me = static_cast<ScriptContext *>(g_lingo->_state->me.u.obj);
	Datum indexD = g_lingo->pop();
	uint index = MAX(0, indexD.asInt());
	if (me->_objArray.contains(index)) {
		g_lingo->push(me->_objArray[index]);
	} else {
		g_lingo->push(Datum(0));
	}
}

void LM::m_put(int nargs) {
	ScriptContext *me = static_cast<ScriptContext *>(g_lingo->_state->me.u.obj);
	Datum value = g_lingo->pop();
	Datum indexD = g_lingo->pop();
	uint index = MAX(0, indexD.asInt());
	me->_objArray[index] = value;
}

// Other

void LM::m_perform(int nargs) {
	bool allowRetVal = g_lingo->pop().asInt() != 0; // Pop allowRetVal that should be used for the LC::Call

	// Lingo doesn't seem to bother cloning the object when
	// mNew is called with mPerform
	Datum d(g_lingo->_state->me);
	AbstractObject *me = d.u.obj;
	Datum methodName = g_lingo->_stack.remove_at(g_lingo->_stack.size() - nargs); // Take method name out of stack
	Symbol funcSym = me->getMethod(*methodName.u.s);
	// Object methods expect the first argument to be the object
	g_lingo->_stack.insert_at(g_lingo->_stack.size() - nargs + 1, d);
	LC::call(funcSym, nargs, allowRetVal);

	if (allowRetVal) {
		// If the method expects a return value, push dummy on stack
		g_lingo->pushVoid();
	}
}

// XObject

void LM::m_describe(int nargs) {
	warning("STUB: m_describe");
}

void LM::m_instanceRespondsTo(int nargs) {
	AbstractObject *me = g_lingo->_state->me.u.obj;
	Datum d = g_lingo->pop();
	Common::String methodName = d.asString();

	if (me->getMethod(methodName).type != VOIDSYM) {
		g_lingo->push(Datum(1));
	} else {
		g_lingo->push(Datum(0));
	}
}

void LM::m_messageList(int nargs) {
	warning("STUB: m_messageList");
	g_lingo->push(Datum(""));
}

void LM::m_name(int nargs) {
	AbstractObject *me = g_lingo->_state->me.u.obj;
	g_lingo->push(me->getName());
}

void LM::m_respondsTo(int nargs) {
	AbstractObject *me = g_lingo->_state->me.u.obj;
	Datum d = g_lingo->pop();
	Common::String methodName = d.asString();

	// TODO: Check inheritance level
	if (me->getMethod(methodName).type != VOIDSYM) {
		g_lingo->push(Datum(1));
	} else {
		g_lingo->push(Datum(0));
	}
}

// Window

Common::String Window::asString() {
	return "window \"" + getName() + "\"";
}

bool Window::hasProp(const Common::String &propName) {
	Common::String fieldName = Common::String::format("%d%s", kTheWindow, propName.c_str());
	return g_lingo->_theEntityFields.contains(fieldName) && hasField(g_lingo->_theEntityFields[fieldName]->field);
}

Datum Window::getProp(const Common::String &propName) {
	Common::String fieldName = Common::String::format("%d%s", kTheWindow, propName.c_str());
	if (g_lingo->_theEntityFields.contains(fieldName)) {
		return getField(g_lingo->_theEntityFields[fieldName]->field);
	}

	warning("Window::getProp: unknown property '%s'", propName.c_str());
	return Datum();
}

bool Window::setProp(const Common::String &propName, const Datum &value, bool force) {
	Common::String fieldName = Common::String::format("%d%s", kTheWindow, propName.c_str());
	if (g_lingo->_theEntityFields.contains(fieldName)) {
		return setField(g_lingo->_theEntityFields[fieldName]->field, value);
	}

	warning("Window::setProp: unknown property '%s'", propName.c_str());
	return false;
}

bool Window::hasField(int field) {
	switch (field) {
	case kTheDrawRect:
	case kTheFileName:
	case kTheModal:
	case kTheRect:
	case kTheSourceRect:
	case kTheTitle:
	case kTheTitleVisible:
	case kTheVisible:
	case kTheWindowType:
		return true;
	default:
		break;
	}
	return false;
}

Datum Window::getField(int field) {
	switch (field) {
	case kTheTitle:
		return getTitle();
	case kTheTitleVisible:
		return isTitleVisible();
	case kTheVisible:
		return isVisible();
	case kTheWindowType:
		return getWindowType();
	case kTheRect:
		return getStageRect();
	case kTheModal:
		return getModal();
	case kTheFileName:
		return getFileName();
	case kTheDrawRect:
	case kTheSourceRect:
	// case kTheImage:
	// case kThePicture::
		ensureMovieIsLoaded();  // Remove fallthrough once implemented
		// fallthrough
	default:
		warning("Window::getField: unhandled field '%s'", g_lingo->field2str(field));
		return Datum();
	}
}

bool Window::setField(int field, const Datum &value) {
	switch (field) {
	case kTheTitle:
		setTitle(value.asString());
		return true;
	case kTheTitleVisible:
		setTitleVisible((bool)value.asInt());
		return true;
	case kTheVisible:
		setVisible((bool)value.asInt());
		return true;
	case kTheWindowType:
		setWindowType(value.asInt());
		return true;
	case kTheRect:
		return setStageRect(value);
	case kTheModal:
		setModal((bool)value.asInt());
		return true;
	case kTheFileName:
		setFileName(value.asString());
		return true;
	default:
		warning("Window::setField: unhandled field '%s'", g_lingo->field2str(field));
		return false;
	}
}

void LM::m_close(int nargs) {
	Window *me = static_cast<Window *>(g_lingo->_state->me.u.obj);
	me->setVisible(false);
}

void LM::m_forget(int nargs) {
	Window *me = static_cast<Window *>(g_lingo->_state->me.u.obj);
	FArray *windowList = g_lingo->_windowList.u.farr;

	int windowIndex = -1;
	for (int i = 0; i < (int)windowList->arr.size(); i++) {
		if (windowList->arr[i].type != OBJECT || windowList->arr[i].u.obj->getObjType() != kWindowObj)
			continue;

		Window *window = static_cast<Window *>(windowList->arr[i].u.obj);
		if (window == me) {
			windowIndex = i;
			break;
		}
	}

	if (windowIndex == -1) {
		warning("m_forget: me object %s not found in window list", g_lingo->_state->me.asString().c_str());
		return;
	}

	if (windowIndex < (int)windowList->arr.size())
		windowList->arr.remove_at(windowIndex);

	// remove me from global vars
	for (auto &it : g_lingo->_globalvars) {
		if (it._value.type != OBJECT || it._value.u.obj->getObjType() != kWindowObj)
			continue;

		if (it._value.u.obj == me)
			g_lingo->_globalvars[it._key] = 0;
	}

}

void LM::m_open(int nargs) {
	Window *me = static_cast<Window *>(g_lingo->_state->me.u.obj);
	me->setVisible(true);
}

void LM::m_moveToBack(int nargs) {
	g_lingo->printSTUBWithArglist("m_moveToBack", nargs);
	g_lingo->dropStack(nargs);
}

void LM::m_moveToFront(int nargs) {
	g_lingo->printSTUBWithArglist("m_moveToFront", nargs);

	Window *me = static_cast<Window *>(g_lingo->_state->me.u.obj);
	me->ensureMovieIsLoaded();
	g_lingo->dropStack(nargs);
}

} // End of namespace Director
