//
// AMX Mod X SteamTools
// Copyright (C) Vincent Herbet (Arkshine)
//
// This software is licensed under the GNU General Public License, version 2.
// For full license details, see LICENSE file.
//

#include "natives_gs.h"
#include "steamtools.h"
#include "utils.h"

// native bool:Steam_IsVACEnabled();
static cell AMX_NATIVE_CALL Steam_IsVACEnabled(AMX* amx, cell* params)
{
	ISteamGameServer* pServer = g_SteamTools->m_GameServer->GetGameServer();

	if (!pServer)
	{
		return 0;
	}

	return pServer->BSecure() ? 1 : 0;
}

// native Steam_GetPublicIP(octets[4]);
static cell AMX_NATIVE_CALL Steam_GetPublicIP(AMX* amx, cell* params)
{
	ISteamGameServer* pServer = g_SteamTools->m_GameServer->GetGameServer();

	if (!pServer)
	{
		return 0;
	}

	uint32_t ipaddr = pServer->GetPublicIP();

	cell* addr = MF_GetAmxAddr(amx, params[1]);

	for (char iter = 3; iter > -1; --iter)
	{
		addr[(~iter) & 0x03] = (static_cast<unsigned char>(ipaddr >> (iter * 8)) & 0xFF);
	}

	return 1;
}

// native bool:Steam_IsLoaded();
static cell AMX_NATIVE_CALL Steam_IsLoaded(AMX* amx, cell* params)
{
	return g_SteamTools->IsSteamToolsLoaded() ? 1 : 0;
}

// native Steam_SetGameDescription(const description[]);
static cell AMX_NATIVE_CALL Steam_SetGameDescription(AMX* amx, cell* params)
{
	ISteamGameServer* pServer = g_SteamTools->m_GameServer->GetGameServer();

	if (!pServer)
	{
		return 0;
	}

	int length;
	const char* description = MF_GetAmxString(amx, params[1], 0, &length);

	pServer->SetGameDescription(description);

	return 1;
}

// native bool:Steam_IsConnected();
static cell AMX_NATIVE_CALL Steam_IsConnected(AMX* amx, cell* params)
{
	ISteamGameServer* pServer = g_SteamTools->m_GameServer->GetGameServer();

	if (!pServer)
	{
		return 0;
	}

	return pServer->BLoggedOn() ? 1 : 0;
}

// native Steam_SetRule(const key[], const value[]);
static cell AMX_NATIVE_CALL Steam_SetRule(AMX* amx, cell* params)
{
	ISteamGameServer* pServer = g_SteamTools->m_GameServer->GetGameServer();

	if (!pServer)
	{
		return 0;
	}

	int length;
	const char* key   = MF_GetAmxString(amx, params[1], 0, &length);
	const char* value = MF_GetAmxString(amx, params[2], 1, &length);

	pServer->SetKeyValue(key, value);

	return 1;
}

// native Steam_ClearRules();
static cell AMX_NATIVE_CALL Steam_ClearRules(AMX* amx, cell* params)
{
	ISteamGameServer* pServer = g_SteamTools->m_GameServer->GetGameServer();

	if (!pServer)
	{
		return 0;
	}

	pServer->ClearAllKeyValues();

	return 1;
}

// native Steam_ForceHeartbeat();
static cell AMX_NATIVE_CALL Steam_ForceHeartbeat(AMX* amx, cell* params)
{
	ISteamGameServer* pServer = g_SteamTools->m_GameServer->GetGameServer();

	if (!pServer)
	{
		return 0;
	}

	pServer->ForceHeartbeat();

	return 1;
}

// native Steam_GroupIDToCSteamID(groupID, steamID[], maxlength);
static cell AMX_NATIVE_CALL Steam_GroupIDToCSteamID(AMX* amx, cell* params)
{
	char CSteamIDString[128];
	UTIL_Format(CSteamIDString, params[3], "%llu", CSteamID(params[1], k_EUniversePublic, k_EAccountTypeClan).ConvertToUint64());

	return MF_SetAmxString(amx, params[2], CSteamIDString, params[3]);
}

// native Steam_CSteamIDToGroupID(const steamID[]);
static cell AMX_NATIVE_CALL Steam_CSteamIDToGroupID(AMX* amx, cell* params)
{
	int length;
	const char* pSteamID = MF_GetAmxString(amx, params[1], 0, &length);

	CSteamID steamID(atoui64(pSteamID));

	if (!steamID.IsValid())
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "%s is not a valid SteamID", pSteamID);
		return 0;
	}

	return steamID.GetAccountID();
}

