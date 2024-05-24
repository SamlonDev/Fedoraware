#include "../Hooks.h"
#include "../../Features/TickHandler/TickHandler.h"

MAKE_HOOK(CL_Move, S::CL_Move(), void, __cdecl,
	float accumulated_extra_samples, bool bFinalTick)
{
	//Hook.Original<FN>();

	F::Ticks.CLMove(accumulated_extra_samples, bFinalTick);
	I::EngineClient->FireEvents();

	for (auto& Line : G::LinesStorage)
	{
		if (Line.m_flTime < 0.f)
			Line.m_flTime = std::min(Line.m_flTime + 1.f, 0.f);
	}
}