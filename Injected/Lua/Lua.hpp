#pragma once
#include <string>
#include <time.h>
#include <algorithm>
#include "WowStuff\Wowtypes.h"

inline void FramescriptExecute(const char* text)
{
	((void(__cdecl*)(const char*, const char*, void*))0x00819210)(text, "CppBot", nullptr);
}

inline auto FramescriptRegister(const char* command, const int func_pointer)
{
	return ((bool(__cdecl*)(const char*, int))0x00817F90)(command, func_pointer);
}

inline auto FramescriptUnregister(const char* command)
{
	return ((bool(__cdecl*)(const char*))0x00817FD0)(command);
}

inline auto FramescriptIsString(int state, int n_param)
{
	return ((bool(__cdecl*)(int, int))0x00817FD0)(state, n_param);
}

inline auto FramescriptToLstring(int state, int n_param, int zero = 0)
{
	return ((char*(__cdecl*)(int, int, int))0x0084E0E0)(state, n_param, zero);
}

inline auto FrameScriptPushNumber(int state, double number)
{
	return ((int(__cdecl*)(int, double))0x0084E2A0)(state, number);
}

inline auto FrameScriptPushNil(int state)
{
	return ((int(__cdecl*)(int))0x0084E280)(state);
}

inline auto FrameScriptDisplayError(int state, char* err, ...)
{
	return ((int(__cdecl*)(int, char*, ...))0x0084F280)(state, err);
}

inline auto GetPtrFromUnitId(const char* unit_id)
{
	return ((int(__cdecl*)(const char*))0x0060C1F0)(unit_id);
}

inline auto chat(std::string message)
{	
	long long lpguid = ((long long(__cdecl*)())(0x004D3790))();	//ClntObjMgrGetActivePlayerGuid()
	if (!(lpguid && ((int(__cdecl*)(long long, int))0x004D4DB0)(lpguid, TYPEMASK_PLAYER)))
		return;

	time_t rawtime;
	struct tm timeinfo;
	char buffer[80];

	time(&rawtime);
	localtime_s(&timeinfo, &rawtime);

	strftime(buffer, 80, "[%H:%M:%S] ", &timeinfo);	
	
	std::string taggedMessage = buffer + message;
	std::replace(taggedMessage.begin(), taggedMessage.end(), '\'', '_');
	std::string to_print = "print ('" + taggedMessage + "')";
	FramescriptExecute(to_print.c_str());
}