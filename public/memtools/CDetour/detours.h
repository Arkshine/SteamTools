/**
* vim: set ts=4 :
* =============================================================================
* SourceMod
* Copyright (C) 2004-2010 AlliedModders LLC.  All rights reserved.
* =============================================================================
*
* This program is free software; you can redistribute it and/or modify it under
* the terms of the GNU General Public License, version 3.0, as published by the
* Free Software Foundation.
* 
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
* FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
* details.
*
* You should have received a copy of the GNU General Public License along with
* this program.  If not, see <http://www.gnu.org/licenses/>.
*
* As a special exception, AlliedModders LLC gives you permission to link the
* code of this program (as well as its derivative works) to "Half-Life 2," the
* "Source Engine," the "SourcePawn JIT," and any Game MODs that run on software
* by the Valve Corporation.  You must obey the GNU General Public License in
* all respects for all other code used.  Additionally, AlliedModders LLC grants
* this exception to all derivative works.  AlliedModders LLC defines further
* exceptions, found in LICENSE.txt (as of this writing, version JULY-31-2007),
* or <http://www.sourcemod.net/license.php>.
*
* Version: $Id: detours.h 257 2008-09-23 03:12:13Z pred $
*/

#ifndef _INCLUDE_SOURCEMOD_DETOURS_H_
#define _INCLUDE_SOURCEMOD_DETOURS_H_

#include "amxxmodule.h"
//#include <jit/jit_helpers.h>
//#include <jit/x86/x86_macros.h>
#include "detourhelpers.h"

/**
 * CDetours class for SourceMod Extensions by pRED*
 * detourhelpers.h entirely stolen from CSS:DM and were written by BAILOPAN (I assume).
 * asm.h/c from devmaster.net (thanks cybermind) edited by pRED* to handle gcc -fPIC thunks correctly
 * Concept by Nephyrin Zey (http://www.doublezen.net/) and Windows Detour Library (http://research.microsoft.com/sn/detours/)
 * Member function pointer ideas by Don Clugston (http://www.codeproject.com/cpp/FastDelegate.asp)
 */

#define DETOUR_MEMBER_CALL(name) (this->*name##_Actual)
#define DETOUR_STATIC_CALL(name) (name##_Actual)

