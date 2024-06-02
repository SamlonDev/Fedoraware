#pragma once
#include "../../SDK/SDK.h"
#include "../Hooks.h"

// Forward declare the classes
namespace MenuHook { class MenuHookImpl; }
namespace WndProc { class WndProcImpl; }

// Define the namespaces
namespace MenuHook
{
	void Init();
	void Unload();
}

namespace WndProc
{
	class WndProcImpl
	{
	public:
		HWND hwWindow;
		WNDPROC Original;

		LONG __stdcall Func(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		static void Init();
		static void Unload();
	};
}
