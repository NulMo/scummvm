/* ScummVM - Scumm Interpreter
 * Copyright (C) 2002 The ScummVM project
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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * $Header$
 */

#include "stdafx.h"
#include "launcher.h"
#include "browser.h"
#include "chooser.h"
#include "newgui.h"
#include "message.h"
#include "ListWidget.h"

#include "backends/fs/fs.h"
#include "common/config-file.h"
#include "common/engine.h"
#include "common/gameDetector.h"

enum {
	kStartCmd = 'STRT',
	kOptionsCmd = 'OPTN',
	kAddGameCmd = 'ADDG',
	kEditGameCmd = 'EDTG',
	kRemoveGameCmd = 'REMG',
	kQuitCmd = 'QUIT'
};

typedef ScummVM::List<const VersionSettings *> GameList;

/*
 * TODO list
 * - add an text entry widget
 * - add an "Add Game..." button that opens a dialog where new games can be 
 *   configured and added to the list of games
 * - add an "Edit Game..." button that opens a dialog that allows to edit game
 *   settings, i.e. the datapath/savepath/sound driver/... for that game
 * - add an "options" dialog
 * - ...
 */

LauncherDialog::LauncherDialog(NewGui *gui, GameDetector &detector)
	: Dialog(gui, 0, 0, 320, 200), _detector(detector)
{
	// Show game name
	new StaticTextWidget(this, 10, 8, 300, kLineHeight,
								"ScummVM "SCUMMVM_VERSION " (" SCUMMVM_CVS ")", 
								kTextAlignCenter);

	// Add three buttons at the bottom
	addButton(1*(_w - kButtonWidth)/6, _h - 24, "Quit", kQuitCmd, 'Q');
	addButton(3*(_w - kButtonWidth)/6, _h - 24, "Options", kOptionsCmd, 'O');
	_startButton = addButton(5*(_w - kButtonWidth)/6, _h - 24, "Start", kStartCmd, 'S');
	_startButton->setEnabled(false);

	// Add list with game titles
	_list = new ListWidget(this, 10, 28, 300, 112);
	_list->setEditable(false);
	_list->setNumberingMode(kListNumberingOff);
	
	// Populate the list
	updateListing();

	// TODO - make a default selection (maybe the game user played last?)
	//_list->setSelected(0);

	// Two more buttons directly below the list box
	const int kBigButtonWidth = 90;
	new ButtonWidget(this, 10, 144, kBigButtonWidth, 16, "Add Game...", kAddGameCmd, 'A');
	_editButton = new ButtonWidget(this, (320-kBigButtonWidth)/2, 144, kBigButtonWidth, 16, "Edit Game...", kEditGameCmd, 'E');
	_editButton->setEnabled(false);
	_removeButton = new ButtonWidget(this, 320-kBigButtonWidth-10, 144, kBigButtonWidth, 16, "Remove Game", kRemoveGameCmd, 'R');
	_removeButton->setEnabled(false);
	
	// Create file browser dialog
	_browser = new BrowserDialog(_gui);
}

LauncherDialog::~LauncherDialog()
{
	delete _browser;
}

void LauncherDialog::updateListing()
{
	int i;
	const VersionSettings *v = version_settings;
	ScummVM::StringList l;
	// TODO - maybe only display those games for which settings are known
	// (i.e. a path to the game data was set and is accesible) ?


	// Retrieve a list of all games defined in the config file
	StringList domains = g_config->get_domains();
	for (i = 0; i < domains.size();i++) {
		String name = (char*)g_config->get("gameid", domains[i]);
		String description = (char*)g_config->get("description", domains[i]);
		
		if (name.isEmpty() || description.isEmpty()) {
			v = version_settings;
			while (v->filename && v->gamename) {
				if (!scumm_stricmp(v->filename, domains[i].c_str())) {
					name = domains[i];
					description = v->gamename;
					break;
				}
				v++;
			}
		} 

		if (!name.isEmpty() && !description.isEmpty()) {
			// Insert the game into the launcher list
			int pos = 0, size = l.size();

			while (pos < size && (description > l[pos]))
				pos++;
			l.insert_at(pos, description);
			_filenames.insert_at(pos, domains[i]);
		}
	}

	if (l.size() > 0) 
		_list->setList(l);
}

/*
 * Return a list of all games which might be the game in the specified directory.
 */
