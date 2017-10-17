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

using namespace ke;

class SteamTools
{
	public:

		void Init();
		bool IsSteamToolsLoaded();

		void RequestState(void(*APIActivatedFunc)(), void(*APIShutdownFunc)());
		void OnAPIActivated();
		void OnAPIShutdown();

	public:

		CSteamID RenderedIDToCSteamID(const char *pRenderedID);
		int FindGameClient(CSteamID &target);

	public:

		UniquePtr<SteamToolsGSDetours>  m_Detours;
		UniquePtr<SteamToolsGameServer> m_GameServer;
		UniquePtr<SteamToolsGSForwards> m_Forwards;
		UniquePtr<SteamToolsGSHooks>    m_Hooks;
		UniquePtr<SteamToolsGSNatives>  m_Natives;

	private:

		void (*m_APiActivatedCallback)() = nullptr;
		void (*m_APiShutdownCallback)()  = nullptr;

		bool m_Loaded = false;
};

extern UniquePtr<SteamTools> g_SteamTools;
