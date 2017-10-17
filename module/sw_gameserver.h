//
// AMX Mod X SteamTools
// Copyright (C) Vincent Herbet (Arkshine)
//
// This software is licensed under the GNU General Public License, version 2.
// For full license details, see LICENSE file.
//

#pragma once

#include "interfaces.h"
#include <CDetour/detours.h>
#include <amtl/am-autoptr.h>

using namespace ke;

typedef HSteamUser (*GetUserFn)();
typedef HSteamPipe (*GetPipeFn)();

typedef bool (*GetCallbackFn)      (HSteamPipe hSteamPipe, CallbackMsg_t* pCallbackMsg);
typedef void (*FreeLastCallbackFn) (HSteamPipe hSteamPipe);

class SteamToolsGameServer
{
	public:

		ISteamClient     *GetSteamClient();
		ISteamGameServer *GetGameServer();
		ISteamUtils      *GetUtils();
		ISteamHTTP       *GetHTTP();

	public:

		void SetSteamClient(ISteamClient* client);

		void GetUserAndPipe(HSteamUser &hSteamUser, HSteamPipe &hSteamPipe);
		void SetUserAndPipe(void *userFnPtr, void *pipeFnPtr);

		void FreeLastCallback();
		void SetCallbackFuncs(void *getFn, void *freeFn);

	public:

		void AddInterfaceHooks();
		void RemoveInterfaceHooks();

		void AddCallbackHook();

	private:

		ISteamGameServer*GetISteamGameServer(HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char *pchVersion);
		ISteamUtils     *GetISteamUtils     (HSteamPipe hSteamPipe, const char *pchVersion);
		ISteamHTTP      *GetISteamHTTP      (HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char *pchVersion);

	private:

		ISteamClient      *m_pClient     = nullptr;
		ISteamGameServer  *m_pGameServer = nullptr;
		ISteamUtils       *m_pUtils      = nullptr;
		ISteamHTTP        *m_pHTTP       = nullptr;

		GetUserFn          m_SteamUserFn = nullptr;
		GetPipeFn          m_SteamPipeFn = nullptr;

		GetCallbackFn      m_GetCallback      = nullptr;
		FreeLastCallbackFn m_FreeLastCallback = nullptr;

		AutoPtr<CDetour>   m_GetCallbackDetour;

	private:

		int m_ServerHookID = 0;
		int m_UtilsHookID  = 0;
		int m_HttpHookID   = 0;
};
