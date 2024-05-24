#pragma once

#include <Windows.h>
#include <unordered_map>
#include <chrono>

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
	void StoreKey(int iKey, KeyStorage* pStorage = nullptr)
	{
		if (!pStorage)
			pStorage = &StorageMap[iKey];

		// down
		bool bDown = GetAsyncKeyState(iKey) & 0x8000;
		if (bDown)
		{ //Utils::IsGameWindowInFocus()
			static HWND hwGame = nullptr;

			while (!hwGame)
			{
				hwGame = FindWindowW(nullptr, L"Team Fortress 2");
				if (!hwGame)
					bDown = false;
			}

			if (GetForegroundWindow() != hwGame)
				bDown = false;
		}
		if (!iKey)
			bDown = false;

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
	bool Down(int iKey, const bool bStore = true, KeyStorage* pStorage = nullptr)
	{
		if (!pStorage)
			pStorage = &StorageMap[iKey];

		if (bStore)
			StoreKey(iKey, pStorage);
		
		return pStorage->bIsDown;
	}

	// Was the button just pressed? This will only be true once.
	bool Pressed(int iKey, const bool bStore = true, KeyStorage* pStorage = nullptr)
	{
		if (!pStorage)
			pStorage = &StorageMap[iKey];

		if (bStore)
			StoreKey(iKey, pStorage);

		return pStorage->bIsPressed;
	}

	// Was the button double clicked? This will only be true once.
	bool Double(int iKey, const bool bStore = true, KeyStorage* pStorage = nullptr)
	{
		if (!pStorage)
			pStorage = &StorageMap[iKey];

		if (bStore)
			StoreKey(iKey, pStorage);

		return pStorage->bIsDouble;
	}

	// Was the button just released? This will only be true once.
	bool Released(int iKey, const bool bStore = true, KeyStorage* pStorage = nullptr)
	{
		if (!pStorage)
			pStorage = &StorageMap[iKey];

		if (bStore)
			StoreKey(iKey, pStorage);

		return pStorage->bIsReleased;
	}
};

namespace F {
	inline CKeyHandler KeyHandler;
}