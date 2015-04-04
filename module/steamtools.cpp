//
// AMX Mod X SteamTools
// Copyright (C) Vincent Herbet (Arkshine)
//
// This software is licensed under the GNU General Public License, version 2.
// For full license details, see LICENSE file.
//

#include "steamtools.h"

ke::AutoPtr<SteamTools> g_SteamTools;

SteamTools::SteamTools() 
	: 
	m_Detours   (nullptr),
	m_GameServer(nullptr),
	m_Forwards  (nullptr),
	m_Hooks     (nullptr),
	m_Natives   (nullptr),

	m_APiActivatedCallback(nullptr), 
	m_APiShutdownCallback(nullptr),

	m_LastThinkTime(0.0f), 
	m_Loaded(false)
{
}

SteamTools::~SteamTools()
{
	delete m_GameServer;
	delete m_Detours;
	delete m_Forwards;
	delete m_Hooks;
	delete m_Natives;
}

void SteamTools::Init()
{
	m_GameServer = new SteamToolsGameServer;
	m_Detours    = new SteamToolsGSDetours;
	m_Forwards   = new SteamToolsGSForwards;
	m_Hooks      = new SteamToolsGSHooks;
	m_Natives    = new SteamWorksGSNatives;
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

void SteamTools::Think()
{
	m_GameServer->Think();
}

bool SteamTools::IsSteamToolsLoaded()
{
	return m_Loaded;
}

CSteamID SteamTools::RenderedIDToCSteamID(const char* pRenderedID)
{
	TSteamGlobalUserID steam2ID;
	steam2ID.m_SteamInstanceID = 0;
	steam2ID.m_SteamLocalUserID.Split.High32bits = 0;
	steam2ID.m_SteamLocalUserID.Split.Low32bits  = 0;

	const char *pchTSteam2ID = pRenderedID;
	const char *pchOptionalLeadString = "STEAM_";

	if (strncasecmp(pRenderedID, pchOptionalLeadString, strlen(pchOptionalLeadString)) == 0)
	{
		pchTSteam2ID = pRenderedID + strlen(pchOptionalLeadString);
	}

	char cExtraCharCheck = 0;

	int cFieldConverted = sscanf(pchTSteam2ID, "%hu:%u:%u%c", &steam2ID.m_SteamInstanceID, &steam2ID.m_SteamLocalUserID.Split.High32bits, &steam2ID.m_SteamLocalUserID.Split.Low32bits, &cExtraCharCheck);

	if (cExtraCharCheck != 0 || cFieldConverted == EOF || cFieldConverted < 2 || (cFieldConverted < 3 && steam2ID.m_SteamInstanceID != 1))
	{
		return k_steamIDNil;
	}

	CSteamID steamID;
	steamID.SetFromSteam2(&steam2ID, k_EUniversePublic);

	return steamID;
}