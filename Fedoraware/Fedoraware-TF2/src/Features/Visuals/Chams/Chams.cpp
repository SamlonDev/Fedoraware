#include "Chams.h"

#include "../../Vars.h"
#include "../../../Hooks/Hooks.h"
#include "../../../Hooks/HookManager.h"
#include "../Materials/Materials.h"
#include "../FakeAngle/FakeAngle.h"
#include "../../Backtrack/Backtrack.h"

Chams_t CChams::GetStruct(std::vector<std::string> VisibleMaterial, std::vector<std::string> OccludedMaterial, Color_t VisibleColor, Color_t OccludedColor)
{
	return Chams_t{
		VisibleMaterial,
		OccludedMaterial,
		VisibleColor,
		OccludedColor
	};
}

bool CChams::GetChams(CBaseEntity* pEntity, Chams_t* pChams)
{
	CBaseEntity* pLocal = g_EntityCache.GetLocal();
	if (!pLocal || pEntity->GetDormant() || !pEntity->ShouldDraw())
		return false;

	if (!Utils::IsOnScreen(pEntity, pEntity->GetWorldSpaceCenter()))
		return false;

	switch (pEntity->GetClassID())
	{
	// player chams
	case ETFClassID::CBasePlayer:
	case ETFClassID::CTFPlayer:
	{
		const bool bFriendly = pEntity->m_iTeamNum() == pLocal->m_iTeamNum();
		*pChams = bFriendly
			? GetStruct(Vars::Chams::Friendly::VisibleMaterial.Value, Vars::Chams::Friendly::OccludedMaterial.Value, Vars::Chams::Friendly::VisibleColor.Value, Vars::Chams::Friendly::OccludedColor.Value)
			: GetStruct(Vars::Chams::Enemy::VisibleMaterial.Value, Vars::Chams::Enemy::OccludedMaterial.Value, Vars::Chams::Enemy::VisibleColor.Value, Vars::Chams::Enemy::OccludedColor.Value);
		return bFriendly ? Vars::Chams::Friendly::Players.Value : Vars::Chams::Enemy::Players.Value;
	}
	case ETFClassID::CTFWearable:
	{
		const auto& pOwner = I::ClientEntityList->GetClientEntityFromHandle(pEntity->m_hOwnerEntity());
		if (!pOwner)
			return false;

		const bool bFriendly = pOwner->m_iTeamNum() == pLocal->m_iTeamNum();
		*pChams = bFriendly
			? GetStruct(Vars::Chams::Friendly::VisibleMaterial.Value, Vars::Chams::Friendly::OccludedMaterial.Value, Vars::Chams::Friendly::VisibleColor.Value, Vars::Chams::Friendly::OccludedColor.Value)
			: GetStruct(Vars::Chams::Enemy::VisibleMaterial.Value, Vars::Chams::Enemy::OccludedMaterial.Value, Vars::Chams::Enemy::VisibleColor.Value, Vars::Chams::Enemy::OccludedColor.Value);
		return bFriendly ? Vars::Chams::Friendly::Players.Value : Vars::Chams::Enemy::Players.Value;
	}
	// building chams
	case ETFClassID::CObjectSentrygun:
	case ETFClassID::CObjectDispenser:
	case ETFClassID::CObjectTeleporter:
	{
		const bool bFriendly = pEntity->m_iTeamNum() == pLocal->m_iTeamNum();
		*pChams = bFriendly
			? GetStruct(Vars::Chams::Friendly::VisibleMaterial.Value, Vars::Chams::Friendly::OccludedMaterial.Value, Vars::Chams::Friendly::VisibleColor.Value, Vars::Chams::Friendly::OccludedColor.Value)
			: GetStruct(Vars::Chams::Enemy::VisibleMaterial.Value, Vars::Chams::Enemy::OccludedMaterial.Value, Vars::Chams::Enemy::VisibleColor.Value, Vars::Chams::Enemy::OccludedColor.Value);
		return bFriendly ? Vars::Chams::Friendly::Buildings.Value : Vars::Chams::Enemy::Buildings.Value;
	}
	// ragdoll chams
	case ETFClassID::CRagdollPropAttached:
	case ETFClassID::CRagdollProp:
	case ETFClassID::CTFRagdoll:
	{
		/*
		// don't interfere with ragdolls
		if (Vars::Visuals::Ragdolls::Type.Value)
		{
			if (Vars::Visuals::Ragdolls::EnemyOnly.Value && pEntity && pLocal && pEntity->m_iTeamNum() == pLocal->m_iTeamNum())
				return false;
			else
				return false;
		}
		*/
		const auto& pOwner = I::ClientEntityList->GetClientEntityFromHandle(pEntity->m_hPlayer());
		if (!pOwner)
			return false;
		
		const bool bFriendly = pOwner->m_iTeamNum() == pLocal->m_iTeamNum();
		*pChams = bFriendly
			? GetStruct(Vars::Chams::Friendly::VisibleMaterial.Value, Vars::Chams::Friendly::OccludedMaterial.Value, Vars::Chams::Friendly::VisibleColor.Value, Vars::Chams::Friendly::OccludedColor.Value)
			: GetStruct(Vars::Chams::Enemy::VisibleMaterial.Value, Vars::Chams::Enemy::OccludedMaterial.Value, Vars::Chams::Enemy::VisibleColor.Value, Vars::Chams::Enemy::OccludedColor.Value);
		return bFriendly ? Vars::Chams::Friendly::Ragdolls.Value : Vars::Chams::Enemy::Ragdolls.Value;
	}
	// projectile chams
	case ETFClassID::CTFProjectile_Rocket:
	case ETFClassID::CTFGrenadePipebombProjectile:
	case ETFClassID::CTFProjectile_Jar:
	case ETFClassID::CTFProjectile_JarGas:
	case ETFClassID::CTFProjectile_JarMilk:
	case ETFClassID::CTFProjectile_Arrow:
	case ETFClassID::CTFProjectile_SentryRocket:
	case ETFClassID::CTFProjectile_Flare:
	case ETFClassID::CTFProjectile_GrapplingHook:
	case ETFClassID::CTFProjectile_Cleaver:
	case ETFClassID::CTFProjectile_EnergyBall:
	case ETFClassID::CTFProjectile_EnergyRing:
	case ETFClassID::CTFProjectile_HealingBolt:
	case ETFClassID::CTFProjectile_ThrowableBreadMonster:
	case ETFClassID::CTFStunBall:
	case ETFClassID::CTFBall_Ornament:
	{
		const auto& pOwner = I::ClientEntityList->GetClientEntityFromHandle(pEntity->m_hThrower());
		if (!pOwner)
			return false;

		const bool bFriendly = pOwner->m_iTeamNum() == pLocal->m_iTeamNum();
		*pChams = bFriendly
			? GetStruct(Vars::Chams::Friendly::VisibleMaterial.Value, Vars::Chams::Friendly::OccludedMaterial.Value, Vars::Chams::Friendly::VisibleColor.Value, Vars::Chams::Friendly::OccludedColor.Value)
			: GetStruct(Vars::Chams::Enemy::VisibleMaterial.Value, Vars::Chams::Enemy::OccludedMaterial.Value, Vars::Chams::Enemy::VisibleColor.Value, Vars::Chams::Enemy::OccludedColor.Value);
		return bFriendly ? Vars::Chams::Friendly::Projectiles.Value : Vars::Chams::Enemy::Projectiles.Value;
	}
	// npc chams
	case ETFClassID::CHeadlessHatman:
	case ETFClassID::CTFTankBoss:
	case ETFClassID::CMerasmus:
	case ETFClassID::CZombie:
	case ETFClassID::CEyeballBoss:
		*pChams = GetStruct(Vars::Chams::World::VisibleMaterial.Value, Vars::Chams::World::OccludedMaterial.Value, Vars::Chams::World::VisibleColor.Value, Vars::Chams::World::OccludedColor.Value);
		return Vars::Chams::World::NPCs.Value;
	// pickup chams
	case ETFClassID::CTFAmmoPack:
	case ETFClassID::CCurrencyPack:
	case ETFClassID::CHalloweenGiftPickup:
		*pChams = GetStruct(Vars::Chams::World::VisibleMaterial.Value, Vars::Chams::World::OccludedMaterial.Value, Vars::Chams::World::VisibleColor.Value, Vars::Chams::World::OccludedColor.Value);
		return Vars::Chams::World::Pickups.Value;
	case ETFClassID::CBaseAnimating:
	{
		const auto szName = pEntity->GetModelName();
		if (Hash::IsAmmo(szName) || Hash::IsHealth(szName) || Hash::IsSpell(szName))
		{
			*pChams = GetStruct(Vars::Chams::World::VisibleMaterial.Value, Vars::Chams::World::OccludedMaterial.Value, Vars::Chams::World::VisibleColor.Value, Vars::Chams::World::OccludedColor.Value);
			return Vars::Chams::World::Pickups.Value;
		}
		break;
	}
	// bomb chams
	case ETFClassID::CTFPumpkinBomb:
	case ETFClassID::CTFGenericBomb:
		*pChams = GetStruct(Vars::Chams::World::VisibleMaterial.Value, Vars::Chams::World::OccludedMaterial.Value, Vars::Chams::World::VisibleColor.Value, Vars::Chams::World::OccludedColor.Value);
		return Vars::Chams::World::Bombs.Value;
	}

	// player chams
	if (const auto& pWeapon = reinterpret_cast<CBaseCombatWeapon*>(pEntity))
	{
		const auto& pOwner = I::ClientEntityList->GetClientEntityFromHandle(pWeapon->m_hOwnerEntity());
		if (!pOwner || !pOwner->IsPlayer())
			return false;

		const bool bFriendly = pOwner->m_iTeamNum() == pLocal->m_iTeamNum();
		*pChams = bFriendly
			? GetStruct(Vars::Chams::Friendly::VisibleMaterial.Value, Vars::Chams::Friendly::OccludedMaterial.Value, Vars::Chams::Friendly::VisibleColor.Value, Vars::Chams::Friendly::OccludedColor.Value)
			: GetStruct(Vars::Chams::Enemy::VisibleMaterial.Value, Vars::Chams::Enemy::OccludedMaterial.Value, Vars::Chams::Enemy::VisibleColor.Value, Vars::Chams::Enemy::OccludedColor.Value);
		return bFriendly ? Vars::Chams::Friendly::Players.Value : Vars::Chams::Enemy::Players.Value;
	}
	
	return false;
}

