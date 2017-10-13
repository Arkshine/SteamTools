//
// AMX Mod X SteamTools
// Copyright (C) Vincent Herbet (Arkshine)
//
// This software is licensed under the GNU General Public License, version 2.
// For full license details, see LICENSE file.
//

#ifndef _STEAMTOOLS_MODULE_H_
#define _STEAMTOOLS_MODULE_H_

#include <sourcehook/sourcehook.h>
#include "interfaces.h"
#include "HLTypeConversion.h"

extern SourceHook::ISourceHook *g_SHPtr;
extern int g_PLID;

extern HLTypeConversion TypeConversion;;

void OnSteamAPIActivated();
void OnSteamAPIShutdown();

#endif // _STEAMTOOLS_MODULE_H_
