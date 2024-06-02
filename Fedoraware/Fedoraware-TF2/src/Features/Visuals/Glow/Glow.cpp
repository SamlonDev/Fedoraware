#ifndef GLOW_H
#define GLOW_H

#include "Glow_Structs.h"

#include "../../Vars.h"
#include "../../Color.h"
#include "../../../Hooks/Hooks.h"
#include "../../../Hooks/HookManager.h"
#include "../Materials/Materials.h"
#include "../FakeAngle/FakeAngle.h"
#include "../../Backtrack/Backtrack.h"

using namespace Hooks;
using namespace Materials;
using namespace FakeAngle;
using namespace Backtrack;

Glow_t GetGlowStruct(bool stencil, bool blur, int stencilScale, int blurScale);
Color_t GetEntityDrawColor(CBaseEntity* pEntity, Colors relative);
bool GetGlow(CBaseEntity* pEntity, Glow_t* glow, Color_t* color);

Glow_t CGlow::GetStruct(bool Stencil, bool Blur, int StencilScale, int BlurScale)
{
	return Glow_t{
		Stencil,
		Blur,
		StencilScale,
		BlurScale
	};
}

Color_t CGlow::GetEntityDrawColor(CBaseEntity* pEntity, Colors relative)
{
	// Implement the GetEntityDrawColor function here
}

bool CGlow::GetGlow(CBaseEntity* pEntity, Glow_t* glow, Color_t* color)
{
	CBaseEntity* pLocal = g_EntityCache.GetLocal();
	if (!pLocal || pEntity->GetDormant() || !pEntity->ShouldDraw())
		return false;

	if (!Utils::IsOnScreen(pEntity, pEntity->GetWorldSpaceCenter()))
		return false;

	switch (pEntity->GetClassID())
	{
		// player glow
	case ETFClassID::CBasePlayer:
	case ETFClassID::CTFPlayer:
	{
		const bool bFriendly = pEntity->m_iTeamNum() == pLocal->m_iTeamNum();
		*glow = bFriendly
			? GetStruct(Vars::Glow::Friendly::Stencil.Value, Vars::Glow::Friendly::Blur.Value, Vars::Glow::Friendly::StencilScale.Value, Vars::Glow::Friendly::BlurScale.Value)
			: GetStruct(Vars::Glow::Enemy::Stencil.Value, Vars::Glow::Enemy::Blur.Value, Vars::Glow::Enemy::StencilScale.Value, Vars::Glow::Enemy::BlurScale.Value);
		*color = GetEntityDrawColor(pEntity, Vars::Colors::Relative.Value);
		return bFriendly ? Vars::Glow::Friendly::Players.Value : Vars::Glow::Enemy::Players.Value;
	}
	// ... other cases follow
	}
}

#endif // GLOW_H
