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

	bool IsPathFile(const char* path)
	{
#if defined(WIN32)

		DWORD attr = GetFileAttributes(path);

		if (attr == INVALID_FILE_ATTRIBUTES)
		{
			return false;
		}

		if (attr & (FILE_ATTRIBUTE_DIRECTORY | FILE_ATTRIBUTE_DEVICE))
		{
			return false;
		}

		return true;

#elif defined(LINUX) || defined(OSX)

		struct stat s;

		if (stat(path, &s) != 0)
		{
			return false;
		}

		return S_ISREG(s.st_mode) ? true : false;
#endif
	}
};
