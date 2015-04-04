//
// AMX Mod X SteamTools
// Copyright (C) Vincent Herbet (Arkshine)
//
// This software is licensed under the GNU General Public License, version 2.
// For full license details, see LICENSE file.
//

#ifndef _UTILS_H_
#define _UTILS_H_

#include "interfaces.h"
#include <amxxmodule.h>

namespace Utils
{
	edict_t* INDEXENT2(int index);
	CSteamID RenderedIDToCSteamID(const char *pRenderedID);	
};

#endif // _UTILS_H_