void CChams::StencilBegin(IMatRenderContext* pRenderContext, bool bTwoModels)
{
	if (!bTwoModels)
		return;
	
	pRenderContext->SetStencilEnable(true);
}
void CChams::StencilVisible(IMatRenderContext* pRenderContext, bool bTwoModels)
{
	if (!bTwoModels)
		return;

	pRenderContext->ClearBuffers(false, false, false);
	pRenderContext->SetStencilCompareFunction(STENCILCOMPARISONFUNCTION_ALWAYS);
	pRenderContext->SetStencilPassOperation(STENCILOPERATION_REPLACE);
	pRenderContext->SetStencilFailOperation(STENCILOPERATION_KEEP);
	pRenderContext->SetStencilZFailOperation(STENCILOPERATION_KEEP);
	pRenderContext->SetStencilReferenceValue(1);
	pRenderContext->SetStencilWriteMask(0xFF);
	pRenderContext->SetStencilTestMask(0x0);
}
void CChams::StencilOccluded(IMatRenderContext* pRenderContext)
{
	pRenderContext->ClearBuffers(false, false, false);
	pRenderContext->SetStencilCompareFunction(STENCILCOMPARISONFUNCTION_EQUAL);
	pRenderContext->SetStencilPassOperation(STENCILOPERATION_KEEP);
	pRenderContext->SetStencilFailOperation(STENCILOPERATION_KEEP);
	pRenderContext->SetStencilZFailOperation(STENCILOPERATION_KEEP);
	pRenderContext->SetStencilReferenceValue(0);
	pRenderContext->SetStencilWriteMask(0x0);
	pRenderContext->SetStencilTestMask(0xFF);
	pRenderContext->DepthRange(0.f, 0.2f);
}
void CChams::StencilEnd(IMatRenderContext* pRenderContext, bool bTwoModels)
{
	if (!bTwoModels)
		return;

	pRenderContext->SetStencilEnable(false);
	pRenderContext->DepthRange(0.f, 1.f);
}

