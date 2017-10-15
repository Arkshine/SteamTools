//
// AMX Mod X SteamTools
// Copyright (C) Vincent Herbet (Arkshine)
//
// This software is licensed under the GNU General Public License, version 2.
// For full license details, see LICENSE file.
//

#include "module.h"
#include "natives_gs.h"
#include "steamtools.h"
#include <amtl/am-string.h>

#define CHECK_STEAMTOOLS()                                                    \
	if (!g_SteamTools->IsSteamToolsLoaded())                                  \
	{                                                                         \
		MF_LogError(amx, AMX_ERR_NATIVE, "SteamTools API is not ready yet."); \
		return 0;                                                             \
	}

static ISteamGameServer* GameServer()
{
	return g_SteamTools->m_GameServer->GetGameServer();
}

static ISteamUtils* GameUtils()
{
	return g_SteamTools->m_GameServer->GetUtils();
}

// native bool:Steam_IsVACEnabled();
static cell AMX_NATIVE_CALL Steam_IsVACEnabled(AMX* amx, cell* params)
{
	CHECK_STEAMTOOLS();

	return GameServer()->BSecure() ? 1 : 0;
}

// native Steam_GetPublicIP(octets[4]);
static cell AMX_NATIVE_CALL Steam_GetPublicIP(AMX* amx, cell* params)
{
	CHECK_STEAMTOOLS();

	auto ipaddr = GameServer()->GetPublicIP();
	auto addr = MF_GetAmxAddr(amx, params[1]);

	for (char iter = 3; iter > -1; --iter)
	{
		addr[(~iter) & 0x03] = (static_cast<unsigned char>(ipaddr >> (iter * 8)) & 0xFF);
	}

	return 1;
}

// native Steam_GetPublicIPCell();
static cell AMX_NATIVE_CALL Steam_GetPublicIPCell(AMX* amx, cell* params)
{
	CHECK_STEAMTOOLS();

	return GameServer()->GetPublicIP();
}

// native bool:Steam_IsLoaded();
static cell AMX_NATIVE_CALL Steam_IsLoaded(AMX* amx, cell* params)
{
	return g_SteamTools->IsSteamToolsLoaded() ? 1 : 0;
}

// native Steam_SetGameDescription(const description[]);
static cell AMX_NATIVE_CALL Steam_SetGameDescription(AMX* amx, cell* params)
{
	CHECK_STEAMTOOLS();

	int length;
	const char* description = MF_GetAmxString(amx, params[1], 0, &length);

	GameServer()->SetGameDescription(description);

	return 1;
}

// native Steam_SetMapName(const name[]);
static cell AMX_NATIVE_CALL Steam_SetMapName(AMX* amx, cell* params)
{
	CHECK_STEAMTOOLS();

	int length;
	auto name = MF_GetAmxString(amx, params[1], 0, &length);

	g_SteamTools->m_Hooks->SetCustomMapName(name);

	return 1;
}

// native Steam_SetServerName(const name[]);
static cell AMX_NATIVE_CALL Steam_SetServerName(AMX* amx, cell* params)
{
	CHECK_STEAMTOOLS();

	int length;
	auto name = MF_GetAmxString(amx, params[1], 0, &length);

	g_SteamTools->m_Hooks->SetCustomServerName(name);

	return 1;
}

// native bool:Steam_IsConnected();
static cell AMX_NATIVE_CALL Steam_IsConnected(AMX* amx, cell* params)
{
	CHECK_STEAMTOOLS();

	return GameServer()->BLoggedOn() ? 1 : 0;
}

// native Steam_SetRule(const key[], const value[]);
static cell AMX_NATIVE_CALL Steam_SetRule(AMX* amx, cell* params)
{
	CHECK_STEAMTOOLS();

	int length;
	const char* key   = MF_GetAmxString(amx, params[1], 0, &length);
	const char* value = MF_GetAmxString(amx, params[2], 1, &length);

	GameServer()->SetKeyValue(key, value);

	return 1;
}

