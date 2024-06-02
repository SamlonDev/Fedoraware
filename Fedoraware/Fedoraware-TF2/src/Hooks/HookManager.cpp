#ifndef HOOK_MANAGER_H
#define HOOK_MANAGER_H

#include <map>
#include <ranges>
#include <vector>

#include "Hooks.h"
#include "../SDK/SDK.h"
#include "MenuHook/MenuHook.h"

class CHookManager;

class CHook
{
public:
	CHook(const std::string& name, void* pInitFunction);
	virtual ~CHook() = default;
	const std::string& GetName() const { return m_Name; }
	void Init();

private:
	std::string m_Name;
	void* m_InitFunction;
};

class CHookManager
{
public:
	CHookManager();
	~CHookManager();
	void Release() noexcept;
	void Init();
	const std::map<std::string, CHook*>& GetMapHooks() const { return m_MapHooks; }

private:
	std::map<std::string, CHook*> m_MapHooks;
};

inline uintptr_t GetVFuncPtr(void* pBaseClass, unsigned int nIndex)
{
	return static_cast<uintptr_t>((*static_cast<int**>(pBaseClass))[nIndex]);
}

#endif // HOOK_MANAGER_H

#include "HookManager.h"
#include <Windows.h>
#include <MinHook.h>

CHook::CHook(const std::string& name, void* pInitFunction)
{
	m_Name = name;
	m_InitFunction = pInitFunction;
	g_HookManager.GetMapHooks()[name] = this;
}

CHookManager::CHookManager()
{
}

CHookManager::~CHookManager()
{
	Release();
}

void CHookManager::Release() noexcept
{
	if (MH_Uninitialize() != MH_OK)
	{
		MessageBoxW(nullptr, L"MH failed to uninitialize!", L"ERROR!", MB_ICONERROR);
	}

	WndProc::Unload();
}

void CHookManager::Init()
{
	if (!I::DirectXDevice)
	{
		MessageBoxW(nullptr, L"DirectX device is not initialized!", L"ERROR!", MB_ICONERROR);
		return;
	}

	if (MH_Initialize() != MH_OK)
	{
		MessageBoxW(nullptr, L"MH failed to initialize!", L"ERROR!", MB_ICONERROR);
		return;
	}

	WndProc::Init();

	for (const auto& hook : GetMapHooks() | std::views::values)
	{
		hook->Init();
	}

	if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK)
	{
		MessageBoxW(nullptr, L"MH failed to enable all hooks!", L"ERROR!", MB_ICONERROR);
	}
}