void CChams::DrawModel(CBaseEntity* pEntity, Chams_t chams, IMatRenderContext* pRenderContext, bool bTwoModels)
{
	mEntities[pEntity->GetIndex()] = true;
	bRendering = true;

	auto visibleMaterials = chams.VisibleMaterial.size() ? chams.VisibleMaterial : std::vector<std::string>{ "None" };
	auto occludedMaterials = chams.OccludedMaterial.size() ? chams.OccludedMaterial : std::vector<std::string>{ "None" };

	StencilBegin(pRenderContext, bTwoModels);

	StencilVisible(pRenderContext, bTwoModels);
	for (auto it = visibleMaterials.begin(); it != visibleMaterials.end(); it++)
	{
		auto material = F::Materials.GetMaterial(*it);

		F::Materials.SetColor(material, chams.VisibleColor, it + 1 == visibleMaterials.end()); // only apply color to last material
		I::ModelRender->ForcedMaterialOverride(material ? material : nullptr);
		pEntity->DrawModel(STUDIO_RENDER);
	}
	if (bTwoModels)
	{
		StencilOccluded(pRenderContext);
		for (auto it = occludedMaterials.begin(); it != occludedMaterials.end(); it++)
		{
			auto material = F::Materials.GetMaterial(*it);

			F::Materials.SetColor(material, chams.OccludedColor, it + 1 == occludedMaterials.end());
			I::ModelRender->ForcedMaterialOverride(material ? material : nullptr);
			pEntity->DrawModel(STUDIO_RENDER);
		}
	}

	StencilEnd(pRenderContext, bTwoModels);
	I::RenderView->SetColorModulation(1.f, 1.f, 1.f);
	I::RenderView->SetBlend(1.f);
	I::ModelRender->ForcedMaterialOverride(nullptr);

	bRendering = false;
}



