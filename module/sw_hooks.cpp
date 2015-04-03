
#include "sw_hooks.h"
#include "steamtools.h"
#include "module.h"
#include <amxxmodule.h>

cvar_t CvarInitSetSteamAccount = { "sv_setsteamaccount", "", FCVAR_EXTDLL | FCVAR_PROTECTED | FCVAR_NOEXTRAWHITEPACE | FCVAR_SPONLY };
cvar_t* CvarSetSteamAccount;

SH_DECL_HOOK0     (ISteamGameServer, WasRestartRequested, SH_NOATTRIB, 0, bool);
SH_DECL_HOOK3     (ISteamGameServer, BeginAuthSession   , SH_NOATTRIB, 0, EBeginAuthSessionResult, const void*, int, CSteamID); 
SH_DECL_HOOK0_void(ISteamGameServer, LogOnAnonymous     , SH_NOATTRIB, 0); 

SteamToolsGSHooks::SteamToolsGSHooks() 
	: 
	m_ForwardRestartRequested(0), 
	m_ForwardTokenRequested(0), 
	m_ForwardBeginAuthSession(0)
{
	CVAR_REGISTER(&CvarInitSetSteamAccount);
	CvarSetSteamAccount = CVAR_GET_POINTER(CvarInitSetSteamAccount.name);

	SERVER_COMMAND("exec steamtools.cfg\n");
	SERVER_EXECUTE();
}

SteamToolsGSHooks::~SteamToolsGSHooks()
{
	RemoveHooks();
}

void SteamToolsGSHooks::RegisterForwards()
{
	m_ForwardRestartRequested = MF_RegisterForward("Steam_RestartRequested", ET_STOP, FP_DONE);
	m_ForwardTokenRequested   = MF_RegisterForward("Steam_TokenRequested"  , ET_IGNORE, FP_ARRAY, FP_CELL, FP_DONE);
	m_ForwardBeginAuthSession = MF_RegisterForward("Steam_BeginAuthSession", ET_IGNORE, FP_ARRAY, FP_CELL, FP_CELL, FP_DONE);
}

bool SteamToolsGSHooks::WasRestartRequested()
{
	bool wasRestartRequested = SH_CALL(g_SteamTools->m_GameServer->GetGameServer(), &ISteamGameServer::WasRestartRequested)();

	if (wasRestartRequested && UTIL_CheckForPublic("Steam_RestartRequested"))
	{
		wasRestartRequested = MF_ExecuteForward(m_ForwardRestartRequested) > 0;
	}

	SH_RETURN_META_VALUE(SH_MRES_SUPERCEDE, wasRestartRequested);
}

void SteamToolsGSHooks::LogOnAnonymous(void)
{
	ISteamGameServer *pGameServer = g_SteamTools->m_GameServer->GetGameServer();

	if (!pGameServer)
	{
		SH_RETURN_META(SH_MRES_SUPERCEDE);
	}

// 	if (!UTIL_CheckForPublic("Steam_TokenRequested"))
// 	{
// 		SH_RETURN_META(SH_MRES_IGNORED);
// 	}
// 
// 	char token[256];
// 	MF_ExecuteForward(m_ForwardTokenRequested, MF_PrepareCharArrayA(token, sizeof(token), true), sizeof(token) - 1);

	pGameServer->LogOn(CvarSetSteamAccount->string);

	SH_RETURN_META(SH_MRES_SUPERCEDE);
}

EBeginAuthSessionResult SteamToolsGSHooks::BeginAuthSession(const void *pAuthTicket, int cbAuthTicket, CSteamID steamID)
{
	if (UTIL_CheckForPublic("Steam_BeginAuthSession"))
	{
		char* AuthTicketString = reinterpret_cast<char*>(const_cast<void*>(pAuthTicket));

		MF_ExecuteForward(m_ForwardBeginAuthSession, MF_PrepareCharArray(AuthTicketString, cbAuthTicket), cbAuthTicket, steamID.GetAccountID());
	}

	SH_RETURN_META_VALUE(SH_MRES_IGNORED, k_EBeginAuthSessionResultOK);
}

void SteamToolsGSHooks::AddHooks()
{
	ISteamGameServer* pGameServer = g_SteamTools->m_GameServer->GetGameServer();

	m_RestartHookID = SH_ADD_HOOK(ISteamGameServer, WasRestartRequested, pGameServer, SH_MEMBER(this, &SteamToolsGSHooks::WasRestartRequested), false);
	m_logAnonHookID = SH_ADD_HOOK(ISteamGameServer, LogOnAnonymous     , pGameServer, SH_MEMBER(this, &SteamToolsGSHooks::LogOnAnonymous)     , false);
//	m_SessionHookID = SH_ADD_HOOK(ISteamGameServer, BeginAuthSession   , pGameServer, SH_MEMBER(this, &SteamToolsGSHooks::BeginAuthSession)   , false);
}

void SteamToolsGSHooks::RemoveHooks()
{
	SH_REMOVE_HOOK_ID(m_RestartHookID);
	SH_REMOVE_HOOK_ID(m_logAnonHookID);
//	SH_REMOVE_HOOK_ID(m_SessionHookID);
}
