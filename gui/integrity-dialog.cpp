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

#include "gui/integrity-dialog.h"

#include "common/array.h"
#include "common/config-manager.h"
#include "common/debug.h"
#include "common/file.h"
#include "common/macresman.h"
#include "common/md5.h"
#include "common/translation.h"

#include "gui/gui-manager.h"
#include "gui/launcher.h"
#include "gui/message.h"
#include "gui/widget.h"

namespace GUI {

enum {
	kCleanupCmd = 'DlCL',
};

struct DialogState {
	IntegrityDialog *dialog;
	IconProcessState state;

	int totalSize;
	int calculatedSize;

	Common::String endpoint;
	Common::Path gamePath;
	Common::String gameid;
	Common::String engineid;
	Common::String extra;
	Common::String platform;
	Common::String language;

	DialogState() {
		state = kChecksumStateNone;
		totalSize = calculatedSize = 0;
		dialog = nullptr;
	}
} static *g_state;

uint32 getDownloadingProgress() {
	if (!g_state || g_state->totalSize == 0)
		return 0;

	uint32 progress = (uint32)(100 * ((double)g_state->calculatedSize / (double)g_state->totalSize));

	return progress;
}

IntegrityDialog::IntegrityDialog(Common::String endpoint, Common::String domain) : Dialog("GameOptions_IntegrityDialog"), _close(false) {

	_backgroundType = GUI::ThemeEngine::kDialogBackgroundPlain;

	_statusText = new StaticTextWidget(this, "GameOptions_IntegrityDialog.StatusText", Common::U32String::format(_("Calculating file checksums...")));
	_errorText = new StaticTextWidget(this, "GameOptions_IntegrityDialog.ErrorText", Common::U32String(""));

	uint32 progress = getDownloadingProgress();
	_progressBar = new SliderWidget(this, "GameOptions_IntegrityDialog.ProgressBar");
	_progressBar->setMinValue(0);
	_progressBar->setMaxValue(100);
	_progressBar->setValue(progress);
	_progressBar->setEnabled(false);
	_percentLabel = new StaticTextWidget(this, "GameOptions_IntegrityDialog.PercentText", Common::String::format("%u %%", progress));
	_cancelButton = new ButtonWidget(this, "GameOptions_IntegrityDialog.MainButton", _("Cancel"), Common::U32String(), kCleanupCmd);

	MessageDialog alert(Common::U32String("Verifying file integrity may take a long time to complete.\nAre you sure you want to continue?"), "OK", "Cancel");
	int result = alert.runModal();
	if (result == 1)
		return;

	if (!g_state) {
		g_state = new DialogState();
		g_state->dialog = this;

		setState(kChecksumStateCalculating);
		refreshWidgets();

		g_state->endpoint = endpoint;
		g_state->gamePath = Common::Path(ConfMan.get("path", domain));
		g_state->gameid = ConfMan.get("gameid", domain);
		g_state->engineid = ConfMan.get("engineid", domain);
		g_state->extra = ConfMan.get("extra", domain);
		g_state->platform = ConfMan.get("platform", domain);
		g_state->language = ConfMan.get("language", domain);
		calculateTotalSize(g_state->gamePath);

		sendJSON();
	} else {
		g_state->dialog = this;

		setState(g_state->state);
		refreshWidgets();
	}
}

IntegrityDialog::~IntegrityDialog() {
}

void IntegrityDialog::open() {
	Dialog::open();
	reflowLayout();
	g_gui.scheduleTopDialogRedraw();
}

void IntegrityDialog::close() {
	if (g_state)
		g_state->dialog = nullptr;

	Dialog::close();
}

void IntegrityDialog::setState(IconProcessState state) {
	g_state->state = state;

	switch (state) {
	case kChecksumStateNone:
	case kChecksumStateCalculating:
		_statusText->setLabel(Common::U32String::format(_("Calculating file checksums...")));
		_cancelButton->setLabel(_("Cancel"));
		_cancelButton->setCmd(kCleanupCmd);
		break;

	case kChecksumComplete:
		_statusText->setLabel(Common::U32String::format(_("Calculation complete")));
		_cancelButton->setVisible(false);
		_cancelButton->setLabel(_("Cancel"));
		_cancelButton->setCmd(kCleanupCmd);

		break;
	}
}

void IntegrityDialog::handleCommand(CommandSender *sender, uint32 cmd, uint32 data) {
	switch (cmd) {
	case kCleanupCmd: {
		delete g_state;
		g_state = nullptr;

		close();
		break;
	}
	default:
		Dialog::handleCommand(sender, cmd, data);
	}
}

void IntegrityDialog::handleTickle() {
	if (_close) {
		close();
		_close = false;
		return;
	}

	int32 progress = getDownloadingProgress();
	if (_progressBar->getValue() != progress) {
		refreshWidgets();
		g_gui.scheduleTopDialogRedraw();
	}

	Dialog::handleTickle();
}

void IntegrityDialog::reflowLayout() {
	Dialog::reflowLayout();
	refreshWidgets();
}

void IntegrityDialog::refreshWidgets() {
	uint32 progress = getDownloadingProgress();
	_percentLabel->setLabel(Common::String::format("%u %%", progress));
	_progressBar->setValue(progress);
}

void IntegrityDialog::setError(Common::U32String &msg) {
	_errorText->setLabel(msg);

	_cancelButton->setLabel(_("Close"));
	_cancelButton->setCmd(kCleanupCmd);
}

void IntegrityDialog::calculateTotalSize(Common::Path gamePath) {
	const Common::FSNode dir(gamePath);

	if (!dir.exists() || !dir.isDirectory())
		return;

	Common::FSList fileList;
	if (!dir.getChildren(fileList, Common::FSNode::kListAll))
		return;

	if (fileList.empty())
		return;

	// Process the files and subdirectories in the current directory recursively
	for (Common::FSList::const_iterator it = fileList.begin(); it != fileList.end(); it++) {
		const Common::FSNode &entry = *it;

		if (entry.isDirectory())
			calculateTotalSize(entry.getPath());
		else {
			Common::File file;
			if (!file.open(entry))
				continue;

			g_state->totalSize += file.size();
		}
	}
}

Common::Array<Common::StringArray> IntegrityDialog::generateChecksums(Common::Path gamePath, Common::Array<Common::StringArray> &fileChecksums) {
	const Common::FSNode dir(gamePath);

	if (!dir.exists() || !dir.isDirectory())
		return {};

	Common::FSList fileList;
	if (!dir.getChildren(fileList, Common::FSNode::kListAll))
		return {};

	if (fileList.empty())
		return {};

	// Process the files and subdirectories in the current directory recursively
	for (Common::FSList::const_iterator it = fileList.begin(); it != fileList.end(); it++) {
		const Common::FSNode &entry = *it;

		if (entry.isDirectory())
			generateChecksums(entry.getPath(), fileChecksums);
		else {
			const Common::Path filename(entry.getPath());
			auto macFile = Common::MacResManager();
			if (macFile.openFileOrDataFork(filename)) {
				auto originalStream = macFile.openFileOrDataFork(filename);
				auto fileStream = originalStream;

				Common::Array<Common::String> fileChecksum = {filename.toString()};
				// Various checksizes
				for (auto size : {0, 5000, 1024 * 1024}) {
					fileChecksum.push_back(Common::computeStreamMD5AsString(*(fileStream), size).c_str());
					fileStream->seek(0);
				}
				// Tail checksums with checksize 5000
				fileStream->seek(-5000, SEEK_END);
				fileChecksum.push_back(Common::computeStreamMD5AsString(*(fileStream)).c_str());

				g_state->calculatedSize += fileStream->size();

				fileChecksums.push_back(fileChecksum);
			}

			Common::File file;
			if (!file.open(entry))
				continue;

			Common::Array<Common::String> fileChecksum = {filename.toString()};
			// Various checksizes
			for (auto size : {0, 5000, 1024 * 1024}) {
				fileChecksum.push_back(Common::computeStreamMD5AsString(file, size).c_str());
				file.seek(0);
			}
			// Tail checksums with checksize 5000
			file.seek(-5000, SEEK_END);
			fileChecksum.push_back(Common::computeStreamMD5AsString(file).c_str());

			g_state->calculatedSize += file.size();

			file.close();
			fileChecksums.push_back(fileChecksum);
		}
	}

	return fileChecksums;
}

Common::JSONValue *IntegrityDialog::generateJSONRequest(Common::Path gamePath, Common::String gameid, Common::String engineid, Common::String extra, Common::String platform, Common::String language) {
	Common::Array<Common::StringArray> fileChecksums = {};
	fileChecksums = generateChecksums(gamePath, fileChecksums);
	Common::JSONObject requestObject;

	requestObject.setVal("gameid", new Common::JSONValue(gameid));
	requestObject.setVal("engineid", new Common::JSONValue(engineid));
	requestObject.setVal("extra", new Common::JSONValue(extra));
	requestObject.setVal("platform", new Common::JSONValue(platform));
	requestObject.setVal("language", new Common::JSONValue(language));

	Common::JSONArray filesObject;

	for (Common::StringArray fileChecksum : fileChecksums) {
		Common::JSONObject file;
		Common::Path relativePath = Common::Path(fileChecksum[0]).relativeTo(gamePath);
		file.setVal("name", new Common::JSONValue(relativePath.toConfig()));

		auto tempNode = Common::FSNode(Common::Path(fileChecksum[0]));
		Common::File tempFile;
		if (!tempFile.open(tempNode))
			continue;
		uint64 fileSize = tempFile.size();
		tempFile.close();

		file.setVal("size", new Common::JSONValue((long long)fileSize));

		Common::JSONArray checksums;
		Common::StringArray checkcodes = {"md5", "md5-5000", "md5-1M", "md5-t-5000"};

		int index = -1;
		for (Common::String val : fileChecksum) {
			index++;

			Common::JSONObject checksum;
			if (index < 1) {
				continue;
			}

			checksum.setVal("type", new Common::JSONValue(checkcodes[index - 1]));
			checksum.setVal("checksum", new Common::JSONValue(val));

			checksums.push_back(new Common::JSONValue(checksum));
		}
		file.setVal("checksums", new Common::JSONValue(checksums));

		filesObject.push_back(new Common::JSONValue(file));
	}

	requestObject.setVal("files", new Common::JSONValue(filesObject));

	Common::JSONValue *request = new Common::JSONValue(requestObject);
	return request;
}

void IntegrityDialog::checksumResponseCallback(const Common::JSONValue *r) {
	debug(3, "JSON Response: %s", r->stringify().c_str());
	Common::String messageText = IntegrityDialog::parseJSON(r);
	MessageDialog result(messageText);
	result.reflowLayout();
}

void IntegrityDialog::errorCallback(const Networking::ErrorResponse &error) {
	warning("ERROR %ld: %s", error.httpResponseCode, error.response.c_str());
}

void IntegrityDialog::sendJSON() {
	auto conn = new Networking::PostRequest(g_state->endpoint,
		new Common::Callback<IntegrityDialog, const Common::JSONValue *>(this, &IntegrityDialog::checksumResponseCallback),
		new Common::Callback<IntegrityDialog, const Networking::ErrorResponse &>(this, &IntegrityDialog::errorCallback));

	Common::JSONValue *json = generateJSONRequest(
		g_state->gamePath, g_state->gameid, g_state->engineid, g_state->extra, g_state->platform, g_state->language);
	conn->setJSONData(json);
	conn->setContentType("application/json");
	conn->start();
	delete json;
}

Common::String IntegrityDialog::parseJSON(const Common::JSONValue *response) {
	Common::String messageText;

	Common::JSONObject responseObject = response->asObject();
	int responeError = responseObject.getVal("error")->asIntegerNumber();

	if (responeError == -1) { // Unknown variant
		long long fileset = responseObject.getVal("fileset")->asIntegerNumber();
		messageText =
			_(Common::String::format("Your set of game files seems to be unknown to us. If you are sure that this is a valid unknown variant, please send the following e-mail to integrity@scummvm.org \n\
		The game of fileset %lld seems to be an unknown game variant. \n\
		The details of the game : %s, %s, %s, %s, %s",
								   fileset, g_state->engineid.c_str(), g_state->gameid.c_str(), g_state->platform.c_str(), g_state->language.c_str(), g_state->extra.c_str()));

		return messageText;
	}

	Common::Array<int> results = Common::Array<int>(5, 0);
	for (Common::JSONValue *fileJSON : responseObject.getVal("files")->asArray()) {
		Common::String name = fileJSON->asObject().getVal("name")->asString();
		Common::String status = fileJSON->asObject().getVal("status")->asString();

		if (status == "ok") {
			results[OK]++;
			continue;
		} else if (status == "missing")
			results[MISSING]++;
		else if (status == "checksum_mismatch")
			results[CHECKSUM_MISMATCH]++;
		else if (status == "size_mismatch")
			results[SIZE_MISMATCH]++;
		else if (status == "unknown")
			results[UNKNOWN]++;

		messageText += Common::String::format("%s %s\n", name.c_str(), status.c_str());
	}
	if (messageText == "")
		messageText += _("Files all OK");

	return messageText;
}

} // End of namespace GUI
