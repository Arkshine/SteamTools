
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
