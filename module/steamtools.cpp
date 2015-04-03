
#include "steamtools.h"

ke::AutoPtr<SteamTools> g_SteamTools;

SteamTools::SteamTools() : m_LastThinkTime(0.0f), m_Loaded(false), m_Callback(nullptr)
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

void SteamTools::GetState(void* func)
{
	m_Callback = reinterpret_cast<void(*)()>(func);
}

void SteamTools::OnAPIActivated()
{
	m_Loaded = true;

	m_Callback();
}

void SteamTools::Think()
{
// 	if (m_LastThinkTime > gpGlobals->time)
// 	{
// 		RETURN_META(MRES_IGNORED);
// 	}

	m_GameServer->Think();

//	m_LastThinkTime = gpGlobals->time + 0.05f;

	RETURN_META(MRES_IGNORED);
}

bool SteamTools::IsSteamToolsLoaded()
{
	return m_Loaded;
}
