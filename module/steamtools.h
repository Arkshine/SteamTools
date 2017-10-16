//
// AMX Mod X SteamTools
// Copyright (C) Vincent Herbet (Arkshine)
//
// This software is licensed under the GNU General Public License, version 2.
// For full license details, see LICENSE file.
//

#pragma once

#include "sw_detours.h"
#include "sw_gameserver.h"
#include "sw_forwards.h"
#include "sw_hooks.h"
#include "natives_gs.h"

#include <amtl/am-autoptr.h>
#include <amtl/am-utility.h>

class SteamTools
{
	public:

		SteamTools();
		~SteamTools();

	public:

		void Init();
		bool IsSteamToolsLoaded();

		void RequestState(void(*APIActivatedFunc)(), void(*APIShutdownFunc)());
		void OnAPIActivated();
		void OnAPIShutdown();

	public:

		CSteamID RenderedIDToCSteamID(const char *pRenderedID);

	public:

		SteamToolsGSDetours*  m_Detours;
		SteamToolsGameServer* m_GameServer;
		SteamToolsGSForwards* m_Forwards;
		SteamToolsGSHooks*    m_Hooks;
		SteamToolsGSNatives*  m_Natives;

	private:

		void (*m_APiActivatedCallback)();
		void (*m_APiShutdownCallback)();

		bool m_Loaded;
};

extern ke::AutoPtr<SteamTools> g_SteamTools;
