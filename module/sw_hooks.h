
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
		EBeginAuthSessionResult BeginAuthSession(const void* pAuthTicket, int cbAuthTicket, CSteamID steamID);
		bool WasRestartRequested();

	public:

		void RegisterForwards();
		void AddHooks();
		void RemoveHooks();

	private:

		int m_ForwardRestartRequested;
		int m_ForwardTokenRequested;
		int m_ForwardBeginAuthSession;

	private:

		int m_RestartHookID;
		int m_logAnonHookID;
		int m_SessionHookID;
};

#endif // _SW_GAMESERVER_HOOKS_H_
