#pragma once

enum CommandCategory
{
	CATEGORY_DEBUG = 0x0,
	CATEGORY_GRAPHICS = 0x1,
	CATEGORY_CONSOLE = 0x2,
	CATEGORY_COMBAT = 0x3,
	CATEGORY_GAME = 0x4,
	CATEGORY_DEFAULT = 0x5,
	CATEGORY_NET = 0x6,
	CATEGORY_SOUND = 0x7,
	CATEGORY_GM = 0x8,
	NUM_CMD_CATEGORIES = 0x9
};

enum COLOR_T
{
	DEFAULT_COLOR = 0x0,
	INPUT_COLOR = 0x1,
	ECHO_COLOR = 0x2,
	ERROR_COLOR = 0x3,
	WARNING_COLOR = 0x4,
	GLOBAL_COLOR = 0x5,
	ADMIN_COLOR = 0x6,
	HIGHLIGHT_COLOR = 0x7,
	BACKGROUND_COLOR = 0x8,
	NUM_COLORTYPES = 0x9,
};

typedef bool (*CommadHandler)(char const* cmd, char const* args);

static auto const ConsoleWriteA = reinterpret_cast<void(__cdecl*)(const char *fmt, COLOR_T color, ...)>(0x00765360);

inline bool RegisterCommand(char const* command, CommadHandler handler, CommandCategory category, char const* help)
{
	return ((bool(__cdecl*)(char const*, CommadHandler, CommandCategory, char const*))0x00769100)(command, handler, category, help);
}

inline void UnregisterCommand(char const* command)
{
	((void(__cdecl*)(char const*))0x007689E0)(command);
}

inline void EnableWowConsole()
{
	*(int*)0x00CABCC4 = 1;
}

inline void ShowConsole()
{
	*(int*)0x00CA1978 = 1;
}

inline void HideConsole()
{
	*(int*)0x00CA1978 = 0;
}

inline void ConsoleWrite(const char *text, COLOR_T color)
{
	((void(__stdcall*)(const char*, COLOR_T))(0x00765270))(text, color);
}