// native Steam_GetCSteamIDForClient(client, steamID[], maxlength);
static cell AMX_NATIVE_CALL Steam_GetCSteamIDForClient(AMX* amx, cell* params)
{
	int index = params[1];

	CSteamID cSteamID = k_steamIDNil;

	if (index >= 0)
	{
		cSteamID = Utils::RenderedIDToCSteamID(GETPLAYERAUTHID(Utils::INDEXENT2(index)));
	}

	if (!cSteamID.IsValid())
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "%s is not a valid SteamID", cSteamID);
		return 0;
	}

	char CSteamIDString[128];
	UTIL_Format(CSteamIDString, params[3], "%llu", cSteamID.ConvertToUint64());

	return MF_SetAmxString(amx, params[2], CSteamIDString, params[3]);
}

// native Steam_RenderedIDToCSteamID(const renderedID[], steamID[], maxlength);
static cell AMX_NATIVE_CALL Steam_RenderedIDToCSteamID(AMX* amx, cell* params)
{
	int length;
	const char* renderedSteamID = MF_GetAmxString(amx, params[1], 0, &length);

	CSteamID cSteamID = Utils::RenderedIDToCSteamID(renderedSteamID);

	if (!cSteamID.IsValid())
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "%s is not a valid rendered SteamID", renderedSteamID);
		return 0;
	}

	char CSteamIDString[128];
	UTIL_Format(CSteamIDString, params[3], "%llu", cSteamID.ConvertToUint64());

	return MF_SetAmxString(amx, params[2], CSteamIDString, params[3]);
}

// native Steam_CSteamIDToRenderedID(const steamID[], renderedID[], maxlength);
static cell AMX_NATIVE_CALL Steam_CSteamIDToRenderedID(AMX* amx, cell* params)
{
	int length;
	const char* pcSteamID = MF_GetAmxString(amx, params[1], 0, &length);

	CSteamID cSteamID(atoui64(pcSteamID));

	if (!cSteamID.IsValid())
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "%s is not a valid SteamID", pcSteamID);
		return 0;
	}

	char renderedSteamIDString[128];
	UTIL_Format(renderedSteamIDString, params[3], "%s", cSteamID.Render());

	return MF_SetAmxString(amx, params[2], renderedSteamIDString, params[3]);
}

// native bool:Steam_RequestGroupStatus(client, groupAccountID);
static cell AMX_NATIVE_CALL Steam_RequestGroupStatus(AMX* amx, cell* params)
{
	ISteamGameServer* pServer = g_SteamTools->m_GameServer->GetGameServer();

	if (!pServer)
	{
		return 0;
	}

	CSteamID pSteamID = Utils::RenderedIDToCSteamID(GETPLAYERAUTHID(Utils::INDEXENT2(params[1])));

	if (!pSteamID.IsValid())
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "No SteamID found for client %d", params[1]);
		return 0;
	}

	return pServer->RequestUserGroupStatus(pSteamID, CSteamID(params[2], k_EUniversePublic, k_EAccountTypeClan));
}

AMX_NATIVE_INFO GameServerNatives[] =
{
	{ "Steam_IsVACEnabled"        , Steam_IsVACEnabled            },
	{ "Steam_GetPublicIP"         , Steam_GetPublicIP             },
	{ "Steam_IsLoaded"            , Steam_IsLoaded                },
	{ "Steam_SetGameDescription"  , Steam_SetGameDescription      },
	{ "Steam_IsConnected"         , Steam_IsConnected             },
	{ "Steam_SetRule"             , Steam_SetRule                 },
	{ "Steam_ClearRules"          , Steam_ClearRules              },
	{ "Steam_ForceHeartbeat"      , Steam_ForceHeartbeat          },
	{ "Steam_GroupIDToCSteamID"   , Steam_GroupIDToCSteamID       },
	{ "Steam_CSteamIDToGroupID"   , Steam_CSteamIDToGroupID       },
	{ "Steam_GetCSteamIDForClient", Steam_GetCSteamIDForClient    },
	{ "Steam_RenderedIDToCSteamID", Steam_RenderedIDToCSteamID    },
	{ "Steam_CSteamIDToRenderedID", Steam_CSteamIDToRenderedID    },
	{ "Steam_RequestGroupStatus"  , Steam_RequestGroupStatus      },

	{ NULL, NULL }
};

SteamWorksGSNatives::SteamWorksGSNatives()
{
	MF_AddNatives(GameServerNatives);
}

SteamWorksGSNatives::~SteamWorksGSNatives()
{
}
