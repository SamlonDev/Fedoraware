#pragma once
#include <Windows.h>

class CPattern
{
	DWORD FindPattern(DWORD dwAddress, DWORD dwLength, LPCSTR szPattern);
	HMODULE GetModuleHandleSafe(LPCSTR szModuleName);

public:
	DWORD Find(LPCSTR szModuleName, LPCSTR szPattern);
};

inline CPattern g_Pattern;