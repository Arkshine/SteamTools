//
// AMX Mod X SteamTools
// Copyright (C) Vincent Herbet (Arkshine)
//
// This software is licensed under the GNU General Public License, version 2.
// For full license details, see LICENSE file.
//

#ifndef _STEAMWORKS_FORWARDS_H_
#define _STEAMWORKS_FORWARDS_H_

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
		void OnGSClientDeny              (GSClientDeny_t* pApprove);
		void OnGSClientKick              (GSClientKick_t* pApprove);
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
		int m_ForwardInvalidateClient;
		int m_ForwardGroupStatusResult;
};

#endif // _STEAMWORKS_FORWARDS_H_