// native Steam_ClearRules();
static cell AMX_NATIVE_CALL Steam_ClearRules(AMX* amx, cell* params)
{
	CHECK_STEAMTOOLS();

	GameServer()->ClearAllKeyValues();

	return 1;
}

// native Steam_ForceHeartbeat();
static cell AMX_NATIVE_CALL Steam_ForceHeartbeat(AMX* amx, cell* params)
{
	CHECK_STEAMTOOLS();

	GameServer()->ForceHeartbeat();

	return 1;
}

// native Steam_GroupIDToCSteamID(groupID, steamID[], maxlength);
static cell AMX_NATIVE_CALL Steam_GroupIDToCSteamID(AMX* amx, cell* params)
{
	char CSteamIDString[128];
	ke::SafeSprintf(CSteamIDString, sizeof(CSteamIDString), "%llu", CSteamID(params[1], k_EUniversePublic, k_EAccountTypeClan).ConvertToUint64());

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

// native Steam_AccountIDToPlayerID(accountID);
static cell AMX_NATIVE_CALL Steam_AccountIDToPlayerID(AMX* amx, cell* params)
{
	auto cSteamID = CSteamID(params[1], k_EUniversePublic, k_EAccountTypeIndividual);

	if (!cSteamID.IsValid())
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "%d is not a valid account ID", params[1]);
		return 0;
	}

	for (auto id = 1; id <= gpGlobals->maxClients; ++id)
	{
		if (MF_IsPlayerAuthorized(id) && !MF_IsPlayerBot(id) && !MF_IsPlayerHLTV(id))
		{
			if (g_SteamTools->RenderedIDToCSteamID(GETPLAYERAUTHID(TypeConversion.id_to_edict(id))) == cSteamID)
			{
				return id;
			}
		}
	}

	return 0;
}

// native Steam_GetCSteamIDForClient(client, steamID[], maxlength);
static cell AMX_NATIVE_CALL Steam_GetCSteamIDForClient(AMX* amx, cell* params)
{
	int index = params[1];

	auto cSteamID = k_steamIDNil;

	if (index >= 0)
	{
		cSteamID = g_SteamTools->RenderedIDToCSteamID(GETPLAYERAUTHID(TypeConversion.id_to_edict(index)));
	}

	if (!cSteamID.IsValid())
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "%s is not a valid SteamID", cSteamID);
		return 0;
	}

	char cSteamIDString[128];
	ke::SafeSprintf(cSteamIDString, sizeof(cSteamIDString), "%llu", cSteamID.ConvertToUint64());

	return MF_SetAmxString(amx, params[2], cSteamIDString, params[3]);
}

// native Steam_RenderedIDToCSteamID(const renderedID[], steamID[], maxlength);
static cell AMX_NATIVE_CALL Steam_RenderedIDToCSteamID(AMX* amx, cell* params)
{
	int length;
	const char* renderedSteamID = MF_GetAmxString(amx, params[1], 0, &length);

	auto cSteamID = g_SteamTools->RenderedIDToCSteamID(renderedSteamID);

	if (!cSteamID.IsValid())
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "%s is not a valid rendered SteamID", renderedSteamID);
		return 0;
	}

	char cSteamIDString[128];
	ke::SafeSprintf(cSteamIDString, sizeof(cSteamIDString), "%llu", cSteamID.ConvertToUint64());

	return MF_SetAmxString(amx, params[2], cSteamIDString, params[3]);
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
	ke::SafeSprintf(renderedSteamIDString, sizeof(renderedSteamIDString), "%s", cSteamID.Render());

	return MF_SetAmxString(amx, params[2], renderedSteamIDString, params[3]);
}

