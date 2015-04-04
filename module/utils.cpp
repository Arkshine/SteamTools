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

	CSteamID RenderedIDToCSteamID(const char *pRenderedID)
	{
		// Convert the Steam2 ID string to a Steam2 ID structure
		TSteamGlobalUserID steam2ID;
		steam2ID.m_SteamInstanceID = 0;
		steam2ID.m_SteamLocalUserID.Split.High32bits = 0;
		steam2ID.m_SteamLocalUserID.Split.Low32bits = 0;

		const char *pchTSteam2ID = pRenderedID;
		const char *pchOptionalLeadString = "STEAM_";

		if (strncasecmp(pRenderedID, pchOptionalLeadString, strlen(pchOptionalLeadString)) == 0)
		{
			pchTSteam2ID = pRenderedID + strlen(pchOptionalLeadString);
		}

		char cExtraCharCheck = 0;

		int cFieldConverted = sscanf(pchTSteam2ID, "%hu:%u:%u%c", &steam2ID.m_SteamInstanceID, &steam2ID.m_SteamLocalUserID.Split.High32bits, &steam2ID.m_SteamLocalUserID.Split.Low32bits, &cExtraCharCheck);

		// Validate the conversion ... a special case is steam2 instance ID 1 which is reserved for special DoD handling
		if (cExtraCharCheck != 0 || cFieldConverted == EOF || cFieldConverted < 2 || (cFieldConverted < 3 && steam2ID.m_SteamInstanceID != 1))
		{
			return k_steamIDNil;
		}

		// Now convert to steam ID from the Steam2 ID structure
		CSteamID steamID;
		steamID.SetFromSteam2(&steam2ID, k_EUniversePublic);
		return steamID;
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
