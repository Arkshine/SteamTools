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

#include <amtl/os/am-path.h>
#include <amtl/os/am-fsutil.h>

cvar_t CvarInitSetSteamAccount = { "sv_setsteamaccount", "", FCVAR_EXTDLL | FCVAR_PROTECTED | FCVAR_NOEXTRAWHITEPACE | FCVAR_SPONLY };
cvar_t* CvarSetSteamAccount;

SH_DECL_HOOK0     (ISteamGameServer, WasRestartRequested, SH_NOATTRIB, 0, bool);
SH_DECL_HOOK0_void(ISteamGameServer, LogOnAnonymous     , SH_NOATTRIB, 0);
SH_DECL_HOOK0     (ISteamGameServer, BSecure            , SH_NOATTRIB, 0, bool);
SH_DECL_HOOK1_void(ISteamGameServer, SetMapName         , SH_NOATTRIB, 0, const char*);
SH_DECL_HOOK1_void(ISteamGameServer, SetServerName      , SH_NOATTRIB, 0, const char*);

SteamToolsGSHooks::SteamToolsGSHooks()
{
	CVAR_REGISTER(&CvarInitSetSteamAccount);
	CvarSetSteamAccount = CVAR_GET_POINTER(CvarInitSetSteamAccount.name);

	char realpath[260];
	ke::SafeSprintf(realpath, sizeof(realpath), "%s/%s/steamtools.cfg", MF_GetModname(), MF_GetLocalInfo("amx_configdir", "addons/amxmodx/configs"));

	if (ke::file::IsFile(realpath))
	{
		SERVER_COMMAND(UTIL_VarArgs("exec %s\n", realpath));
		SERVER_EXECUTE();
	}
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
	auto wasRestartRequested = SH_CALL(g_SteamTools->m_GameServer->GetGameServer(), &ISteamGameServer::WasRestartRequested)();

	if (wasRestartRequested)
	{
		wasRestartRequested = MF_ExecuteForward(m_ForwardRestartRequested) > 0;
	}

	SH_RETURN_META_VALUE(SH_MRES_SUPERCEDE, wasRestartRequested);
}

void SteamToolsGSHooks::LogOnAnonymous(void)
{
	auto pGameServer = g_SteamTools->m_GameServer->GetGameServer();

	if (!pGameServer)
	{
		SH_RETURN_META(SH_MRES_SUPERCEDE);
	}

	if (!strlen(CvarSetSteamAccount->string))
	{
		SH_RETURN_META(SH_MRES_IGNORED);
	}

	pGameServer->LogOn(CvarSetSteamAccount->string, CvarSetSteamAccount->string);

	SH_RETURN_META(SH_MRES_SUPERCEDE);
}

bool SteamToolsGSHooks::BSecure()
{
	auto pGameServer = g_SteamTools->m_GameServer->GetGameServer();

	auto publicIP = pGameServer->GetPublicIP();
	auto cSteamID = pGameServer->GetSteamID();

	MF_PrintSrvConsole("   Public IP is %i.%i.%i.%i\n", publicIP >> 24 & 255, publicIP >> 16 & 255, publicIP >> 8 & 255, publicIP & 255);
	MF_PrintSrvConsole("   Assigned %s gameserver Steam %s (%llu)\n", cSteamID.GetEAccountType() == k_EAccountTypeAnonGameServer ? "anonymous" : "persistent", cSteamID.SteamRender(), cSteamID.ConvertToUint64());

	auto isSecure = SH_CALL(pGameServer, &ISteamGameServer::BSecure)();

	SH_RETURN_META_VALUE(SH_MRES_IGNORED, isSecure);
}


void SteamToolsGSHooks::SetCustomMapName(const char *name)
{
	ke::SafeStrcpy(m_CustomMapName, sizeof(m_CustomMapName), name);
}

void SteamToolsGSHooks::SetCustomServerName(const char *name)
{
	ke::SafeStrcpy(m_CustomServerName, sizeof(m_CustomServerName), name);
}

void SteamToolsGSHooks::SetMapName(const char *name)
{
	SH_CALL(g_SteamTools->m_GameServer->GetGameServer(), &ISteamGameServer::SetMapName)(*m_CustomMapName ? m_CustomMapName : name);
	SH_RETURN_META(SH_MRES_SUPERCEDE);
}

void SteamToolsGSHooks::SetServerName(const char *name)
{
	SH_CALL(g_SteamTools->m_GameServer->GetGameServer(), &ISteamGameServer::SetServerName)(*m_CustomServerName ? m_CustomServerName : name);
	SH_RETURN_META(SH_MRES_SUPERCEDE);
}


void SteamToolsGSHooks::AddHooks()
{
	auto pGameServer = g_SteamTools->m_GameServer->GetGameServer();

	m_RestartHookID = SH_ADD_HOOK(ISteamGameServer, WasRestartRequested, pGameServer, SH_MEMBER(this, &SteamToolsGSHooks::WasRestartRequested), false);
	m_LogAnonHookID = SH_ADD_HOOK(ISteamGameServer, LogOnAnonymous     , pGameServer, SH_MEMBER(this, &SteamToolsGSHooks::LogOnAnonymous)     , false);
	m_BSecureHookID = SH_ADD_HOOK(ISteamGameServer, BSecure            , pGameServer, SH_MEMBER(this, &SteamToolsGSHooks::BSecure)            , false);

	m_SetMapNameHookID    = SH_ADD_HOOK(ISteamGameServer, SetMapName   , pGameServer, SH_MEMBER(this, &SteamToolsGSHooks::SetMapName)   , false);
	m_SetServerNameHookID = SH_ADD_HOOK(ISteamGameServer, SetServerName, pGameServer, SH_MEMBER(this, &SteamToolsGSHooks::SetServerName), false);
}

void SteamToolsGSHooks::RemoveHooks()
{
	SH_REMOVE_HOOK_ID(m_RestartHookID);
	SH_REMOVE_HOOK_ID(m_LogAnonHookID);
	SH_REMOVE_HOOK_ID(m_BSecureHookID);

	SH_REMOVE_HOOK_ID(m_SetMapNameHookID);
	SH_REMOVE_HOOK_ID(m_SetServerNameHookID);
}
