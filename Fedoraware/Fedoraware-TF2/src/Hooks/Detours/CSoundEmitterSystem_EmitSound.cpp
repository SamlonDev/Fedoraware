#include "../Hooks.h"
#include <boost/algorithm/string.hpp>

const static std::vector<std::string> NOISEMAKER_SOUNDS{ "items/halloween", "items/football_manager", "items/japan_fundraiser", "items/samurai", "items/summer", "misc/happy_birthday_tf", "misc/jingle_bells" };

MAKE_HOOK(CSoundEmitterSystem_EmitSound, S::CSoundEmitterSystem_EmitSound(), void, __fastcall,
	void* ecx, void* edx, IRecipientFilter& filter, int entindex, const EmitSound_t& ep)
{
	if (ep.m_pSoundName)
	{
		std::string soundName(ep.m_pSoundName);
		boost::algorithm::to_lower(soundName);

		// Footsteps
		if (Vars::Misc::Sound::Block.Value & (1 << 0))
		{
			if (soundName.find("footsteps") != std::string::npos)
				return;
		}

		// Noisemaker
		if (Vars::Misc::Sound::Block.Value & (1 << 1))
		{
			for (auto& sound : NOISEMAKER_SOUNDS)
			{
				if (soundName.find(sound) != std::string::npos)
					return;
			}
		}
	}

	return Hook.Original<FN>()(ecx, edx, filter, entindex, ep);
}