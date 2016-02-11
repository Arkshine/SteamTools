//
// AMX Mod X SteamTools
// Copyright (C) Vincent Herbet (Arkshine)
//
// This software is licensed under the GNU General Public License, version 2.
// For full license details, see LICENSE file.
//

#include "sw_detours.h"
#include "module.h"
#include "steamtools.h"
#include <MemoryUtils.h>
#include <CDetour/detours.h>

#if defined(WIN32)

	#include "Win32Library.h"
	typedef HMODULE LibraryHandle;

#elif defined(LINUX) || defined(OSX)

	#include "POSIXLibrary.h"
	typedef void*   LibraryHandle;

#endif

DETOUR_DECL_STATIC8(Hook_SteamGameServer_Init, bool, uint32, unIP, uint16, usPort, uint16, usGamePort, uint16, usSpectatorPort, uint16, usQueryPort, EServerMode, eServerMode, const char*, pchGameDir, const char*, pchVersionString)
{
	bool result = DETOUR_STATIC_CALL(Hook_SteamGameServer_Init)(unIP, usPort, usGamePort, usSpectatorPort, usQueryPort, eServerMode, pchGameDir, pchVersionString);

	if (result && g_SteamTools->m_GameServer->GetSteamClient())
	{
		g_SteamTools->OnAPIActivated();
	}

	g_SteamTools->m_GameServer->RemoveInterfaceHooks();

	return result;
}

DETOUR_DECL_STATIC2(Hook_SteamAPI_Init_Internal, void*, LibraryHandle*, handle, bool, checkInstance)
{
	void* steamclient = DETOUR_STATIC_CALL(Hook_SteamAPI_Init_Internal)(handle, checkInstance);

	if (steamclient)
	{
		g_SteamTools->m_GameServer->SetSteamClient(reinterpret_cast<ISteamClient*>(steamclient));
		g_SteamTools->m_GameServer->SetCallbackFuncs(g_MemUtils.ResolveSymbol(*handle, "Steam_BGetCallback"), g_MemUtils.ResolveSymbol(*handle, "Steam_FreeLastCallback"));
		g_SteamTools->m_GameServer->AddInterfaceHooks();
		g_SteamTools->m_GameServer->AddCallbackHook();
	}
	else
	{
		SERVER_PRINT("[STEAMTOOLS] Failed to get steamclient interface\n");
	}

	return steamclient;
}


DETOUR_DECL_STATIC0(Hook_SteamGameServer_Shutdown, void)
{
	g_SteamTools->OnAPIShutdown();

	DETOUR_STATIC_CALL(Hook_SteamGameServer_Shutdown)();
}

SteamToolsGSDetours::SteamToolsGSDetours() : m_InitGameServerDetour(nullptr), m_ShutdownGameServerDetour(nullptr), m_InitSteamClientDetour(nullptr)
{
	ke::AutoPtr<DynamicLibrary> lib;

#if defined(WIN32)

	const char* steamAPILibrary = "steam_api.dll";
	#define STEAMAPI_INIT_INTERNAL "\\x55\\x8B\\x2A\\x81\\x2A\\x2A\\x2A\\x2A\\x2A\\x53\\x56\\x8B"

#elif defined(LINUX)

	const char* steamAPILibrary = "libsteam_api.so";
	#define STEAMAPI_INIT_INTERNAL "\\x55\\x89\\x2A\\x57\\x56\\x53\\x81\\x2A\\x2A\\x2A\\x2A\\x2A\\xE8\\x2A\\x2A\\x2A\\x2A\\x81\\x2A\\x2A\\x2A\\x2A\\x2A\\x8B\\x2A\\x2A\\x0F"

#elif defined(OSX)

	const char* steamAPILibrary = "libsteam_api.dylib";
	#define STEAMAPI_INIT_INTERNAL "_Z22SteamAPI_Init_InternalPPvb"

#endif

	lib = new DynamicLibrary(steamAPILibrary);

	if (lib->IsLoaded())
	{
	#if defined(OSX)
		void* addressInitSteamclientFn = g_MemUtils.ResolveSymbol(lib->GetSymbol("SteamGameServer_Init"), STEAMAPI_INIT_INTERNAL);
	#else
		void* addressInitSteamclientFn = g_MemUtils.DecodeAndFindPattern(lib->GetSymbol("SteamGameServer_Init"), STEAMAPI_INIT_INTERNAL);
	#endif
		
		if (!addressInitSteamclientFn)
		{	
			SERVER_PRINT("[STEAMTOOLS] Could not get address of function which inits ISteamClient interface\n");
			return;
		}

		g_SteamTools->m_GameServer->SetUserAndPipe(lib->GetSymbol("SteamGameServer_GetHSteamUser"), lib->GetSymbol("SteamGameServer_GetHSteamPipe"));

		m_InitSteamClientDetour    = DETOUR_CREATE_STATIC_FIXED(Hook_SteamAPI_Init_Internal  , addressInitSteamclientFn);
		m_InitGameServerDetour     = DETOUR_CREATE_STATIC_FIXED(Hook_SteamGameServer_Init    , lib->GetSymbol("SteamGameServer_Init"));
		m_ShutdownGameServerDetour = DETOUR_CREATE_STATIC_FIXED(Hook_SteamGameServer_Shutdown, lib->GetSymbol("SteamGameServer_Shutdown"));

		m_InitGameServerDetour->EnableDetour();
		m_ShutdownGameServerDetour->EnableDetour();
		m_InitSteamClientDetour->EnableDetour();
	}
	else
	{
		SERVER_PRINT(UTIL_VarArgs("[STEAMTOOLS] Failed to open \"%s\"\n", steamAPILibrary));
	}
}

SteamToolsGSDetours::~SteamToolsGSDetours()
{
	if (m_InitGameServerDetour)
	{
		m_InitGameServerDetour->Destroy();
		m_InitGameServerDetour = nullptr;
	}

	if (m_ShutdownGameServerDetour)
	{
		m_ShutdownGameServerDetour->Destroy();
		m_ShutdownGameServerDetour = nullptr;
	}

	if (m_InitSteamClientDetour)
	{
		m_InitSteamClientDetour->Destroy();
		m_InitSteamClientDetour = nullptr;
	}
}