#define DETOUR_DECL_STATIC0(name, ret) \
ret (*name##_Actual)(void) = NULL; \
ret name(void)

#define DETOUR_DECL_STATIC1(name, ret, p1type, p1name) \
ret (*name##_Actual)(p1type) = NULL; \
ret name(p1type p1name)

#define DETOUR_DECL_STATIC2(name, ret, p1type, p1name, p2type, p2name) \
ret (*name##_Actual)(p1type, p2type) = NULL; \
ret name(p1type p1name, p2type p2name)

#define DETOUR_DECL_STATIC3(name, ret, p1type, p1name, p2type, p2name, p3type, p3name) \
ret (*name##_Actual)(p1type, p2type, p3type) = NULL; \
ret name(p1type p1name, p2type p2name, p3type p3name)

#define DETOUR_DECL_STATIC4(name, ret, p1type, p1name, p2type, p2name, p3type, p3name, p4type, p4name) \
ret (*name##_Actual)(p1type, p2type, p3type, p4type) = NULL; \
ret name(p1type p1name, p2type p2name, p3type p3name, p4type p4name)

#define DETOUR_DECL_STATIC6(name, ret, p1type, p1name, p2type, p2name, p3type, p3name, p4type, p4name, p5type, p5name, p6type, p6name) \
ret (*name##_Actual)(p1type, p2type, p3type, p4type, p5type, p6type) = NULL; \
ret name(p1type p1name, p2type p2name, p3type p3name, p4type p4name, p5type p5name, p6type p6name)

#define DETOUR_DECL_STATIC8(name, ret, p1type, p1name, p2type, p2name, p3type, p3name, p4type, p4name, p5type, p5name, p6type, p6name, p7type, p7name, p8type, p8name) \
ret (*name##_Actual)(p1type, p2type, p3type, p4type, p5type, p6type, p7type, p8type) = NULL; \
ret name(p1type p1name, p2type p2name, p3type p3name, p4type p4name, p5type p5name, p6type p6name, p7type p7name, p8type p8name)

#define DETOUR_DECL_MEMBER0(name, ret) \
class name##Class \
{ \
public: \
	ret name(); \
	static ret (name##Class::* name##_Actual)(void); \
}; \
ret (name##Class::* name##Class::name##_Actual)(void) = NULL; \
ret name##Class::name()

#define DETOUR_DECL_MEMBER1(name, ret, p1type, p1name) \
class name##Class \
{ \
public: \
	ret name(p1type p1name); \
	static ret (name##Class::* name##_Actual)(p1type); \
}; \
ret (name##Class::* name##Class::name##_Actual)(p1type) = NULL; \
ret name##Class::name(p1type p1name)

#define DETOUR_DECL_MEMBER2(name, ret, p1type, p1name, p2type, p2name) \
class name##Class \
{ \
public: \
	ret name(p1type p1name, p2type p2name); \
	static ret (name##Class::* name##_Actual)(p1type, p2type); \
}; \
ret (name##Class::* name##Class::name##_Actual)(p1type, p2type) = NULL; \
ret name##Class::name(p1type p1name, p2type p2name)

#define DETOUR_DECL_MEMBER3(name, ret, p1type, p1name, p2type, p2name, p3type, p3name) \
class name##Class \
{ \
public: \
	ret name(p1type p1name, p2type p2name, p3type p3name); \
	static ret (name##Class::* name##_Actual)(p1type, p2type, p3type); \
}; \
ret (name##Class::* name##Class::name##_Actual)(p1type, p2type, p3type) = NULL; \
ret name##Class::name(p1type p1name, p2type p2name, p3type p3name)

#define DETOUR_DECL_MEMBER4(name, ret, p1type, p1name, p2type, p2name, p3type, p3name, p4type, p4name) \
class name##Class \
{ \
public: \
        ret name(p1type p1name, p2type p2name, p3type p3name, p4type p4name); \
        static ret (name##Class::* name##_Actual)(p1type, p2type, p3type, p4type); \
}; \
ret (name##Class::* name##Class::name##_Actual)(p1type, p2type, p3type, p4type) = NULL; \
ret name##Class::name(p1type p1name, p2type p2name, p3type p3name, p4type p4name)

#define DETOUR_DECL_MEMBER8(name, ret, p1type, p1name, p2type, p2name, p3type, p3name, p4type, p4name, p5type, p5name, p6type, p6name, p7type, p7name, p8type, p8name) \
class name##Class \
{ \
public: \
        ret name(p1type p1name, p2type p2name, p3type p3name, p4type p4name, p5type p5name, p6type p6name, p7type p7name, p8type p8name); \
        static ret (name##Class::* name##_Actual)(p1type, p2type, p3type, p4type, p5type, p6type, p7type, p8type); \
}; \
ret (name##Class::* name##Class::name##_Actual)(p1type, p2type, p3type, p4type, p5type, p6type, p7type, p8type) = NULL; \
ret name##Class::name(p1type p1name, p2type p2name, p3type p3name, p4type p4name, p5type p5name, p6type p6name, p7type p7name, p8type p8name)


#define GET_MEMBER_CALLBACK(name) (void *)GetCodeAddress(&name##Class::name)
#define GET_MEMBER_TRAMPOLINE(name) (void **)(&name##Class::name##_Actual)

#define GET_STATIC_CALLBACK(name) (void *)&name
#define GET_STATIC_TRAMPOLINE(name) (void **)&name##_Actual

#define DETOUR_CREATE_MEMBER(name, gamedata, target)	CDetourManager::CreateDetour(GET_MEMBER_CALLBACK(name), GET_MEMBER_TRAMPOLINE(name), gamedata, target);
#define DETOUR_CREATE_MEMBER_FIXED(name, address)		CDetourManager::CreateDetour(GET_MEMBER_CALLBACK(name), GET_MEMBER_TRAMPOLINE(name), address);
#define DETOUR_CREATE_STATIC(name, gamedata, target)	CDetourManager::CreateDetour(GET_STATIC_CALLBACK(name), GET_STATIC_TRAMPOLINE(name), gamedata, target);
#define DETOUR_CREATE_STATIC_FIXED(name, address)		CDetourManager::CreateDetour(GET_STATIC_CALLBACK(name), GET_STATIC_TRAMPOLINE(name), address);

class GenericClass {};
typedef void (GenericClass::*VoidFunc)();

inline void *GetCodeAddr(VoidFunc mfp)
{
	return *(void **)&mfp;
}

/**
 * Converts a member function pointer to a void pointer.
 * This relies on the assumption that the code address lies at mfp+0
 * This is the case for both g++ and later MSVC versions on non virtual functions but may be different for other compilers
 * Based on research by Don Clugston : http://www.codeproject.com/cpp/FastDelegate.asp
 */
#define GetCodeAddress(mfp) GetCodeAddr(reinterpret_cast<VoidFunc>(mfp))

class CDetourManager;

class CDetour
{
public:
	~CDetour();

public:

	bool IsEnabled();

	/**
	 * These would be somewhat self-explanatory I hope
	 */
	void EnableDetour();
	void DisableDetour();

	void Destroy();

	friend class CDetourManager;

protected:
	CDetour(void *callbackfunction, void **trampoline, const char *signame);
	CDetour(void *callbackfunction, void **trampoline, void *address);

	bool Init();
private:

	/* These create/delete the allocated memory */
	bool CreateDetour();
	void DeleteDetour();

	bool enabled;
	bool detoured;

	patch_t detour_restore;
	/* Address of the detoured function */
	void *detour_address;
	/* Address of the allocated trampoline function */
	void *detour_trampoline;
	size_t detour_trampolineSize;
	/* Address of the callback handler */
	void *detour_callback;
	/* The function pointer used to call our trampoline */
	void **trampoline;
	
	const char *signame;
	void *address;
};

class CDetourManager
{
public:

	static void Init();

	/**
	 * Creates a new detour
	 *
	 * @param callbackfunction			Void pointer to your detour callback function.
	 * @param trampoline				Address of the trampoline pointer
	 * @param signame					Section name containing a signature to fetch from the gamedata file.
	 * @return							A new CDetour pointer to control your detour.
	 *
	 * Example:
	 *
	 * CBaseServer::ConnectClient(netadr_s &, int, int, int, char  const*, char  const*, char  const*, int)
	 *
	 * Define a new class with the required function and a member function pointer to the same type:
	 *
	 * class CBaseServerDetour
	 * {
	 * public:
	 *		 bool ConnectClient(void *netaddr_s, int, int, int, char  const*, char  const*, char  const*, int);
	 *		 static bool (CBaseServerDetour::* ConnectClient_Actual)(void *netaddr_s, int, int, int, char  const*, char  const*, char  const*, int);
	 * }
	 *
	 *	void *callbackfunc = GetCodeAddress(&CBaseServerDetour::ConnectClient);
	 *	void **trampoline = (void **)(&CBaseServerDetour::ConnectClient_Actual);
	 *
	 * Creation:
	 * CDetourManager::CreateDetour(callbackfunc,  trampoline, "ConnectClient");
	 *
	 * Usage:
	 *
	 * CBaseServerDetour::ConnectClient(void *netaddr_s, int, int, int, char  const*, char  const*, char  const*, int)
	 * {
	 *			//pre hook code
	 *			bool result = (this->*ConnectClient_Actual)(netaddr_s, rest of params);
	 *			//post hook code
	 *			return result;
	 * }
	 *
	 * Note we changed the netadr_s reference into a void* to avoid needing to define the type
	 */
	static CDetour *CreateDetour(void *callbackfunction, void **trampoline, const char *signame);
	static CDetour *CreateDetour(void *callbackfunction, void **trampoline, void *address);

	friend class CBlocker;
	friend class CDetour;

/*private:
	static ISourcePawnEngine *spengine;
	static IGameConfig *gameconf;*/
};


// Helpers from jit_helpers.h/x86_macros.h.

class JitWriter
{
public:
	inline cell read_cell()
	{
		cell val = *(inptr);
		inptr++;
		return val;
	}
	inline cell peek_cell()
	{
		return *inptr;
	}
	inline cell *read_cellptr()
	{
		cell *val = *(cell **)(inptr);
		inptr++;
		return val;
	}
	inline void write_ubyte(unsigned char c)
	{
		if (outbase)
		{
			*outptr = c;
		}
		outptr++;
	}
	inline void write_ushort(unsigned short c)
	{
		if (outbase)
		{
			*(unsigned short *)outptr = c;
		}
		outptr += sizeof(unsigned short);
	}
	inline void write_byte(char c)
	{
		if (outbase)
		{
			*outptr = c;
		}
		outptr++;
	}
	inline void write_int32(int c)
	{
		if (outbase)
		{
			*(int *)outptr = c;
		}
		outptr += sizeof(int);
	}
	inline void write_uint32(unsigned int c)
	{
		if (outbase)
		{
			*(unsigned int *)outptr = c;
		}
		outptr += sizeof(unsigned int);
	}
	inline unsigned int get_outputpos()
	{
		return (outptr - outbase);
	}
	inline void set_outputpos(unsigned int offs)
	{
		outptr = outbase + offs;
	}
	inline unsigned int get_inputpos()
	{
		return (unsigned int)((char *)inptr - (char *)inbase);
	}
public:
	cell *inptr;		/* input pointer */
	cell *inbase;		/* input base */
	char *outbase;	/* output pointer */
	char *outptr;	/* output base */
};

#define IA32_JMP_IMM32	0xE9	// encoding is imm32

inline unsigned int IA32_Jump_Imm32(JitWriter *jit, int disp)
{
	unsigned int ptr;
	jit->write_ubyte(IA32_JMP_IMM32);
	ptr = jit->get_outputpos();
	jit->write_int32(disp);
	return ptr;
}

/**
* Corrects a jump using an absolute offset, not a relative one.
*/
inline void IA32_Write_Jump32_Abs(JitWriter *jit, unsigned int jmp, void *target)
{
	//save old ptr
	char *oldptr = jit->outptr;
	//get relative difference
	long diff = ((long)target - ((long)jit->outbase + jmp + 4));
	//overwrite old value
	jit->outptr = jit->outbase + jmp;
	jit->write_int32(diff);
	//restore old ptr
	jit->outptr = oldptr;
}


#endif // _INCLUDE_SOURCEMOD_DETOURS_H_
