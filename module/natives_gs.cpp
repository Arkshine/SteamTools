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

	return static_cast<cell>(GameServer()->BSecure());
}

// native Steam_GetPublicIP(octets[4]);
static cell AMX_NATIVE_CALL Steam_GetPublicIP(AMX* amx, cell* params)
{
	CHECK_STEAMTOOLS();

	enum args { arg_count, arg_octects };

	auto ipaddr = GameServer()->GetPublicIP();
	auto output = MF_GetAmxAddr(amx, params[arg_octects]);

	for (char iter = 3; iter > -1; --iter)
	{
		output[(~iter) & 0x03] = (static_cast<byte>(ipaddr >> (iter * 8)) & 0xFF);
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
	return static_cast<cell>(g_SteamTools->IsSteamToolsLoaded());
}

// native Steam_SetGameDescription(const description[]);
static cell AMX_NATIVE_CALL Steam_SetGameDescription(AMX* amx, cell* params)
{
	CHECK_STEAMTOOLS();

	enum args { arg_count, arg_description };

	int length;
	auto description = MF_GetAmxString(amx, params[arg_description], 0, &length);

	if (static_cast<size_t>(length) >= k_cbMaxGameServerGameDescription)
	{
		description[k_cbMaxGameServerGameDescription] = '\0';
	}

	GameServer()->SetGameDescription(description);

	return 1;
}

// native Steam_SetMapName(const name[]);
static cell AMX_NATIVE_CALL Steam_SetMapName(AMX* amx, cell* params)
{
	CHECK_STEAMTOOLS();

	enum args { arg_count, arg_name };

	auto name = MF_GetAmxString(amx, params[arg_name], 0, nullptr);

	g_SteamTools->m_Hooks->SetCustomMapName(name);

	return 1;
}

// native Steam_SetServerName(const name[]);
static cell AMX_NATIVE_CALL Steam_SetServerName(AMX* amx, cell* params)
{
	CHECK_STEAMTOOLS();

	enum args { arg_count, arg_name };

	auto name = MF_GetAmxString(amx, params[arg_name], 0, nullptr);

	g_SteamTools->m_Hooks->SetCustomServerName(name);

	return 1;
}

// native bool:Steam_IsConnected();
static cell AMX_NATIVE_CALL Steam_IsConnected(AMX* amx, cell* params)
{
	CHECK_STEAMTOOLS();

	return static_cast<cell>(GameServer()->BLoggedOn());
}

// native Steam_SetRule(const key[], const value[]);
static cell AMX_NATIVE_CALL Steam_SetRule(AMX* amx, cell* params)
{
	CHECK_STEAMTOOLS();

	enum args { arg_count, arg_key, arg_value };

	auto key   = MF_GetAmxString(amx, params[arg_key]  , 0, nullptr);
	auto value = MF_GetAmxString(amx, params[arg_value], 1, nullptr);

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
	enum args { arg_count, arg_groupid, arg_steamid, arg_maxlength };

	char CSteamIDString[128];
	ke::SafeSprintf(CSteamIDString, sizeof(CSteamIDString), "%llu", CSteamID(params[arg_groupid], k_EUniversePublic, k_EAccountTypeClan).ConvertToUint64());

	return MF_SetAmxString(amx, params[arg_steamid], CSteamIDString, params[arg_maxlength]);
}

// native Steam_CSteamIDToGroupID(const steamID[]);
static cell AMX_NATIVE_CALL Steam_CSteamIDToGroupID(AMX* amx, cell* params)
{
	enum args { arg_count, arg_steamid };

	auto pSteamID = MF_GetAmxString(amx, params[arg_steamid], 0, nullptr);

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
	enum args { arg_count, arg_accountid };

	auto cSteamID = CSteamID(params[arg_accountid], k_EUniversePublic, k_EAccountTypeIndividual);

	if (!cSteamID.IsValid())
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "%d is not a valid account ID", params[arg_accountid]);
		return 0;
	}

	auto id = g_SteamTools->FindGameClient(cSteamID);

	if (id == -1)
	{
		return 0;
	}

	return id;
}

