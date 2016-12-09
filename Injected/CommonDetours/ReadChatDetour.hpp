#pragma once
#include "..\ChatCommandHandler.hpp"

//it's a a thiscall
int __fastcall ReadChatDetour(int instance, int EDX_Dummy, int unk, int zero)
{
	if (unk != 0)
	{
		int ptr = *(int*)(instance + 0x2B4);

		if (ptr)
		{
			char* line = (char*)ptr;
			if (line && sizeof(line) > 1)
				ChatLine = line;
		}
	}

	//---------------- return to the original function ----------------
	auto det = g_memops["ReadChat"];
	det->Restore();
	int res = ((int(__thiscall*)(int, int, int))det->target)(instance, unk, zero);
	det->Apply();
	return res;
}