//
// AMX Mod X SteamTools
// Copyright (C) Vincent Herbet (Arkshine)
//
// This software is licensed under the GNU General Public License, version 2.
// For full license details, see LICENSE file.
//

#ifndef _SW_GAMESERVER_H_
#define _SW_GAMESERVER_H_

#include "interfaces.h"

typedef HSteamUser (*GetUserFn)();
typedef HSteamPipe (*GetPipeFn)();

typedef bool (*GetCallbackFn)      (HSteamPipe hSteamPipe, CallbackMsg_t* pCallbackMsg);
typedef void (*FreeLastCallbackFn) (HSteamPipe hSteamPipe);

class SteamToolsGameServer
{
	public:

		SteamToolsGameServer();
		~SteamToolsGameServer();

	public:
		
		ISteamClient     *GetSteamClient();
		ISteamGameServer *GetGameServer();
		ISteamUtils      *GetUtils();
		ISteamHTTP       *GetHTTP();

	public:

		void SetSteamClient(ISteamClient* client);

		void GetUserAndPipe(HSteamUser &hSteamUser, HSteamPipe &hSteamPipe);
		void SetUserAndPipe(void* userFnPtr, void* pipeFnPtr);

		bool GetCallback     (HSteamPipe hSteamPipe, CallbackMsg_t* pCallbackMsg);
		void FreeLastCallback(HSteamPipe hSteamPipe);
		void SetCallbackFuncs(void* getFn, void* freeFn);

	public:

		void AddHooks();
		void RemoveHooks();
		void Think();

	private:

		ISteamGameServer* GetISteamGameServer(HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char* pchVersion);
		ISteamUtils*      GetISteamUtils     (HSteamPipe hSteamPipe, const char* pchVersion);
		ISteamHTTP*       GetISteamHTTP      (HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char* pchVersion);

	private:

		ISteamClient      *m_pClient;
		ISteamGameServer  *m_pGameServer;
		ISteamUtils       *m_pUtils;
		ISteamHTTP        *m_pHTTP;

		GetUserFn          m_SteamUserFn;
		GetPipeFn          m_SteamPipeFn;

		GetCallbackFn      m_GetCallback;
		FreeLastCallbackFn m_FreeLastCallback;

	private:

		int m_ServerHookID;
		int m_UtilsHookID;
		int m_HttpHookID;
};

#endif // _SW_GAMESERVER_H_
