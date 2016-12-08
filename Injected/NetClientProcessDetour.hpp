#pragma once
#include "WowStuff\Opcodes.h"
#include "DataStore.hpp"

//it's a a thiscall
int __fastcall NetClientProcessDetour(int connection, int EDX_Dummy, int a2, CDataStore* data, int a4)
{
	auto ds = DataStore(data);
	uint16 opcode = ds.ReadFake<uint16>();

	if (opcode == SMSG_ATTACKSTART)
	{
		chat("dude, SMSG_ATTACKSTART");
	}

	//---------------- return to the original function ----------------
	auto det = g_memops["NetClientProcess"];
	det->Restore();
	int res = ((int(__thiscall*)(int, int, CDataStore*, int))det->target)(connection, a2, data, a4);
	det->Apply();
	return res;
}