#pragma once
#ifndef WIN32
#define WIN32
#endif
#include <WinSock2.h>
#include <string>
#include <cstdio>

#include "Util\WindowsConsole.hpp"
#include "Util\KeyboardHook.hpp"
#include "WowStuff\WowConsole.hpp"
#include "Manager.hpp"
#include "NetClientProcessDetour.hpp"
#include "ReadChatDetour.hpp"
#include "WowClientDB.hpp"
#include "Memory\AntiWarden.hpp"

HINSTANCE instanceDLL = nullptr;
HWND wowWindow = nullptr;

DWORD MainThreadControl(LPVOID /* param */);
LRESULT WINAPI KeyboardHook(int/* nCode*/, WPARAM/* wParam*/, LPARAM/* lParam*/);

// entry point of the DLL
BOOL APIENTRY DllMain(HINSTANCE instDLL, DWORD reason, LPVOID /* reserved */)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		instanceDLL = instDLL;

		// disables thread notifications (DLL_THREAD_ATTACH, DLL_THREAD_DETACH)
		DisableThreadLibraryCalls(instDLL);	

		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&MainThreadControl, NULL, 0, NULL);		
	}	
	else if (reason == DLL_PROCESS_DETACH)
	{		
		WindowsConsole::Destroy();		
	}
	return TRUE;
}

int endScenePointer{};
int resetPointer{};
void GetDevicePointers();

void WINAPI MessageLoop(LPVOID lpParm)
{
	hKeyHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardHook, instanceDLL, 0);
	MSG message;
	GetMessage(&message, NULL, 0, 0);	
}

bool CCommand_TestCommand(char const* cmd, char const* args)
{
	ConsoleWriteA("Hello from TestCommand: cmd '%s', args '%s'", INPUT_COLOR, cmd, args);
	return true;
}

auto __cdecl LuaUnitInLos(int state)
{
	if (FramescriptIsString(state, 1))
	{
		char* unit = FramescriptToLstring(state, 1, 0);
		int addr = GetPtrFromUnitId(unit);
		if (addr && GetLocalPlayer())
		{
			if (Object(addr).InLos())
				FrameScriptPushNumber(state, 1);
			else
				FrameScriptPushNumber(state, 0);
		}
		else
			FrameScriptPushNil(state);
	}
	else
		FrameScriptDisplayError(state, "Usage: UnitInLos(\"unit\")");

	return 1;
}

auto luaCommandsRegistered{ false };
auto LoadScriptFunctionsDetour()
{
	FramescriptRegister("UnitInLos", (int)LuaUnitInLos);
	luaCommandsRegistered = true;

	//---------------- return to the original function ----------------
	auto det = g_Detours["LoadScriptFunctions"];
	det->Restore();
	int res = ((int(__cdecl*)())det->target)();
	det->Apply();
	return res;
}

DWORD MainThreadControl(LPVOID lpParm)
{	
	g_Detours["WardenDataHandler"] = new Detour(0x007DA850, (int)SMSG_WARDEN_DATA_HandlerDetour);

	WindowsConsole::Create();	
	Sleep(200);

	HWND wowWindow = FindWindow(0, "World of Warcraft");
	SetForegroundWindow(wowWindow);

	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&MessageLoop, NULL, 0, NULL);

	GetDevicePointers();
	printf("End Scene Pointer = %X\n", endScenePointer); 
	printf("Reset Pointer = %X\n", resetPointer);

	g_Detours["EndScene"] = new Detour(endScenePointer, (int)EndSceneDetour);
	g_Detours["Reset"] = new Detour(resetPointer, (int)ResetDetour);
	//g_Detours["OnKeyUp"] = new Detour(0x00763BE0, (int)OnKeyUpDetour);	
	g_Detours["NetClientProcess"] = new Detour(0x00631FE0, (int)NetClientProcessDetour);
	g_Detours["ReadChat"] = new Detour(0x00966580, (int)ReadChatDetour);	
	g_Detours["LoadScriptFunctions"] = new Detour(0x005120E0, (int)LoadScriptFunctionsDetour);
	
	if(!luaCommandsRegistered && *(int*)0x00BD091C)
		FramescriptRegister("UnitInLos", (int)LuaUnitInLos);

	InitDBTables();

	EnableWowConsole();
	// Fix InvalidPtrCheck for callbacks outside of .text section
	*(int*)0x00D415B8 = 1;
	*(int*)0x00D415BC = 0x7FFFFFFF;
	ConsoleWrite("test", DEFAULT_COLOR);	
	RegisterCommand("testcmd", CCommand_TestCommand, CATEGORY_DEBUG, "Test help string");
	//ShowConsole();	

	//-----------------------------------  loop here before exit ----------------------------------
	//wait for the numpad 7 to be pressed...
	while (!should_exit) { }

	// exit
	UnregisterCommand("testcmd");
	FramescriptUnregister("UnitInLos");

	if (hKeyHook)	
		UnhookWindowsHookEx(hKeyHook);

	//tell the enscene hook to finish
	shouldRemoveEndSceneInjection = true;

	printf("Exiting...\n");

	//wait the endscene hook to finish...
	while (!endSceneUnhooked) { }

	// unhooks all detours we created
	for (auto& det : g_Detours)
	{
		delete det.second;
	}
	g_Detours.clear();
	// shutdowns the bot
	// note: after that DLL's entry point will be called with reason DLL_PROCESS_DETACH
	FreeLibraryAndExitThread((HMODULE)instanceDLL, 0);
	return 0;
}

void GetDevicePointers()
{
	// Device
	int ptr;
	{
		ptr = *(int*)0x00C5DF88;
	}while (ptr == 0);

	{
		ptr = *(int*)(ptr + 0x397C);
	}while (ptr == 0);

	// Scene	
	{
		ptr = *(int*)ptr;
	}while (ptr == 0);

	{
		endScenePointer = *(int*)(ptr + 0xA8);   //42 * 4	
	}while (endScenePointer == 0);

	{
		resetPointer = *(int*)(ptr + 0x40);   //16 * 4	
	}while (resetPointer == 0);
}








































//hKeyHook = SetWindowsHookEx(WH_KEYBOARD, (HOOKPROC)KeyboardHook, instanceDLL, 0);
//if (hKeyHook)
//	UnhookWindowsHookEx(hKeyHook);


//int countt;
//int __stdcall EndSceneDetour(int device)
//{
//	if (countt++ % 10 == 0)
//	{
//		//MessageBox(HWND_DESKTOP, "Text here", "Text here", MB_OK);
//		printf("count = %d\n", countt);
//		if (countt < 30)
//		{
//			((void(__cdecl*)(const char*, const char*, void*))0x00819210)("print('aaaaaaaaaaa')", "LetsDance", nullptr);
//		}
//	}
//	auto det = g_Detours["EndScene"];
//	det->Restore();
//	int res = ((int(__stdcall*)(int))det->GetOrig())(device);
//	det->Apply();
//	return res;
//}