//
// AMX Mod X SteamTools
// Copyright (C) Vincent Herbet (Arkshine)
//
// This software is licensed under the GNU General Public License, version 2.
// For full license details, see LICENSE file.
//

#ifndef _STEAMWORKS_GAMESERVER_HOOKS_H_
#define _STEAMWORKS_GAMESERVER_HOOKS_H_

#include "interfaces.h"

constexpr size_t k_cbMaxGameServerMapName = 32u; // The maximum size (in UTF-8 bytes, including the null terminator) supported for game server map names which is set with ISteamGameServer::SetMapName.
constexpr size_t k_cbMaxGameServerName    = 64u; // The maximum size (in UTF-8 bytes, including the null terminator) supported for game server names which is set with ISteamGameServer::SetServerName.

class SteamToolsGSHooks
{
	public:

		SteamToolsGSHooks();
		~SteamToolsGSHooks();

	public:

		void LogOnAnonymous();
		bool WasRestartRequested();
		bool BSecure();

		void SetMapName(const char *name);
		void SetServerName(const char *name);

	public:

		void RegisterForwards();
		void AddHooks();
		void RemoveHooks();

		void SetCustomMapName(const char *name);
		void SetCustomServerName(const char *name);

	private:

		int m_ForwardRestartRequested;
		int m_ForwardTokenRequested;

	private:

		int m_RestartHookID;
		int m_LogAnonHookID;
		int m_BSecureHookID;

		int m_SetMapNameHookID;
		int m_SetServerNameHookID;

	private:

		bool m_ShowGameServerInfo;

		char m_CustomMapName[k_cbMaxGameServerMapName] = "";
		char m_CustomServerName[k_cbMaxGameServerName] = "";
};

#endif // _STEAMWORKS_GAMESERVER_HOOKS_H_
