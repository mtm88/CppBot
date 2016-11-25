#pragma once
#include "WowStuff\Opcodes.h"
#include "DataStore.hpp"

int __stdcall NetClientProcessDetour(/*int connection, */int a2, CDataStore* data, int a4)
{
	//because the func is __thiscall, just patching for now
	int connection;
	_asm mov connection, ecx;
	//-----------------------------------------------------

	auto ds = DataStore(data);
	uint16 opcode = ds.ReadFake<uint16>();

	if (opcode == SMSG_ATTACKSTART)
	{
		chat("dude, SMSG_ATTACKSTART");
	}

	//---------------- return to the original function ----------------
	auto det = g_Detours["NetClientProcess"];
	det->Restore();
	int res = ((int(__thiscall*)(int, int, CDataStore*, int))det->GetOrig())(connection, a2, data, a4);
	det->Apply();
	return res;
}