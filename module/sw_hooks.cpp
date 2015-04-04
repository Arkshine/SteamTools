//
// AMX Mod X SteamTools
// Copyright (C) Vincent Herbet (Arkshine)
//
// This software is licensed under the GNU General Public License, version 2.
// For full license details, see LICENSE file.
//

#include "sw_hooks.h"
#include "steamtools.h"
#include "module.h"
#include <amxxmodule.h>

cvar_t CvarInitSetSteamAccount = { "sv_setsteamaccount", "", FCVAR_EXTDLL | FCVAR_PROTECTED | FCVAR_NOEXTRAWHITEPACE | FCVAR_SPONLY };
cvar_t* CvarSetSteamAccount;

SH_DECL_HOOK0     (ISteamGameServer, WasRestartRequested, SH_NOATTRIB, 0, bool);
SH_DECL_HOOK0_void(ISteamGameServer, LogOnAnonymous     , SH_NOATTRIB, 0); 

SteamToolsGSHooks::SteamToolsGSHooks() 
	: 
	m_ForwardRestartRequested(0), 
	m_ForwardTokenRequested(0),
	m_RestartHookID(0),
	m_logAnonHookID(0)
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
}

bool SteamToolsGSHooks::WasRestartRequested()
{
	bool wasRestartRequested = SH_CALL(g_SteamTools->m_GameServer->GetGameServer(), &ISteamGameServer::WasRestartRequested)();

	if (wasRestartRequested)
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

	if (!strlen(CvarSetSteamAccount->string))
	{
		SH_RETURN_META(SH_MRES_IGNORED);
	}

	pGameServer->LogOn(CvarSetSteamAccount->string);

	SH_RETURN_META(SH_MRES_SUPERCEDE);
}

void SteamToolsGSHooks::AddHooks()
{
	ISteamGameServer* pGameServer = g_SteamTools->m_GameServer->GetGameServer();

	m_RestartHookID = SH_ADD_HOOK(ISteamGameServer, WasRestartRequested, pGameServer, SH_MEMBER(this, &SteamToolsGSHooks::WasRestartRequested), false);
	m_logAnonHookID = SH_ADD_HOOK(ISteamGameServer, LogOnAnonymous     , pGameServer, SH_MEMBER(this, &SteamToolsGSHooks::LogOnAnonymous)     , false);
}

void SteamToolsGSHooks::RemoveHooks()
{
	SH_REMOVE_HOOK_ID(m_RestartHookID);
	SH_REMOVE_HOOK_ID(m_logAnonHookID);
}