void CChams::RenderMain()
{
	mEntities.clear();

	const auto pRenderContext = I::MaterialSystem->GetRenderContext();
	if (!pRenderContext)
		return;

	// main
	for (int n = 1; n < I::ClientEntityList->GetHighestEntityIndex(); n++)
	{
		CBaseEntity* pEntity = I::ClientEntityList->GetClientEntity(n);
		if (!pEntity)
			continue;

		Chams_t chams = {};
		const bool bShouldDraw = GetChams(pEntity, &chams);

		if (bShouldDraw)
			DrawModel(pEntity, chams, pRenderContext);
	}

	// backtrack / fakeangle
	for (int n = 1; n <= I::EngineClient->GetMaxClients(); n++)
	{
		CBaseEntity* pEntity = I::ClientEntityList->GetClientEntity(n);
		if (!pEntity || !pEntity->IsPlayer() || !pEntity->ShouldDraw() || pEntity->GetDormant())
			continue;

		bRendering = bExtra = true;

		const float flOldInvisibility = pEntity->IsPlayer() ? pEntity->m_flInvisibility() : -1.f;
		if (flOldInvisibility > 0.999f)
			pEntity->m_flInvisibility() = 0.f;

		pEntity->DrawModel(STUDIO_RENDER);

		if (flOldInvisibility > 0.999f)
			pEntity->m_flInvisibility() = flOldInvisibility;

		bRendering = bExtra = false;
	}
}

