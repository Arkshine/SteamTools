//
// AMX Mod X SteamTools
// Copyright (C) Vincent Herbet (Arkshine)
//
// This software is licensed under the GNU General Public License, version 2.
// For full license details, see LICENSE file.
//

#include "sw_detours.h"
#include "steamtools.h"

#include <MemoryUtils.h>
#include <CDetour/detours.h>

#if defined(KE_WINDOWS)
	typedef HMODULE SysType;
#else
	typedef void* SysType;
#endif

extern "C" void SteamAPIWarningMessageHook(int hpipe, const char *message)
{
	MF_Log("SteamAPIWarning: %s", message);
}

DETOUR_DECL_STATIC6(Hook_SteamGameServer_Init, bool, uint32, unIP, uint16, usPort, uint16, usGamePort, uint16, usQueryPort, EServerMode, eServerMode, const char*, pchVersionString)
{
	auto result = DETOUR_STATIC_CALL(Hook_SteamGameServer_Init)(unIP, usPort, usGamePort, usQueryPort, eServerMode, pchVersionString);

	if (result && g_SteamTools->m_GameServer->GetSteamClient())
	{
		g_SteamTools->m_GameServer->GetUtils()->SetWarningMessageHook(SteamAPIWarningMessageHook);
		g_SteamTools->OnAPIActivated();
	}

	g_SteamTools->m_GameServer->RemoveInterfaceHooks();

	return result;
}

DETOUR_DECL_STATIC2(Hook_SteamAPI_Init_Internal, void*, SysType*, handle, bool, checkInstance)
{
	auto steamclient = DETOUR_STATIC_CALL(Hook_SteamAPI_Init_Internal)(handle, checkInstance);

	if (steamclient)
	{
		auto library = SimpleLib(*handle);

		g_SteamTools->m_GameServer->SetSteamClient(reinterpret_cast<ISteamClient*>(steamclient));
		g_SteamTools->m_GameServer->SetCallbackFuncs(library.lookup("Steam_BGetCallback"), library.lookup("Steam_FreeLastCallback"));
		g_SteamTools->m_GameServer->AddInterfaceHooks();
		g_SteamTools->m_GameServer->AddCallbackHook();
	}
	else
	{
		MF_PrintSrvConsole("[STEAMTOOLS] Failed to get steamclient interface\n");
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
#if defined(KE_WINDOWS)

	auto steamAPILibrary = "steam_api.dll";
	#define STEAMAPI_INIT_INTERNAL "\\x55\\x8B\\x2A\\x81\\x2A\\x2A\\x2A\\x2A\\x2A\\x53\\x56\\x8B"

#elif defined(KE_LINUX)

	auto steamAPILibrary = "libsteam_api.so";
	#define STEAMAPI_INIT_INTERNAL "\\x55\\x89\\x2A\\x57\\x56\\x53\\x81\\x2A\\x2A\\x2A\\x2A\\x2A\\xE8\\x2A\\x2A\\x2A\\x2A\\x81\\x2A\\x2A\\x2A\\x2A\\x2A\\x8B\\x2A\\x2A\\x0F"

#elif defined(KE_MACOSX)

	auto steamAPILibrary = "libsteam_api.dylib";
	#define STEAMAPI_INIT_INTERNAL "_Z22SteamAPI_Init_InternalPPvb"

#endif

	char error[255] = "unknown";
	auto lib = ke::SharedLib::Open(steamAPILibrary, error, sizeof(error));

	if (lib->valid())
	{
	#if defined(KE_MACOSX)
		auto addressInitSteamclientFn = g_MemUtils.ResolveSymbol(lib->lookup("SteamGameServer_Init"), STEAMAPI_INIT_INTERNAL);
	#else
		auto addressInitSteamclientFn = g_MemUtils.DecodeAndFindPattern(lib->lookup("SteamGameServer_Init"), STEAMAPI_INIT_INTERNAL);
	#endif

		if (!addressInitSteamclientFn)
		{
			MF_PrintSrvConsole("[STEAMTOOLS] Could not get address of function which inits ISteamClient interface\n");
			return;
		}

		g_SteamTools->m_GameServer->SetUserAndPipe(lib->lookup("SteamGameServer_GetHSteamUser"), lib->lookup("SteamGameServer_GetHSteamPipe"));

		m_InitSteamClientDetour    = DETOUR_CREATE_STATIC_FIXED(Hook_SteamAPI_Init_Internal  , addressInitSteamclientFn);
		m_InitGameServerDetour     = DETOUR_CREATE_STATIC_FIXED(Hook_SteamGameServer_Init    , lib->lookup("SteamGameServer_Init"));
		m_ShutdownGameServerDetour = DETOUR_CREATE_STATIC_FIXED(Hook_SteamGameServer_Shutdown, lib->lookup("SteamGameServer_Shutdown"));

		m_InitGameServerDetour->EnableDetour();
		m_ShutdownGameServerDetour->EnableDetour();
		m_InitSteamClientDetour->EnableDetour();
	}
	else
	{
		MF_PrintSrvConsole("[STEAMTOOLS] Failed to open \"%s\". Error: %s\n", steamAPILibrary, error);
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
