//
// AMX Mod X SteamTools
// Copyright (C) Vincent Herbet (Arkshine)
//
// This software is licensed under the GNU General Public License, version 2.
// For full license details, see LICENSE file.
//

#include "utils.h"

namespace Utils
{
	edict_t* INDEXENT2(int index)
	{
		if (index >= 1 && index <= gpGlobals->maxClients)
		{
			return MF_GetPlayerEdict(index);
		}
		else
		{
			return g_engfuncs.pfnPEntityOfEntIndex(index);
		}
	}
};
