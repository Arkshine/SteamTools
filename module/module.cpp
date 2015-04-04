
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
	g_pFunctionTable_Post->pfnStartFrame = OnStartFrame;

	g_SteamTools->m_Forwards->OnSteamFullyLoaded();
	g_SteamTools->m_Hooks->AddHooks();
	
	SERVER_PRINT("[STEAMTOOLS] Loading complete.\n");
}

void OnSteamAPIShutdown()
{
	g_pFunctionTable_Post->pfnStartFrame = nullptr;

	g_SteamTools->m_Forwards->OnSteamShutdown();
	g_SteamTools->m_Hooks->RemoveHooks();
}

void OnStartFrame()
{
	g_SteamTools->Think();

	RETURN_META(MRES_IGNORED);
}
