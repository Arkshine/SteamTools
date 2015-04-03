
#include "module.h"
#include "steamtools.h"
#include <sourcehook/sourcehook_impl.h>

SourceHook::CSourceHookImpl g_SourceHook;
SourceHook::ISourceHook *g_SHPtr = &g_SourceHook;
int g_PLID = 0;

extern DLL_FUNCTIONS* g_pFunctionTable_Post;

void OnAmxxAttach()
{
	g_SteamTools = new SteamTools;

	g_SteamTools->Init();
	g_SteamTools->GetState(&OnSteamAPIActivated);
}

void OnPluginsLoaded()
{
	g_SteamTools->m_Forwards->RegisterForwards();
	g_SteamTools->m_Hooks->RegisterForwards();
}

void OnSteamAPIActivated()
{
	g_pFunctionTable_Post->pfnStartFrame = OnStartFrame;

	MF_Log("[STEAMTOOLS] Loading complete.\n");
}

void OnStartFrame()
{
	g_SteamTools->Think();

	RETURN_META(MRES_IGNORED);
}

CSteamID atocsteamid(const char *pRenderedID)
{
	// Convert the Steam2 ID string to a Steam2 ID structure
	TSteamGlobalUserID steam2ID;
	steam2ID.m_SteamInstanceID = 0;
	steam2ID.m_SteamLocalUserID.Split.High32bits = 0;
	steam2ID.m_SteamLocalUserID.Split.Low32bits = 0;

	const char *pchTSteam2ID = pRenderedID;
	const char *pchOptionalLeadString = "STEAM_";

	if (strncasecmp(pRenderedID, pchOptionalLeadString, strlen(pchOptionalLeadString)) == 0)
	{
		pchTSteam2ID = pRenderedID + strlen(pchOptionalLeadString);
	}

	char cExtraCharCheck = 0;

	int cFieldConverted = sscanf(pchTSteam2ID, "%hu:%u:%u%c", &steam2ID.m_SteamInstanceID, &steam2ID.m_SteamLocalUserID.Split.High32bits, &steam2ID.m_SteamLocalUserID.Split.Low32bits, &cExtraCharCheck);

	// Validate the conversion ... a special case is steam2 instance ID 1 which is reserved for special DoD handling
	if (cExtraCharCheck != 0 || cFieldConverted == EOF || cFieldConverted < 2 || (cFieldConverted < 3 && steam2ID.m_SteamInstanceID != 1))
	{
		return k_steamIDNil;
	}

	// Now convert to steam ID from the Steam2 ID structure
	CSteamID steamID;
	steamID.SetFromSteam2(&steam2ID, k_EUniversePublic);
	return steamID;
}