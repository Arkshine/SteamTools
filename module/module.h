//
// AMX Mod X SteamTools
// Copyright (C) Vincent Herbet (Arkshine)
//
// This software is licensed under the GNU General Public License, version 2.
// For full license details, see LICENSE file.
//

#ifndef _MODULE_H_
#define _MODULE_H_

#include <sourcehook/sourcehook.h>
#include "interfaces.h"

extern SourceHook::ISourceHook *g_SHPtr;
extern int g_PLID;

void OnSteamAPIActivated();
void OnSteamAPIShutdown();
void OnStartFrame();

#endif // _MODULE_H_
