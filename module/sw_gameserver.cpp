//
// AMX Mod X SteamTools
// Copyright (C) Vincent Herbet (Arkshine)
//
// This software is licensed under the GNU General Public License, version 2.
// For full license details, see LICENSE file.
//

#include "sw_gameserver.h"
#include "module.h"
#include "steamtools.h"

SteamToolsGameServer::SteamToolsGameServer() 
	:
	m_pClient(nullptr), 
	m_pGameServer(nullptr),	
	m_pUtils(nullptr), 
	m_pHTTP(nullptr),

	m_SteamUserFn(nullptr), 
	m_SteamPipeFn(nullptr),
	m_GetCallback(nullptr), 
	m_FreeLastCallback(nullptr),

	m_ServerHookID(0), 
	m_UtilsHookID(0), 
	m_HttpHookID(0)
{   
}

SteamToolsGameServer::~SteamToolsGameServer()
{

}

ISteamClient* SteamToolsGameServer::GetSteamClient()
{
	return m_pClient;
}

ISteamGameServer* SteamToolsGameServer::GetGameServer()
{
	return m_pGameServer;
}

ISteamUtils* SteamToolsGameServer::GetUtils()
{
	return m_pUtils;
}

ISteamHTTP* SteamToolsGameServer::GetHTTP()
{
	return m_pHTTP;
}

SH_DECL_HOOK3(ISteamClient, GetISteamGameServer, SH_NOATTRIB, 0, ISteamGameServer*, HSteamUser, HSteamPipe, const char*);
SH_DECL_HOOK2(ISteamClient, GetISteamUtils     , SH_NOATTRIB, 0, ISteamUtils*     , HSteamPipe, const char*);
SH_DECL_HOOK3(ISteamClient, GetISteamHTTP      , SH_NOATTRIB, 0, ISteamHTTP*      , HSteamUser, HSteamPipe, const char*);

ISteamGameServer* SteamToolsGameServer::GetISteamGameServer(HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char* pchVersion)
{
	m_pGameServer = SH_CALL(m_pClient, &ISteamClient::GetISteamGameServer)(hSteamUser, hSteamPipe, STEAMGAMESERVER_INTERFACE_VERSION);
	SH_RETURN_META_VALUE(SH_MRES_SUPERCEDE, m_pGameServer);
}

ISteamUtils* SteamToolsGameServer::GetISteamUtils(HSteamPipe hSteamPipe, const char* pchVersion)
{
	m_pUtils = SH_CALL(m_pClient, &ISteamClient::GetISteamUtils)(hSteamPipe, STEAMUTILS_INTERFACE_VERSION);
	SH_RETURN_META_VALUE(SH_MRES_SUPERCEDE, m_pUtils);
}

ISteamHTTP* SteamToolsGameServer::GetISteamHTTP(HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char* pchVersion)
{
	m_pHTTP = SH_CALL(m_pClient, &ISteamClient::GetISteamHTTP)(hSteamUser, hSteamPipe, STEAMHTTP_INTERFACE_VERSION);
	SH_RETURN_META_VALUE(SH_MRES_SUPERCEDE, m_pHTTP);
}

void SteamToolsGameServer::SetSteamClient(ISteamClient* client)
{
	m_pClient = client;
}

void SteamToolsGameServer::AddHooks()
{
	m_ServerHookID = SH_ADD_HOOK(ISteamClient, GetISteamGameServer, m_pClient, SH_MEMBER(this, &SteamToolsGameServer::GetISteamGameServer), false);
	m_UtilsHookID  = SH_ADD_HOOK(ISteamClient, GetISteamUtils     , m_pClient, SH_MEMBER(this, &SteamToolsGameServer::GetISteamUtils)     , false);
	m_HttpHookID   = SH_ADD_HOOK(ISteamClient, GetISteamHTTP      , m_pClient, SH_MEMBER(this, &SteamToolsGameServer::GetISteamHTTP)      , false);
}

void SteamToolsGameServer::RemoveHooks()
{
	SH_REMOVE_HOOK_ID(m_ServerHookID);
	SH_REMOVE_HOOK_ID(m_UtilsHookID);
	SH_REMOVE_HOOK_ID(m_HttpHookID);
}

void SteamToolsGameServer::GetUserAndPipe(HSteamUser& hSteamUser, HSteamPipe& hSteamPipe)
{
	hSteamUser = m_SteamUserFn();
	hSteamPipe = m_SteamPipeFn();
}

void SteamToolsGameServer::SetUserAndPipe(void* user, void* pipe)
{
	m_SteamUserFn = reinterpret_cast<GetUserFn>(user);
	m_SteamPipeFn = reinterpret_cast<GetPipeFn>(pipe);
}

bool SteamToolsGameServer::GetCallback(HSteamPipe hSteamPipe, CallbackMsg_t* pCallbackMsg)
{
	return m_GetCallback(hSteamPipe, pCallbackMsg);
}

void SteamToolsGameServer::FreeLastCallback(HSteamPipe hSteamPipe)
{
	m_FreeLastCallback(hSteamPipe);
}

void SteamToolsGameServer::SetCallbackFuncs(void* getFn, void* freeFn)
{
	m_GetCallback      = reinterpret_cast<GetCallbackFn>(getFn);
	m_FreeLastCallback = reinterpret_cast<FreeLastCallbackFn>(freeFn);
}

void SteamToolsGameServer::Think()
{
	if (!g_SteamTools->m_GameServer->GetSteamClient())
	{
		return;
	}

	static int lastCallback = -1;

	CallbackMsg_t callbackMsg;

	if (GetCallback(m_SteamPipeFn(), &callbackMsg))
	{
		if (lastCallback == callbackMsg.m_iCallback)
		{
			return;
		}

		switch (lastCallback = callbackMsg.m_iCallback)
		{
			case SteamServersConnected_t::k_iCallback:
			{
				g_SteamTools->m_Forwards->OnSteamServersConnected(reinterpret_cast<SteamServersConnected_t*>(callbackMsg.m_pubParam));
				break;
			}
			case SteamServerConnectFailure_t::k_iCallback:
			{
				g_SteamTools->m_Forwards->OnSteamServersConnectFailure(reinterpret_cast<SteamServerConnectFailure_t*>(callbackMsg.m_pubParam));
				break;
			}
			case SteamServersDisconnected_t::k_iCallback:
			{
				g_SteamTools->m_Forwards->OnSteamServersDisconnected(reinterpret_cast<SteamServersDisconnected_t*>(callbackMsg.m_pubParam));
				break;
			}
			case GSClientApprove_t::k_iCallback:
			{
				g_SteamTools->m_Forwards->OnGSClientApprove(reinterpret_cast<GSClientApprove_t*>(callbackMsg.m_pubParam));
				break;
			}
			case ValidateAuthTicketResponse_t::k_iCallback:
			{
				g_SteamTools->m_Forwards->OnValidateTicket(reinterpret_cast<ValidateAuthTicketResponse_t*>(callbackMsg.m_pubParam));
				break;
			}
			case GSClientGroupStatus_t::k_iCallback:
			{
				g_SteamTools->m_Forwards->OnGroupStatusResult(reinterpret_cast<GSClientGroupStatus_t*>(callbackMsg.m_pubParam));
				FreeLastCallback(m_SteamPipeFn());
				break;
			}
		}
	}
	else if (lastCallback != -1)
	{
		lastCallback = -1;
	}
}