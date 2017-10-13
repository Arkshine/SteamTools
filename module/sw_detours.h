//
// AMX Mod X SteamTools
// Copyright (C) Vincent Herbet (Arkshine)
//
// This software is licensed under the GNU General Public License, version 2.
// For full license details, see LICENSE file.
//

#ifndef _STEAMWORKS_DETOURS_H_
#define _STEAMWORKS_DETOURS_H_

#include "interfaces.h"
#include "CDetour/detours.h"
#include <amtl/os/am-shared-library.h>

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

class SimpleLib
{
	#if defined(KE_WINDOWS)
		typedef HMODULE SysType;
	#else
		typedef void* SysType;
	#endif

	public:

		explicit SimpleLib(const SysType& lib) : lib_(lib)
		{}

		void* lookup(const char* symbol)
		{
	#if defined(KE_WINDOWS)
			return reinterpret_cast<void*>(GetProcAddress(lib_, symbol));
	#else
			return dlsym(lib_, symbol);
	#endif
		}

	private:

		SysType lib_;
};

#endif // _STEAMWORKS_DETOURS_H_