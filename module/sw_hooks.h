
#ifndef _SW_GAMESERVER_HOOKS_H_
#define _SW_GAMESERVER_HOOKS_H_

#include "interfaces.h"

class SteamToolsGSHooks
{
	public:

		SteamToolsGSHooks();
		~SteamToolsGSHooks();

	public:

		void LogOnAnonymous();
		bool WasRestartRequested();

	public:

		void RegisterForwards();
		void AddHooks();
		void RemoveHooks();

	private:

		int m_ForwardRestartRequested;
		int m_ForwardTokenRequested;

	private:

		int m_RestartHookID;
		int m_logAnonHookID;
};

#endif // _SW_GAMESERVER_HOOKS_H_
