//
// AMX Mod X SteamTools
// Copyright (C) Vincent Herbet (Arkshine)
//
// This software is licensed under the GNU General Public License, version 2.
// For full license details, see LICENSE file.
//

#pragma once
#if defined(WIN32)
#  if defined(_MSC_VER)
#    define atoui64(str) _strtoui64(str, 0, 10)
#  else
#    define atoui64(str) strtoul(str, 0, 10)
#  endif
#else
#  define atoui64(str) strtoull(str, 0, 10)
#endif

class SteamToolsGSNatives
{
	public:

		SteamToolsGSNatives();
		~SteamToolsGSNatives();
};
