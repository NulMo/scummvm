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

/*
 * Copyright (C) 2006-2010 - Frictional Games
 *
 * This file is part of HPL1 Engine.
 */

#include "hpl1/engine/impl/SqScript.h"
#include "hpl1/engine/impl/Platform.h"
#include "hpl1/engine/math/Math.h"
#include "hpl1/engine/system/LowLevelSystem.h"
#include "hpl1/engine/system/String.h"
#include "common/file.h"
#include "hpl1/debug.h"

namespace hpl {

//////////////////////////////////////////////////////////////////////////
// CONSTRUCTORS
//////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------

cSqScript::cSqScript(const tString &asName, asIScriptEngine *apScriptEngine,
					 cScriptOutput *apScriptOutput, int alHandle)
	: iScript(asName) {
	mpScriptEngine = apScriptEngine;
	mpScriptOutput = apScriptOutput;
	mlHandle = alHandle;

	mpContext = mpScriptEngine->CreateContext();

	// Create a unique module name
	msModuleName = "Module_" + cString::ToString(cMath::RandRectl(0, 1000000)) +
				   "_" + cString::ToString(mlHandle);
}

cSqScript::~cSqScript() {
#if 0
  		mpScriptEngine->Discard(msModuleName.c_str());
		mpContext->Release();
#endif
}

//-----------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS
//////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------

bool cSqScript::CreateFromFile(const tString &asFileName) {
#if 0
  		int lLength;
		char *pCharBuffer = LoadCharBuffer(asFileName,lLength);
		if(pCharBuffer==NULL){
			Error("Couldn't load script '%s'!\n",asFileName.c_str());
			return false;
		}

		if(mpScriptEngine->AddScriptSection(msModuleName.c_str(), "main", pCharBuffer, lLength)<0)
		{
			Error("Couldn't add script '%s'!\n",asFileName.c_str());
			hplDeleteArray(pCharBuffer);
			return false;
		}

		if(mpScriptEngine->Build(msModuleName.c_str())<0)
		{
			Error("Couldn't build script '%s'!\n",asFileName.c_str());
			Log("------- SCRIPT OUTPUT BEGIN --------------------------\n");
			mpScriptOutput->Display();
			mpScriptOutput->Clear();
			Log("------- SCRIPT OUTPUT END ----------------------------\n");



			hplDeleteArray(pCharBuffer);
			return false;
		}
		mpScriptOutput->Clear();

		hplDeleteArray(pCharBuffer);
		return true;
#endif
	return false;
}

//-----------------------------------------------------------------------

int cSqScript::GetFuncHandle(const tString &asFunc) {
#if 0
  		return mpScriptEngine->GetFunctionIDByName(msModuleName.c_str(),asFunc.c_str());
#endif
	return 0;
}

//-----------------------------------------------------------------------

void cSqScript::AddArg(const tString &asArg) {
}

//-----------------------------------------------------------------------

bool cSqScript::Run(const tString &asFuncLine) {
#if 0
  		mpScriptEngine->ExecuteString(msModuleName.c_str(), asFuncLine.c_str());

		return true;
#endif
	return false;
}

//-----------------------------------------------------------------------

bool cSqScript::Run(int alHandle) {
#if 0
  		mpContext->Prepare(alHandle);

		/* Set all the args here */

		mpContext->Execute();

		return true;
#endif
	return false;
}

//-----------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////
// PRIVATE METHODS
//////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------

char *cSqScript::LoadCharBuffer(const tString &asFileName, int &alLength) {
	Common::File file; 
	//FIXME: use proper string types
	file.open(asFileName.c_str());
	if(!file.isOpen()) {
		debugCN(Hpl1::kDebugLevelError, Hpl1::kDebugFilePath, 
			"script file at %s could not be opened", asFileName.c_str()); 
		return nullptr;
	}
	
	alLength = file.size(); 
	char *pBuffer = hplNewArray(char, alLength);
	file.read(pBuffer, alLength);
	if(file.err()) {
		debugCN(Hpl1::kDebugLevelError, Hpl1::kDebugResourceLoading, 
			"error in reading script file %s", asFileName.c_str()); 
		return nullptr; 
	}
	return pBuffer;
}

//-----------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////
// STATIC PRIVATE METHODS
//////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------

//-----------------------------------------------------------------------

} // namespace hpl
