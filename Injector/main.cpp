#include <iostream>
#include <Windows.h>

#pragma comment(lib, "Shlwapi.lib")

// this DLL will be injected
const char injectDLLName[] = "Injected.dll";
//const char injectDLLName[] = "bwh.dll";

HINSTANCE hInst;

int  InjectDll(DWORD hProcess, const char* dllLocation);

//-----------------------------------------------
// WinMain
//
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine,	int nCmdShow)
{	
	hInst = hInstance;
	
	HWND windowHandle = FindWindowW(NULL, L"World of Warcraft");
	
	if (windowHandle == NULL)
	{		
		// additional information
		STARTUPINFO si;
		PROCESS_INFORMATION pi;

		// set the size of the structures
		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&pi, sizeof(pi));

		// start the program up
		CreateProcess("c:\\WOTLK\\wow.exe",   // the path
			"",				// Command line
			NULL,           // Process handle not inheritable
			NULL,           // Thread handle not inheritable
			FALSE,          // Set handle inheritance to FALSE
			0,              // No creation flags
			NULL,           // Use parent's environment block
			NULL,           // Use parent's starting directory 
			&si,            // Pointer to STARTUPINFO structure
			&pi				// Pointer to PROCESS_INFORMATION structure
			);
			// Close process and thread handles. 
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
	}

	while (!(windowHandle = FindWindowW(NULL, L"World of Warcraft")))
	{

	} 
	DWORD pid = 0;
	DWORD* ppid = &pid;
	GetWindowThreadProcessId(windowHandle, ppid);

	char path [MAX_PATH];	
	DWORD injectorPathSize = GetModuleFileName(NULL, path, MAX_PATH);
	char* pSlash = strrchr(path, '\\');
	strcpy_s(pSlash + 1, sizeof(injectDLLName), injectDLLName);
	InjectDll(pid, path);
	return 0;
}


int InjectDll(DWORD pid, const char* dllNameArray)
{
	HANDLE hProcess = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ, FALSE, pid);
	if (hProcess == NULL)
		return 1;		

	void* pAllocDllName = VirtualAllocEx(hProcess, NULL, strlen(dllNameArray), MEM_COMMIT, PAGE_READWRITE);
	WriteProcessMemory(hProcess, pAllocDllName, dllNameArray, strlen(dllNameArray), NULL);

	HMODULE hKernel32 = GetModuleHandle("Kernel32");
	HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)GetProcAddress(hKernel32, "LoadLibraryA"), pAllocDllName, 0, NULL);
	DWORD exitCode = 0;
	if (hThread != NULL)
	{
		WaitForSingleObject(hThread, INFINITE);
		GetExitCodeThread(hThread, &exitCode);
		CloseHandle(hThread);
	}

	VirtualFreeEx(hProcess, pAllocDllName, strlen(dllNameArray), MEM_RELEASE);

	/*hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)GetProcAddress(hKernel32, "FreeLibrary"), (void*)exitCode, 0, NULL);
	if (hThread == NULL)
		return false;

	WaitForSingleObject(hThread, INFINITE);
	GetExitCodeThread(hThread, &exitCode);*/
	//CloseHandle(hThread);

	return exitCode;
}