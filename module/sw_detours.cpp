
#include "sw_detours.h"
#include "module.h"
#include "steamtools.h"
#include <CDetour/detours.h>

#if defined(WIN32)
#  include "Win32Library.h"
#elif defined(LINUX) || defined(OSX)
#  include "POSIXLibrary.h"
#endif

DETOUR_DECL_STATIC8(Hook_SteamGameServer_Init, bool, uint32, unIP, uint16, usPort, uint16, usGamePort, uint16, usSpectatorPort, uint16, usQueryPort, EServerMode, eServerMode, const char*, pchGameDir, const char*, pchVersionString)
{
	g_SteamTools->m_GameServer->AddHooks();

	bool result = DETOUR_STATIC_CALL(Hook_SteamGameServer_Init)(unIP, usPort, usGamePort, usSpectatorPort, usQueryPort, eServerMode, pchGameDir, pchVersionString);

	if (result)
	{
		g_SteamTools->OnAPIActivated();
	}

	g_SteamTools->m_GameServer->RemoveHooks();

	return result;
}

DETOUR_DECL_STATIC0(Hook_SteamGameServer_Shutdown, void)
{
	g_SteamTools->OnAPIShutdown();

	DETOUR_STATIC_CALL(Hook_SteamGameServer_Shutdown)();
}

SteamToolsGSDetours::SteamToolsGSDetours() : m_InitDetour(nullptr), m_ShutdownDetour(nullptr)
{
	ke::AutoPtr<DynamicLibrary> lib;

#if defined(WIN32)
	const char* steamClientLibrary = "steamclient.dll";
	const char* steamAPILibrary    = "steam_api.dll";
#elif defined(LINUX)
	const char* steamClientLibrary = "steamclient.so";
	const char* steamAPILibrary    = "libsteam_api.so";
#elif defined(OSX)
	const char* steamClientLibrary = "steamclient.dylib";
	const char* steamAPILibrary    = "libsteam_api.dylib";
#endif

	lib = new DynamicLibrary(steamClientLibrary);

	if (lib->IsLoaded())
	{
		CreateInterfaceFn iface   = reinterpret_cast<CreateInterfaceFn>(lib->GetSymbol("CreateInterface"));
		ISteamClient* steamclient = reinterpret_cast<ISteamClient*>(iface(STEAMCLIENT_INTERFACE_VERSION, nullptr));

		if (!steamclient)
		{
			SERVER_PRINT("[STEAMTOOLS] Failed to get steamclient interface\n");
			return;
		}

		g_SteamTools->m_GameServer->SetSteamClient(steamclient);
		g_SteamTools->m_GameServer->SetCallbackFuncs(lib->GetSymbol("Steam_BGetCallback"), lib->GetSymbol("Steam_FreeLastCallback"));
	}

	lib = new DynamicLibrary(steamAPILibrary);

	if (lib->IsLoaded())
	{
		g_SteamTools->m_GameServer->SetUserAndPipe(lib->GetSymbol("SteamGameServer_GetHSteamUser"), lib->GetSymbol("SteamGameServer_GetHSteamPipe"));

		m_InitDetour     = DETOUR_CREATE_STATIC_FIXED(Hook_SteamGameServer_Init    , lib->GetSymbol("SteamGameServer_Init"));
		m_ShutdownDetour = DETOUR_CREATE_STATIC_FIXED(Hook_SteamGameServer_Shutdown, lib->GetSymbol("SteamGameServer_Shutdown"));

		m_InitDetour->EnableDetour();
		m_ShutdownDetour->EnableDetour();
	}
}

SteamToolsGSDetours::~SteamToolsGSDetours()
{
	if (m_InitDetour)
	{
		m_InitDetour->Destroy();
	}

	if (m_ShutdownDetour)
	{
		m_ShutdownDetour->Destroy();
	}
}
