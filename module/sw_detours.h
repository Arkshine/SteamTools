
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

		CDetour* m_InitDetour;
		CDetour* m_ShutdownDetour;
};

#endif // _SW_DETOURS_H_