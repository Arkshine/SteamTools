
#ifndef _NATIVES_GS_H_
#define _NATIVES_GS_H_

#if defined(WIN32)
#  if defined(_MSC_VER )
#    define atoui64(str) _strtoui64(str, 0, 10) 
#  else 
#    define atoui64(str) strtoul(str, 0, 10) 
#  endif 
#else 
#  define atoui64(str) strtoull(str, 0, 10) 
#endif

class SteamWorksGSNatives
{
	public:

		SteamWorksGSNatives();
		~SteamWorksGSNatives();
};

#endif // _NATIVES_GS_H_