// native Steam_GetCSteamIDForClient(client, steamID[], maxlength);
static cell AMX_NATIVE_CALL Steam_GetCSteamIDForClient(AMX* amx, cell* params)
{
	enum args { arg_count, arg_client, arg_steamid, arg_maxlength };

	auto index = params[arg_client];
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

	return MF_SetAmxString(amx, params[arg_steamid], cSteamIDString, params[arg_maxlength]);
}

// native Steam_RenderedIDToCSteamID(const renderedID[], steamID[], maxlength);
static cell AMX_NATIVE_CALL Steam_RenderedIDToCSteamID(AMX* amx, cell* params)
{
	enum args { arg_count, arg_rendereid, arg_steamid, arg_maxlength };

	auto renderedSteamID = MF_GetAmxString(amx, params[arg_rendereid], 0, nullptr);

	auto cSteamID = g_SteamTools->RenderedIDToCSteamID(renderedSteamID);

	if (!cSteamID.IsValid())
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "%s is not a valid rendered SteamID", renderedSteamID);
		return 0;
	}

	char cSteamIDString[128];
	ke::SafeSprintf(cSteamIDString, sizeof(cSteamIDString), "%llu", cSteamID.ConvertToUint64());

	return MF_SetAmxString(amx, params[arg_steamid], cSteamIDString, params[arg_maxlength]);
}

// native Steam_CSteamIDToRenderedID(const steamID[], renderedID[], maxlength);
static cell AMX_NATIVE_CALL Steam_CSteamIDToRenderedID(AMX* amx, cell* params)
{
	enum args { arg_count, arg_steamid, arg_renderedid, arg_maxlength };

	const char* pcSteamID = MF_GetAmxString(amx, params[arg_steamid], 0, nullptr);

	CSteamID cSteamID(atoui64(pcSteamID));

	if (!cSteamID.IsValid())
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "%s is not a valid SteamID", pcSteamID);
		return 0;
	}

	char renderedSteamIDString[128];
	ke::SafeSprintf(renderedSteamIDString, sizeof(renderedSteamIDString), "%s", cSteamID.Render());

	return MF_SetAmxString(amx, params[arg_renderedid], renderedSteamIDString, params[arg_maxlength]);
}

// native bool:Steam_RequestGroupStatus(client, groupAccountID);
static cell AMX_NATIVE_CALL Steam_RequestGroupStatus(AMX* amx, cell* params)
{
	CHECK_STEAMTOOLS();

	enum args { arg_count, arg_client, arg_groupid };

	auto cSteamID = g_SteamTools->RenderedIDToCSteamID(GETPLAYERAUTHID(TypeConversion.id_to_edict(params[arg_client])));

	if (!cSteamID.IsValid())
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "No SteamID found for client %d", params[arg_client]);
		return 0;
	}

	return GameServer()->RequestUserGroupStatus(cSteamID, CSteamID(params[arg_groupid], k_EUniversePublic, k_EAccountTypeClan));
}

// native bool:Steam_RequestGroupStatusAuthID(authID, groupAccountID);
static cell AMX_NATIVE_CALL Steam_RequestGroupStatusAuthID(AMX* amx, cell* params)
{
	CHECK_STEAMTOOLS();

	enum args { arg_count, arg_authid, arg_groupid };

	auto cSteamID = CSteamID(params[arg_authid] , k_EUniversePublic, k_EAccountTypeIndividual);
	auto cGroupID = CSteamID(params[arg_groupid], k_EUniversePublic, k_EAccountTypeClan);

	return GameServer()->RequestUserGroupStatus(cSteamID, cGroupID);
}

// native EUserHasLicenseForAppResult:Steam_HasLicenseForApp(client);
static cell AMX_NATIVE_CALL Steam_HasLicenseForApp(AMX* amx, cell* params)
{
	CHECK_STEAMTOOLS();

	enum args { arg_count, arg_client };

	auto index = params[arg_client];

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