void CChams::RenderBacktrack(const DrawModelState_t& pState, const ModelRenderInfo_t& pInfo, matrix3x4* pBoneToWorld)
{
	if (!Vars::Backtrack::Enabled.Value || !Vars::Chams::Backtrack::Active.Value)
		return;

	const auto ModelRender_DrawModelExecute = g_HookManager.GetMapHooks()["ModelRender_DrawModelExecute"];
	if (!ModelRender_DrawModelExecute)
		return;

	const auto& pEntity = I::ClientEntityList->GetClientEntity(pInfo.m_nEntIndex);
	if (!pEntity || pEntity->GetClassID() != ETFClassID::CTFPlayer || !pEntity->IsAlive())
		return;

	const auto& pLocal = g_EntityCache.GetLocal();
	const auto& pWeapon = g_EntityCache.GetWeapon();
	if (!pLocal || !pWeapon)
		return;
	if (G::CurWeaponType == EWeaponType::PROJECTILE)
		return;
	if (pEntity == pLocal ||
		pWeapon->m_iItemDefinitionIndex() != Soldier_t_TheDisciplinaryAction && pWeapon->GetWeaponID() != TF_WEAPON_MEDIGUN && pEntity->m_iTeamNum() == pLocal->m_iTeamNum() ||
		pWeapon->GetWeaponID() == TF_WEAPON_MEDIGUN && pEntity->m_iTeamNum() != pLocal->m_iTeamNum())
		return;



	auto drawModel = [ModelRender_DrawModelExecute, pLocal](Vec3 vCenter, std::vector<std::string> materials, Color_t color, const DrawModelState_t& pState, const ModelRenderInfo_t& pInfo, matrix3x4* pBoneToWorld)
		{
			if (!Utils::IsOnScreen(pLocal, vCenter))
				return;

			for (auto it = materials.begin(); it != materials.end(); it++)
			{
				auto material = F::Materials.GetMaterial(*it);

				F::Materials.SetColor(material, color, it + 1 == materials.end());
				I::ModelRender->ForcedMaterialOverride(material ? material : nullptr);
				ModelRender_DrawModelExecute->Original<void(__thiscall*)(CModelRender*, const DrawModelState_t&, const ModelRenderInfo_t&, matrix3x4*)>()(I::ModelRender, pState, pInfo, pBoneToWorld);
			}

			I::RenderView->SetColorModulation(1.f, 1.f, 1.f);
			I::RenderView->SetBlend(1.f);
			I::ModelRender->ForcedMaterialOverride(nullptr);
		};



	auto& vMaterials = Vars::Chams::Backtrack::VisibleMaterial.Value;
	auto& sColor = Vars::Chams::Backtrack::VisibleColor.Value;

	const auto& pRecords = F::Backtrack.GetRecords(pEntity);
	auto vRecords = F::Backtrack.GetValidRecords(pRecords);
	if (!vRecords.size())
		return;

	switch (Vars::Chams::Backtrack::Draw.Value)
	{
	case 0: // last
	{
		auto vLastRec = vRecords.end() - 1;
		if (vLastRec != vRecords.end() && pEntity->GetAbsOrigin().DistTo(vLastRec->vOrigin) > 0.1f)
			drawModel(vLastRec->vCenter, vMaterials, sColor, pState, pInfo, reinterpret_cast<matrix3x4*>(&vLastRec->BoneMatrix));
		break;
	}
	case 1: // last + first
	{
		auto vFirstRec = vRecords.begin();
		if (vFirstRec != vRecords.end() && pEntity->GetAbsOrigin().DistTo(vFirstRec->vOrigin) > 0.1f)
			drawModel(vFirstRec->vCenter, vMaterials, sColor, pState, pInfo, reinterpret_cast<matrix3x4*>(&vFirstRec->BoneMatrix));
		auto vLastRec = vRecords.end() - 1;
		if (vLastRec != vRecords.end() && pEntity->GetAbsOrigin().DistTo(vLastRec->vOrigin) > 0.1f)
			drawModel(vLastRec->vCenter, vMaterials, sColor, pState, pInfo, reinterpret_cast<matrix3x4*>(&vLastRec->BoneMatrix));
		break;
	}
	case 2: // all
	{
		for (auto& record : vRecords)
		{
			if (pEntity->GetAbsOrigin().DistTo(record.vOrigin) < 0.1f)
				continue;

			drawModel(record.vCenter, vMaterials, sColor, pState, pInfo, reinterpret_cast<matrix3x4*>(&record.BoneMatrix));
		}
	}
	}
}
void CChams::RenderFakeAngle(const DrawModelState_t& pState, const ModelRenderInfo_t& pInfo, matrix3x4* pBoneToWorld)
{
	if (!Vars::Chams::FakeAngle::Active.Value || pInfo.m_nEntIndex != I::EngineClient->GetLocalPlayer() || !F::FakeAngle.DrawChams || !F::FakeAngle.BonesSetup)
		return;

	const auto ModelRender_DrawModelExecute = g_HookManager.GetMapHooks()["ModelRender_DrawModelExecute"];
	if (!ModelRender_DrawModelExecute)
		return;



	auto& vMaterials = Vars::Chams::FakeAngle::VisibleMaterial.Value;
	auto& sColor = Vars::Chams::FakeAngle::VisibleColor.Value;

	for (auto it = vMaterials.begin(); it != vMaterials.end(); it++)
	{
		auto material = F::Materials.GetMaterial(*it);

		F::Materials.SetColor(material, sColor, it + 1 == vMaterials.end());
		I::ModelRender->ForcedMaterialOverride(material ? material : nullptr);
		ModelRender_DrawModelExecute->Original<void(__thiscall*)(CModelRender*, const DrawModelState_t&, const ModelRenderInfo_t&, matrix3x4*)>()(I::ModelRender, pState, pInfo, F::FakeAngle.BoneMatrix);
	}

	I::RenderView->SetColorModulation(1.f, 1.f, 1.f);
	I::RenderView->SetBlend(1.f);
	I::ModelRender->ForcedMaterialOverride(nullptr);
}
void CChams::RenderHandler(const DrawModelState_t& pState, const ModelRenderInfo_t& pInfo, matrix3x4* pBoneToWorld)
{
	if (!bExtra)
	{
		if (const auto ModelRender_DrawModelExecute = g_HookManager.GetMapHooks()["ModelRender_DrawModelExecute"])
			ModelRender_DrawModelExecute->Original<void(__thiscall*)(CModelRender*, const DrawModelState_t&, const ModelRenderInfo_t&, matrix3x4*)>()(I::ModelRender, pState, pInfo, pBoneToWorld);
	}
	else
	{
		RenderBacktrack(pState, pInfo, pBoneToWorld);
		RenderFakeAngle(pState, pInfo, pBoneToWorld);
	}
}

