#pragma once

#include <Windows.h>
#include <unordered_map>
#include <chrono>

constexpr int MAX_KEYS = 256;

struct KeyStorage
{
	bool bIsDown = false;
	bool bIsPressed = false;
	bool bIsDouble = false;
	int iPressTime = 0;
	bool bIsReleased = false;
};

class CKeyHandler
{
	std::unordered_map<int, KeyStorage> StorageMap;

public:
	CKeyHandler()
	{
		StorageMap.reserve(MAX_KEYS);
	}

	void StoreKey(int iKey, KeyStorage* pStorage)
	{
		if (!pStorage || StorageMap.find(iKey) == StorageMap.end())
			return;

		// down
		bool bDown = GetAsyncKeyState(iKey) & 0x8000;
		if (bDown)
		{
			HWND hwGame = FindWindowW(nullptr, L"Team Fortress 2");
			if (!hwGame)
				return;

			if (GetForegroundWindow() != hwGame)
				bDown = false;
		}

		// pressed
		const bool bPressed = bDown && !pStorage->bIsDown;

		// double click
		const int iEpoch = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		const bool bDouble = bPressed && iEpoch < pStorage->iPressTime + 250;

		// released
		const bool bReleased = !bDown && pStorage->bIsDown;

		pStorage->bIsDown = bDown;
		pStorage->bIsPressed = bPressed;
		pStorage->bIsDouble = bDouble;
		pStorage->bIsReleased = bReleased;
		if (bPressed)
			pStorage->iPressTime = iEpoch;
	}

	// Is the button currently down?
	bool Down(int iKey, bool bStore = true, KeyStorage* pStorage = nullptr)
	{
		if (!pStorage)
			pStorage = &StorageMap[iKey];

		if (iKey < 0 || iKey >= MAX_KEYS)
			return false;

		if (bStore)
			StoreKey(iKey, pStorage);

		return pStorage->bIsDown;
	}

	// Was the button just pressed? This will only be true once.
	bool Pressed(int iKey, bool bStore = true, KeyStorage* pStorage = nullptr)
	{
		if (!pStorage)
			pStorage = &StorageMap[iKey];

		if (iKey < 0 || iKey >= MAX_KEYS)
			return false;

		if (bStore)
			StoreKey(iKey, pStorage);

		return pStorage->bIsPressed;
	}

	// Was the button double clicked? This will only be true once.
	bool Double(int iKey, bool bStore = true, KeyStorage* pStorage = nullptr)
	{
		if (!pStorage)
			pStorage = &StorageMap[iKey];

		if (iKey < 0 || iKey >= MAX_KEYS)
			return false;

		if (bStore)
			StoreKey(iKey, pStorage);

		return pStorage->bIsDouble;
	}

	// Was the button just released? This will only be true once.
	bool Released(int iKey, bool bStore = true, KeyStorage* pStorage = nullptr)
	{
		if (!pStorage)
			pStorage = &StorageMap[iKey];

		if (iKey < 0 || iKey >= MAX_KEYS)
			return false;

		if (bStore)
			StoreKey(iKey, pStorage);

		return pStorage->bIsReleased;
	}

	void Reset()
	{
		StorageMap.clear();
		StorageMap.reserve(MAX_KEYS);
	}
};

namespace F {
	inline CKeyHandler KeyHandler;
}
