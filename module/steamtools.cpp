//
// AMX Mod X SteamTools
// Copyright (C) Vincent Herbet (Arkshine)
//
// This software is licensed under the GNU General Public License, version 2.
// For full license details, see LICENSE file.
//

#include "steamtools.h"

UniquePtr<SteamTools> g_SteamTools;

void SteamTools::Init()
{
	m_GameServer = MakeUnique<SteamToolsGameServer>();
	m_Detours    = MakeUnique<SteamToolsGSDetours>();
	m_Forwards   = MakeUnique<SteamToolsGSForwards>();
	m_Hooks      = MakeUnique<SteamToolsGSHooks>();
	m_Natives    = MakeUnique<SteamToolsGSNatives>();
}

void SteamTools::RequestState(void(*APIActivatedFunc)(), void(*APIShutdownFunc)())
{
	m_APiActivatedCallback = APIActivatedFunc;
	m_APiShutdownCallback  = APIShutdownFunc;
}

void SteamTools::OnAPIActivated()
{
	m_Loaded = true;

	m_APiActivatedCallback();
}

void SteamTools::OnAPIShutdown()
{
	m_Loaded = false;

	m_APiShutdownCallback();
}

bool SteamTools::IsSteamToolsLoaded()
{
	return m_Loaded;
}

int SteamTools::FindGameClient(CSteamID &target)
{
	int id;

	for (id = 1; id <= gpGlobals->maxClients; ++id)
	{
		if (MF_IsPlayerAuthorized(id) && !MF_IsPlayerBot(id) && !MF_IsPlayerHLTV(id))
		{
			if (g_SteamTools->RenderedIDToCSteamID(GETPLAYERAUTHID(MF_GetPlayerEdict(id))) == target)
			{
				break;
			}
		}
	}

	if (id > gpGlobals->maxClients)
	{
		id = -1;
	}

	return id;
}

CSteamID SteamTools::RenderedIDToCSteamID(const char* pRenderedID)
{
	TSteamGlobalUserID steam2ID;
	steam2ID.m_SteamInstanceID = 0;
	steam2ID.m_SteamLocalUserID.Split.High32bits = 0;
	steam2ID.m_SteamLocalUserID.Split.Low32bits  = 0;

	auto pchTSteam2ID = pRenderedID;
	auto pchOptionalLeadString = "STEAM_";

	if (strncasecmp(pRenderedID, pchOptionalLeadString, strlen(pchOptionalLeadString)) == 0)
	{
		pchTSteam2ID = pRenderedID + strlen(pchOptionalLeadString);
	}

	char cExtraCharCheck = 0;
	auto cFieldConverted = sscanf(pchTSteam2ID, "%hu:%u:%u%c", &steam2ID.m_SteamInstanceID, &steam2ID.m_SteamLocalUserID.Split.High32bits, &steam2ID.m_SteamLocalUserID.Split.Low32bits, &cExtraCharCheck);

	if (cExtraCharCheck != 0 || cFieldConverted == EOF || cFieldConverted < 2 || (cFieldConverted < 3 && steam2ID.m_SteamInstanceID != 1))
	{
		return k_steamIDNil;
	}

	CSteamID steamID;
	steamID.SetFromSteam2(&steam2ID, k_EUniversePublic);

	return steamID;
}