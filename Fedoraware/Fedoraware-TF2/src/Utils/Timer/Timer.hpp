#pragma once
#include <chrono>

/*
 *	Credits to cathook (nullifiedcat)
 */

class Timer {
private:
	std::chrono::steady_clock::time_point Last;

public:
	Timer()
	{
		Last = std::chrono::steady_clock::now();
	}

	[[nodiscard]] bool Check(unsigned ms) const
	{
		const auto currentTime = std::chrono::steady_clock::now();
		return std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - Last).count() >= ms;
	}

	bool Run(unsigned ms)
	{
		if (Check(ms))
		{
			Update();
			return true;
		}
		return false;
	}

	inline void Update()
	{
		Last = std::chrono::steady_clock::now();
	}
};