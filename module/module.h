
#ifndef _MODULE_H_
#define _MODULE_H_

#include <sourcehook/sourcehook.h>
#include "interfaces.h"

extern SourceHook::ISourceHook *g_SHPtr;
extern int g_PLID;

void OnSteamAPIActivated();
void OnStartFrame();
bool UTIL_CheckForPublic(const char* publicName);
CSteamID atocsteamid(const char *pRenderedID);

#endif // _MODULE_H_