// native bool:Steam_RequestGroupStatus(client, groupAccountID);
static cell AMX_NATIVE_CALL Steam_RequestGroupStatus(AMX* amx, cell* params)
{
	CHECK_STEAMTOOLS();

	auto cSteamID = g_SteamTools->RenderedIDToCSteamID(GETPLAYERAUTHID(TypeConversion.id_to_edict(params[1])));
	auto cGroupID = CSteamID(params[2], k_EUniversePublic, k_EAccountTypeClan);

	if (!cSteamID.IsValid())
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "No SteamID found for client %d", params[1]);
		return 0;
	}

	return GameServer()->RequestUserGroupStatus(cSteamID, cGroupID);
}

// native bool:Steam_RequestGroupStatusAuthID(authID, groupAccountID);
static cell AMX_NATIVE_CALL Steam_RequestGroupStatusAuthID(AMX* amx, cell* params)
{
	CHECK_STEAMTOOLS();

	auto cSteamID = CSteamID(params[1], k_EUniversePublic, k_EAccountTypeIndividual);
	auto cGroupID = CSteamID(params[2], k_EUniversePublic, k_EAccountTypeClan);

	return GameServer()->RequestUserGroupStatus(cSteamID, cGroupID);
}

// native EUserHasLicenseForAppResult:Steam_HasLicenseForApp(client);
static cell AMX_NATIVE_CALL Steam_HasLicenseForApp(AMX* amx, cell* params)
{
	CHECK_STEAMTOOLS();

	auto index = params[1];

	if (!MF_IsPlayerIngame(index))
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "Client index %d is invalid", index);
		return 0;
	}

	auto cSteamID = g_SteamTools->RenderedIDToCSteamID(GETPLAYERAUTHID(TypeConversion.id_to_edict(index)));

	if (!cSteamID.IsValid())
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "No SteamID found for client %d", params[1]);
		return 0;
	}

	auto appID = GameUtils()->GetAppID();

	if (appID == k_uAppIdInvalid)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "No App ID could not be retrieved");
		return 0;
	}

	return GameServer()->UserHasLicenseForApp(cSteamID, appID);
}


AMX_NATIVE_INFO GameServerNatives[] =
{
	{ "Steam_IsVACEnabled"            , Steam_IsVACEnabled             },
	{ "Steam_GetPublicIP"             , Steam_GetPublicIP              },
	{ "Steam_GetPublicIPCell"         , Steam_GetPublicIPCell          },
	{ "Steam_IsLoaded"                , Steam_IsLoaded                 },
	{ "Steam_SetGameDescription"      , Steam_SetGameDescription       },
	{ "Steam_SetMapName"              , Steam_SetMapName               },
	{ "Steam_SetServerName"           , Steam_SetServerName            },
	{ "Steam_IsConnected"             , Steam_IsConnected              },
	{ "Steam_SetRule"                 , Steam_SetRule                  },
	{ "Steam_ClearRules"              , Steam_ClearRules               },
	{ "Steam_ForceHeartbeat"          , Steam_ForceHeartbeat           },
	{ "Steam_GroupIDToCSteamID"       , Steam_GroupIDToCSteamID        },
	{ "Steam_CSteamIDToGroupID"       , Steam_CSteamIDToGroupID        },
	{ "Steam_AccountIDToPlayerID"     , Steam_AccountIDToPlayerID      },
	{ "Steam_GetCSteamIDForClient"    , Steam_GetCSteamIDForClient     },
	{ "Steam_RenderedIDToCSteamID"    , Steam_RenderedIDToCSteamID     },
	{ "Steam_CSteamIDToRenderedID"    , Steam_CSteamIDToRenderedID     },
	{ "Steam_RequestGroupStatus"      , Steam_RequestGroupStatus       },
	{ "Steam_RequestGroupStatusAuthID", Steam_RequestGroupStatusAuthID },
	{ "Steam_HasLicenseForApp"        , Steam_HasLicenseForApp         },
	{ nullptr                         , nullptr                        }
};

SteamToolsGSNatives::SteamToolsGSNatives()
{
	MF_AddNatives(GameServerNatives);
}

SteamToolsGSNatives::~SteamToolsGSNatives()
{
}
