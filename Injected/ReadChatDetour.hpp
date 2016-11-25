#pragma once
#include "ChatCommandHandler.hpp"

int __stdcall ReadChatDetour(/*int instance,*/ int unk, int zero)
{
	//because the func is __thiscall, just patching for now
	int instance;
	_asm mov instance, ecx;
	//-----------------------------------------------------

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
	auto det = g_Detours["ReadChat"];
	det->Restore();
	int res = ((int(__thiscall*)(int, int, int))det->GetOrig())(instance, unk, zero);
	det->Apply();
	return res;
}