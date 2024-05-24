#include "MenuHook.h"
#include "../../Features/Menu/Menu.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Prepares the menu for unloading
void MenuHook::Unload()
{
	F::Menu.IsOpen = true;
	F::Menu.Unload = true;
}

MAKE_HOOK(Direct3DDevice9_Present, Utils::GetVFuncPtr(I::DirectXDevice, 17), HRESULT,
	__stdcall, IDirect3DDevice9* pDevice, const RECT* pSource, const RECT* pDestination, HWND hWindowOverride,
	const RGNDATA* pDirtyRegion)
{
	F::Menu.Render(pDevice);

	return Hook.Original<FN>()(pDevice, pSource, pDestination, hWindowOverride, pDirtyRegion);
}

MAKE_HOOK(Direct3DDevice9_Reset, Utils::GetVFuncPtr(I::DirectXDevice, 16), HRESULT, __stdcall,
	LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
	const HRESULT Original = Hook.Original<FN>()(pDevice, pPresentationParameters);
	ImGui_ImplDX9_CreateDeviceObjects();
	return Original;
}

LONG __stdcall WndProc::Func(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (F::Menu.IsOpen)
	{
		ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam); // CRASH: seemingly random assert with imgui, if anyone is able to consistently replicate this lmk
		if (ImGui::GetIO().WantTextInput || G::InKeybind)
		{
			I::InputSystem->ResetInputStateVFunc();
			return 1;
		}

		if (uMsg >= WM_MOUSEFIRST && WM_MOUSELAST >= uMsg)
			return 1;
	}

	return CallWindowProc(Original, hWnd, uMsg, wParam, lParam);
}

MAKE_HOOK(VGuiSurface_LockCursor, Utils::GetVFuncPtr(I::MatSystemSurface, 62), void, __fastcall,
	void* ecx, void* edx)
{
	if (F::Menu.IsOpen)
		return I::MatSystemSurface->UnlockCursor();

	Hook.Original<FN>()(ecx, edx);
}

MAKE_HOOK(VGuiSurface_SetCursor, Utils::GetVFuncPtr(I::MatSystemSurface, 51), void, __fastcall,
	void* ecx, void* edx, HCursor cursor)
{
	if (F::Menu.IsOpen)
	{
		switch (F::Menu.Cursor)
		{
		case 0: cursor = 2; break;
		case 1: cursor = 3; break;
		case 2: cursor = 12; break;
		case 3: cursor = 11; break;
		case 4: cursor = 10; break;
		case 5: cursor = 9; break;
		case 6: cursor = 8; break;
		case 7: cursor = 14; break;
		case 8: cursor = 13; break;
		}
		return Hook.Original<FN>()(ecx, edx, cursor);
	}

	Hook.Original<FN>()(ecx, edx, cursor);
}

void WndProc::Init()
{
	while (!hwWindow)
		hwWindow = FindWindowW(nullptr, L"Team Fortress 2");

	Original = reinterpret_cast<WNDPROC>(SetWindowLongPtr(hwWindow, GWL_WNDPROC, reinterpret_cast<LONG_PTR>(Func)));
}

void WndProc::Unload()
{
	SetWindowLongPtr(hwWindow, GWL_WNDPROC, reinterpret_cast<LONG_PTR>(Original));
}