GameList findGame(FilesystemNode *dir)
{
	GameList list;

	FSList *files = dir->listDir(FilesystemNode::kListFilesOnly);
	const int size = files->size();
	char detectName[256];
	int i;

	// Iterate over all known games and for each check if it might be
	// the game in the presented directory.
	const VersionSettings *v = version_settings;
	while (v->filename && v->gamename) {

		// Determine the 'detectname' for this game, that is, the name of a 
		// file that *must* be presented if the directory contains the data
		// for this game. For example, FOA requires atlantis.000
		if (v->detectname)
			strcpy(detectName, v->detectname);
		else {
			strcpy(detectName, v->filename);
			if (v->features & GF_AFTER_V7)
				strcat(detectName, ".la0");
			else if (v->features & GF_HUMONGOUS)
				strcat(detectName, ".he0");
			else
				strcat(detectName, ".000");
		}

		// Iterate over all files in the given directory
		for (i = 0; i < size; i++) {
			const char *filename = (*files)[i].displayName().c_str();

			if (0 == scumm_stricmp(detectName, filename)) {
				// Match found, add to list of candidates, then abort inner loop.
				list.push_back(v);
				break;
			}
		}

		v++;
	}

	return list;
}

void LauncherDialog::handleCommand(CommandSender *sender, uint32 cmd, uint32 data)
{
	int item =  _list->getSelected();

	switch (cmd) {
	case kAddGameCmd: {
		// Allow user to add a new game to the list.
		// 1) show a dir selection dialog which lets 
		// the user pick the directory the game data resides in.
		// 2) show the user a list of games to pick from, already narrowed
		// down to all possible choices
		
		if (_browser->runModal()) {
			// User did make a choice...
			FilesystemNode *dir = _browser->getResult();
			
			// ...so let's determine a list of candidates, games that
			// could be contained in the specified directory.
			GameList candidates = findGame(dir);
			const VersionSettings *v = 0;
			
			if (candidates.isEmpty()) {
				// No game was found in the specified directory
				MessageDialog alert(_gui, "ScummVM could not find any game in the specified directory");
				alert.runModal();
			} else if (candidates.size() == 1) {
				// Exact match
				v = candidates[0];
			} else {
				// Display the candidates to the user and let her/him pick one
				StringList list;
				int i;
				for (i = 0; i < candidates.size(); i++)
					list.push_back(candidates[i]->gamename);
				
				ChooserDialog dialog(_gui, "Pick the game:", list);
				i = dialog.runModal();
				if (0 <= i && i < candidates.size())
					v = candidates[i];
			}
			
			if (v != 0) {
				// The auto detector or the user made a choice.
				// For now we just forcefully insert it into the config list, but
				// in the future we might want to check for an existing entry
				// first... of course the question is, what do we do if we find one?
				g_config->set("path", dir->path(), v->filename);
				
				// Write it to disk
				g_config->set_writing(true);
				g_config->flush();
				g_config->set_writing(false);
				
				// Update the ListWidget and force a redraw
				updateListing();
				draw();
			}
		}
		}
		break;
	case kRemoveGameCmd: {
		// TODO - remove the currently selected game from the list
		assert(item >= 0);
		MessageDialog alert(_gui, "'Remove game' dialog not yet implemented!");
		alert.runModal();
		}
		break;
	case kEditGameCmd: {
		// Set game specifc options. Most of these should be "optional", i.e. by 
		// default set nothing and use the global ScummVM settings. E.g. the user
		// can set here an optional alternate music volume, or for specific games
		// a different music driver etc.
		// This is useful because e.g. MonkeyVGA needs Adlib music to have decent
		// music support etc.
		assert(item >= 0);
		MessageDialog alert(_gui, "'Edit game' dialog not yet implemented!");
		alert.runModal();
		}
		break;
	case kOptionsCmd: {
		// TODO - show up a generic options dialog, loosely based upon the one 
		// we have in scumm/dialogs.cpp. So we will be modifying the settings
		// in _detector, like which music engine to use, volumes, etc.
		//
		// We also allow the global save game path to be set here.
		MessageDialog alert(_gui, "Global game options dialog not yet implemented!");
		alert.runModal();
		}
		break;
	case kStartCmd:
	case kListItemDoubleClickedCmd:
		// Print out what was selected
		assert(item >= 0);
		_detector.setGame(_filenames[item].c_str());
		close();
		break;
	case kListSelectionChangedCmd:
		_startButton->setEnabled(data >= 0);
		_startButton->draw();
		//_editButton->setEnabled(data >= 0);
		//_editButton->draw();
		_removeButton->setEnabled(data >= 0);
		_removeButton->draw();
		break;
	case kQuitCmd:
		g_system->quit();
		break;
	default:
		Dialog::handleCommand(sender, cmd, data);
	}
}
