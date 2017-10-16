//
// AMX Mod X SteamTools
// Copyright (C) Vincent Herbet (Arkshine)
//
// This software is licensed under the GNU General Public License, version 2.
// For full license details, see LICENSE file.
//

#pragma once

#include "interfaces.h"

class SteamToolsGSHooks
{
	public:

		SteamToolsGSHooks();
		~SteamToolsGSHooks();

	public: // Virtual functions hooked

		void LogOnAnonymous();
		bool WasRestartRequested();
		bool BSecure();

		void SetMapName(const char *name);
		void SetServerName(const char *name);

	public: // Logic

		void RegisterForwards();
		void AddHooks();
		void RemoveHooks();

		void SetCustomMapName(const char *name);
		void SetCustomServerName(const char *name);

	private: // AMX Mod X Forward id

		int m_ForwardRestartRequested;
		int m_ForwardTokenRequested;

	private: // Virtual Hook Id

		int m_RestartHookID;
		int m_LogAnonHookID;
		int m_BSecureHookID;

		int m_SetMapNameHookID;
		int m_SetServerNameHookID;

	private: // Custom Server State

		char m_CustomMapName[k_cbMaxGameServerMapName] = "";
		char m_CustomServerName[k_cbMaxGameServerName] = "";
};

