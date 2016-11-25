#pragma once
#include "Wowtypes.h"

inline uint64 GetLocalPlayerGuid()								//similar to GetPlayerGuid() in OM.hpp
{
	return ((uint64(__cdecl*)())(0x004D3790))();				//ClntObjMgrGetActivePlayerGuid()
}

inline int GetLocalPlayer()										//similar to GetPlayerAddr() in OM.hpp
{	
	uint64 lpguid = GetLocalPlayerGuid();
	return lpguid? ((int(__cdecl*)(long long, int))0x004D4DB0)(lpguid, TYPEMASK_PLAYER) : 0;
}

inline int GetObjectByGuidTypeCheck(uint64 guid, int typemask = -1)
{
	return  guid ? ((int(__cdecl*)(uint64, int))0x004D4DB0)(guid, typemask) : 0;
}

inline int PerformanceCount() { return ((int(__cdecl*)())0x0086AE20)(); }
