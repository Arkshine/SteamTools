//
// AMX Mod X SteamTools
// Copyright (C) Vincent Herbet (Arkshine)
//
// This software is licensed under the GNU General Public License, version 2.
// For full license details, see LICENSE file.
//

#pragma once

#include "interfaces.h"

class SteamToolsGSForwards
{
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

		int m_ForwardFullyLoaded       = -1;
		int m_ForwardShutdown          = -1;
		int m_ForwardConnected         = -1;
		int m_ForwardConnectFailure    = -1;
		int m_ForwardDisconnected      = -1;
		int m_ForwardValidateClient    = -1;
		int m_ForwardInvalidateClient  = -1;
		int m_ForwardGroupStatusResult = -1;
};
