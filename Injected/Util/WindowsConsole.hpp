#pragma once

class WindowsConsole
{
public:
	
	static bool Create()
	{
		
		if (!AllocConsole())
			return false;
		
		HWND consoleWindowHandler = GetConsoleWindow();

		// disables the "close button" on the console		
		EnableMenuItem(GetSystemMenu(consoleWindowHandler, FALSE), SC_CLOSE, MF_GRAYED);

		// just be sure to display the grayed (and disabled) close button
		DrawMenuBar(consoleWindowHandler); // re-draw
		
		HANDLE standardOutputHandler = GetStdHandle(STD_OUTPUT_HANDLE);

		if (!standardOutputHandler || standardOutputHandler == INVALID_HANDLE_VALUE)
			return false;
		
		SetConsoleTitle("CPPBOT");

		// re-opens STDOUT handle as a console window output
		FILE* stream;		
		freopen_s(&stream, "CONOUT$", "w", stdout);	

		//ShowWindow(consoleWindowHandler, SW_SHOWMINIMIZED);

		return true;
	}
	
	static void Destroy() { FreeConsole(); }	
};
