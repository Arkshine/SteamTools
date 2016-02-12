//
// AMX Mod X SteamTools
// Copyright (C) Vincent Herbet (Arkshine)
//
// This software is licensed under the GNU General Public License, version 2.
// For full license details, see LICENSE file.
//

#include "module.h"
#include "steamtools.h"
#include <sourcehook/sourcehook_impl.h>

SourceHook::CSourceHookImpl g_SourceHook;
SourceHook::ISourceHook *g_SHPtr = &g_SourceHook;
int g_PLID = 0;

extern DLL_FUNCTIONS* g_pFunctionTable_Post;

void OnAmxxAttach()
{
	g_SteamTools = new SteamTools;

	g_SteamTools->Init();
	g_SteamTools->RequestState(OnSteamAPIActivated, OnSteamAPIShutdown);
}

void OnPluginsLoaded()
{
	g_SteamTools->m_Forwards->RegisterForwards();
	g_SteamTools->m_Hooks->RegisterForwards();
}

void OnSteamAPIActivated()
{
	g_SteamTools->m_Forwards->OnSteamFullyLoaded();
	g_SteamTools->m_Hooks->AddHooks();
	
	SERVER_PRINT("[STEAMTOOLS] Loading complete.\n");
}

void OnSteamAPIShutdown()
{
	g_SteamTools->m_Forwards->OnSteamShutdown();
	g_SteamTools->m_Hooks->RemoveHooks();
}
