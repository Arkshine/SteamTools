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
