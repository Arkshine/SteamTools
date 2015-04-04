
#ifndef _STEAMTOOLS_H_
#define _STEAMTOOLS_H_

#include "sw_detours.h"
#include "sw_gameserver.h"
#include "sw_forwards.h"
#include "sw_hooks.h"
#include "natives_gs.h"

#include <am-utility.h>

class SteamTools
{
	public:

		SteamTools();
		~SteamTools();

	public:

		void Init();
		void Think();
		bool IsSteamToolsLoaded();

		void RequestState(void(*APIActivatedFunc)(), void(*APIShutdownFunc)());
		void OnAPIActivated();
		void OnAPIShutdown();

	public:

		SteamToolsGSDetours*  m_Detours;
		SteamToolsGameServer* m_GameServer;
		SteamToolsGSForwards* m_Forwards;
		SteamToolsGSHooks*    m_Hooks;
		SteamWorksGSNatives*  m_Natives;

	private:

		void (*m_APiActivatedCallback)();
		void (*m_APiShutdownCallback)();

		float m_LastThinkTime;
		bool  m_Loaded;
};

extern ke::AutoPtr<SteamTools> g_SteamTools;

#endif // _STEAMTOOLS_H_