bool CChams::RenderViewmodel(void* ecx, int flags, int* iReturn)
{
	if (!Vars::Chams::Viewmodel::Weapon.Value)
		return false;

	const auto C_BaseAnimating_DrawModel = g_HookManager.GetMapHooks()["C_BaseAnimating_DrawModel"];
	if (!C_BaseAnimating_DrawModel)
		return false;



	auto& vMaterials = Vars::Chams::Viewmodel::VisibleMaterial.Value;
	auto& sColor = Vars::Chams::Viewmodel::VisibleColor.Value;

	for (auto it = vMaterials.begin(); it != vMaterials.end(); it++)
	{
		auto material = F::Materials.GetMaterial(*it);

		F::Materials.SetColor(material, sColor, it + 1 == vMaterials.end());
		I::ModelRender->ForcedMaterialOverride(material ? material : nullptr);
		*iReturn = C_BaseAnimating_DrawModel->Original<int(__thiscall*)(void*, int)>()(ecx, flags);
	}

	I::RenderView->SetColorModulation(1.f, 1.f, 1.f);
	I::RenderView->SetBlend(1.f);
	I::ModelRender->ForcedMaterialOverride(nullptr);

	return true;
}
bool CChams::RenderViewmodel(const DrawModelState_t& pState, const ModelRenderInfo_t& pInfo, matrix3x4* pBoneToWorld)
{
	if (!Vars::Chams::Viewmodel::Hands.Value)
		return false;

	const auto ModelRender_DrawModelExecute = g_HookManager.GetMapHooks()["ModelRender_DrawModelExecute"];
	if (!ModelRender_DrawModelExecute)
		return false;



	auto& vMaterials = Vars::Chams::Viewmodel::VisibleMaterial.Value;
	auto& sColor = Vars::Chams::Viewmodel::VisibleColor.Value;

	for (auto it = vMaterials.begin(); it != vMaterials.end(); it++)
	{
		auto material = F::Materials.GetMaterial(*it);

		F::Materials.SetColor(material, sColor, it + 1 == vMaterials.end());
		I::ModelRender->ForcedMaterialOverride(material ? material : nullptr);
		ModelRender_DrawModelExecute->Original<void(__thiscall*)(CModelRender*, const DrawModelState_t&, const ModelRenderInfo_t&, matrix3x4*)>()(I::ModelRender, pState, pInfo, pBoneToWorld);
	}

	I::RenderView->SetColorModulation(1.f, 1.f, 1.f);
	I::RenderView->SetBlend(1.f);
	I::ModelRender->ForcedMaterialOverride(nullptr);

	return true;
}