//
// AMX Mod X SteamTools
// Copyright (C) Vincent Herbet (Arkshine)
//
// This software is licensed under the GNU General Public License, version 2.
// For full license details, see LICENSE file.
//

#ifndef _SW_DETOURS_H_
#define _SW_DETOURS_H_

#include "interfaces.h"
#include "CDetour/detours.h"

class SteamToolsGSDetours
{
	public:

		SteamToolsGSDetours();
		~SteamToolsGSDetours();

	private:

		CDetour* m_InitGameServerDetour;
		CDetour* m_ShutdownGameServerDetour;
		CDetour* m_InitSteamClientDetour;
};

#endif // _SW_DETOURS_H_