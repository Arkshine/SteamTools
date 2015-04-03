
#ifndef _SW_FORWARDS_H_
#define _SW_FORWARDS_H_

#include "interfaces.h"

class SteamToolsGSForwards
{
	public:

		SteamToolsGSForwards();
		~SteamToolsGSForwards();

	public:

		void RegisterForwards();

	public:

		void OnSteamFullyLoaded();
		void OnSteamShutdown();

	public:

		void OnGSClientApprove           (GSClientApprove_t* pApprove);
		void OnValidateTicket            (ValidateAuthTicketResponse_t* pTicket);
		void OnSteamServersConnected     (SteamServersConnected_t* pResponse);
		void OnSteamServersConnectFailure(SteamServerConnectFailure_t* pResponse);
		void OnSteamServersDisconnected  (SteamServersDisconnected_t* pResponse);
		void OnGroupStatusResult         (GSClientGroupStatus_t* pReputation);

	private:

		int m_ForwardFullyLoaded;
		int m_ForwardShutdown;
		int m_ForwardConnected;
		int m_ForwardConnectFailure;
		int m_ForwardDisconnected;
		int m_ForwardValidateClient;
		int m_ForwardGroupStatusResult;
};

#endif // _SW_FORWARDS_H_
