#pragma once
#include "Helpers.hpp"
#include "..\EndsceneHandler.hpp"
#include <Windows.h>

HHOOK hKeyHook = nullptr;

LRESULT CALLBACK KeyboardHook(int nCode, WPARAM wParam, LPARAM lParam)
{
	if ((nCode == HC_ACTION) && ((wParam == WM_SYSKEYDOWN) || (wParam == WM_KEYDOWN)))
	{

	}

	if ((nCode == HC_ACTION) && ((wParam == WM_SYSKEYUP) || (wParam == WM_KEYUP)))
	{
		auto key = ((KBDLLHOOKSTRUCT*)lParam)->vkCode;

		//printf("%X", key);

		if (key == VK_NUMPAD2)
			do_something = true;

		if (key == VK_NUMPAD7)
			should_exit = true;
	}

	return CallNextHookEx(hKeyHook, nCode, wParam, lParam);
}

bool __cdecl OnKeyUpDetour(int* key)
{	
	//chat(tohex(*key));
	
	/*if (*key == 0x107)
	{
		should_exit = true;
		
	}*/

	//---------------- return to the original function ----------------
	auto det = g_memops["OnKeyUp"];
	det->Restore(); 
	auto res = ((bool(__cdecl*)(int*))det->target)(key);
	det->Apply();
	return res;
}