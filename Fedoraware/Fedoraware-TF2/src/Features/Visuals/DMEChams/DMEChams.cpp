// attempted two models with dme, but turned out inconsistent

#include "Chams.h"

#include "../../Vars.h"
#include "../../../Hooks/Hooks.h"
#include "../../../Hooks/HookManager.h"
#include "../Materials/Materials.h"
#include "../FakeAngleManager/FakeAng.h"
#include "../../Backtrack/Backtrack.h"

void CChams::SetupBegin(IMatRenderContext* pRenderContext, bool bTwoModels)
{
	if (!bTwoModels)
		return;
	
	pRenderContext->SetStencilEnable(true);
}
void CChams::SetupVisible(IMatRenderContext* pRenderContext, bool bTwoModels)
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
void CChams::SetupOccluded(IMatRenderContext* pRenderContext)
{
	pRenderContext->ClearBuffers(false, false, false);
	pRenderContext->SetStencilCompareFunction(STENCILCOMPARISONFUNCTION_EQUAL);
	pRenderContext->SetStencilPassOperation(STENCILOPERATION_KEEP);
	pRenderContext->SetStencilFailOperation(STENCILOPERATION_KEEP);
	pRenderContext->SetStencilZFailOperation(STENCILOPERATION_KEEP);
	pRenderContext->SetStencilReferenceValue(0);
	pRenderContext->SetStencilWriteMask(0x0);
	pRenderContext->SetStencilTestMask(0xFF);
	pRenderContext->DepthRange(Vars::Chams::debug::znear.Value, Vars::Chams::debug::zfar.Value);
}
void CChams::SetupEnd(IMatRenderContext* pRenderContext, bool bTwoModels)
{
	if (!bTwoModels)
		return;

	pRenderContext->SetStencilEnable(false);
	pRenderContext->DepthRange(0.f, 1.f);
}

bool CChams::GetChams(int iIndex, std::string_view szModelName, Chams_t* pChams, bool* bViewmodel)
{
	CBaseEntity* pLocal = g_EntityCache.GetLocal();
	if (!pLocal)
		return false;

	CBaseEntity* pEntity = I::ClientEntityList->GetClientEntity(iIndex);
	if (pEntity)
	{
		switch (pEntity->GetClassID())
		{
		// player chams
		case ETFClassID::CBasePlayer:
		case ETFClassID::CTFPlayer:
		{
			const bool bFriendly = pEntity->m_iTeamNum() == pLocal->m_iTeamNum();
			*pChams = bFriendly ? Vars::Chams::Friendly::Chams.Value : Vars::Chams::Enemy::Chams.Value;
			return bFriendly ? Vars::Chams::Friendly::Players.Value : Vars::Chams::Enemy::Players.Value;
		}
		case ETFClassID::CTFWearable:
		{
			const auto& pOwner = I::ClientEntityList->GetClientEntityFromHandle(pEntity->m_hOwnerEntity());
			if (!pOwner)
				return false;

			const bool bFriendly = pOwner->m_iTeamNum() == pLocal->m_iTeamNum();
			*pChams = bFriendly ? Vars::Chams::Friendly::Chams.Value : Vars::Chams::Enemy::Chams.Value;
			return bFriendly ? Vars::Chams::Friendly::Players.Value : Vars::Chams::Enemy::Players.Value;
		}
		// building chams
		case ETFClassID::CObjectSentrygun:
		case ETFClassID::CObjectDispenser:
		case ETFClassID::CObjectTeleporter:
		{
			const bool bFriendly = pEntity->m_iTeamNum() == pLocal->m_iTeamNum();
			*pChams = bFriendly ? Vars::Chams::Friendly::Chams.Value : Vars::Chams::Enemy::Chams.Value;
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
			const bool bFriendly = pEntity->m_iTeamNum() == pLocal->m_iTeamNum();
			*pChams = bFriendly ? Vars::Chams::Friendly::Chams.Value : Vars::Chams::Enemy::Chams.Value;
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
			*pChams = bFriendly ? Vars::Chams::Friendly::Chams.Value : Vars::Chams::Enemy::Chams.Value;
			return bFriendly ? Vars::Chams::Friendly::Projectiles.Value : Vars::Chams::Enemy::Projectiles.Value;
		}
		// npc chams
		case ETFClassID::CHeadlessHatman:
		case ETFClassID::CTFTankBoss:
		case ETFClassID::CMerasmus:
		case ETFClassID::CZombie:
		case ETFClassID::CEyeballBoss:
		// pickup chams
		case ETFClassID::CHalloweenGiftPickup:
		case ETFClassID::CTFAmmoPack:
			*pChams = Vars::Chams::World::Chams.Value;
			return Vars::Chams::World::Pickups.Value;
		case ETFClassID::CBaseAnimating:
		{
			const auto szName = pEntity->GetModelName();
			if (Hash::IsAmmo(szName) || Hash::IsHealth(szName) || Hash::IsSpell(szName))
			{
				*pChams = Vars::Chams::World::Chams.Value;
				return Vars::Chams::World::Pickups.Value;
			}
			break;
		}
		// bomb chams
		case ETFClassID::CTFPumpkinBomb:
		case ETFClassID::CTFGenericBomb:
			*pChams = Vars::Chams::World::Chams.Value;
			return Vars::Chams::World::Bombs.Value;
		// hands chams
		case ETFClassID::CTFViewModel:
			*pChams = Vars::Chams::Viewmodel::Chams.Value;
			return *bViewmodel = Vars::Chams::Viewmodel::Hands.Value;
		}

		// player chams
		if (const auto& pWeapon = reinterpret_cast<CBaseCombatWeapon*>(pEntity))
		{
			const auto& pOwner = I::ClientEntityList->GetClientEntityFromHandle(pWeapon->m_hOwnerEntity());
			if (!pOwner)
				return false;

			const bool bFriendly = pOwner->m_iTeamNum() == pLocal->m_iTeamNum();
			*pChams = bFriendly ? Vars::Chams::Friendly::Chams.Value : Vars::Chams::Enemy::Chams.Value;
			return bFriendly ? Vars::Chams::Friendly::Players.Value : Vars::Chams::Enemy::Players.Value;
		}
	}
	else
	{
		// weapon chams
		const bool bValid = szModelName.find("weapon") != std::string_view::npos
			&& szModelName.find("arrow") == std::string_view::npos
			&& szModelName.find("w_syringe") == std::string_view::npos
			&& szModelName.find("nail") == std::string_view::npos
			&& szModelName.find("shell") == std::string_view::npos
			&& szModelName.find("parachute") == std::string_view::npos
			&& szModelName.find("buffbanner") == std::string_view::npos
			&& szModelName.find("shogun_warbanner") == std::string_view::npos
			&& szModelName.find("targe") == std::string_view::npos // same as world model, can't filter
			&& szModelName.find("shield") == std::string_view::npos // same as world model, can't filter
			&& szModelName.find("repair_claw") == std::string_view::npos;
		if (!bValid)
			return false;

		*pChams = Vars::Chams::Viewmodel::Chams.Value;
		return *bViewmodel = Vars::Chams::Viewmodel::Weapon.Value;
	}
	
	return false;
}

void CChams::DrawModel(Chams_t chams, const DrawModelState_t& pState, const ModelRenderInfo_t& pInfo, matrix3x4* pBoneToWorld, bool bTwoModels, bool bViewmodel)
{
	const auto ModelRender_DrawModelExecute = g_HookManager.GetMapHooks()["ModelRender_DrawModelExecute"];
	const auto& pRenderContext = I::MaterialSystem->GetRenderContext();
	if (!ModelRender_DrawModelExecute || !pRenderContext)
		return;



	Utils::ConLog("draw", std::format("{}", I::ModelInfoClient->GetModelName(pInfo.m_pModel)).c_str());

	auto visibleMaterial = F::Materials.GetMaterial(chams.VisibleMaterial), occludedMaterial = F::Materials.GetMaterial(chams.OccludedMaterial);
	SetupBegin(pRenderContext, bTwoModels);

	SetupVisible(pRenderContext, bTwoModels);
	F::Materials.SetColor(visibleMaterial, chams.VisibleColor);
	I::ModelRender->ForcedMaterialOverride(visibleMaterial ? visibleMaterial : nullptr);
	ModelRender_DrawModelExecute->Original<void(__thiscall*)(CModelRender*, const DrawModelState_t&, const ModelRenderInfo_t&, matrix3x4*)>()(I::ModelRender, pState, pInfo, pBoneToWorld);
	
	if (bTwoModels)
	{
		SetupOccluded(pRenderContext);
		F::Materials.SetColor(occludedMaterial, chams.OccludedColor);
		I::ModelRender->ForcedMaterialOverride(occludedMaterial ? occludedMaterial : nullptr);
		ModelRender_DrawModelExecute->Original<void(__thiscall*)(CModelRender*, const DrawModelState_t&, const ModelRenderInfo_t&, matrix3x4*)>()(I::ModelRender, pState, pInfo, pBoneToWorld);
	}

	SetupEnd(pRenderContext, bTwoModels);
	I::RenderView->SetColorModulation(1.f, 1.f, 1.f);
	I::RenderView->SetBlend(1.f);
	I::ModelRender->ForcedMaterialOverride(nullptr);
}



void CChams::RenderFakeAng(const DrawModelState_t& pState, const ModelRenderInfo_t& pInfo, matrix3x4* pBoneToWorld)
{
	if (!Vars::Chams::FakeAngle::Active.Value || !F::FakeAng.DrawChams)
		return;

	DrawModel(Vars::Chams::FakeAngle::Chams.Value, pState, pInfo, reinterpret_cast<matrix3x4*>(&F::FakeAng.BoneMatrix), false);
}

void CChams::RenderBacktrack(const DrawModelState_t& pState, const ModelRenderInfo_t& pInfo, matrix3x4* pBoneToWorld)
{
	if (!Vars::Backtrack::Enabled.Value || !Vars::Chams::Backtrack::Active.Value)
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

	switch (Vars::Chams::Backtrack::Draw.Value)
	{
	case 0: // last
	{
		std::optional<TickRecord> vLastRec = F::Backtrack.GetLastRecord(pEntity);
		if (vLastRec && pEntity->GetAbsOrigin().DistTo(vLastRec->vOrigin) >= 0.1f)
			DrawModel(Vars::Chams::Backtrack::Chams.Value, pState, pInfo, reinterpret_cast<matrix3x4*>(&vLastRec->BoneMatrix), false);
		break;
	}
	case 1: // last + first
	{
		std::optional<TickRecord> vFirstRec = F::Backtrack.GetFirstRecord(pEntity);
		if (vFirstRec && pEntity->GetAbsOrigin().DistTo(vFirstRec->vOrigin) >= 0.1f)
			DrawModel(Vars::Chams::Backtrack::Chams.Value, pState, pInfo, reinterpret_cast<matrix3x4*>(&vFirstRec->BoneMatrix), false);
		std::optional<TickRecord> vLastRec = F::Backtrack.GetLastRecord(pEntity);
		if (vLastRec && pEntity->GetAbsOrigin().DistTo(vLastRec->vOrigin) >= 0.1f)
			DrawModel(Vars::Chams::Backtrack::Chams.Value, pState, pInfo, reinterpret_cast<matrix3x4*>(&vLastRec->BoneMatrix), false);
		break;
	}
	case 2: // all
	{
		const auto& vRecords = F::Backtrack.GetRecords(pEntity);
		if (vRecords && !vRecords->empty())
		{
			for (auto& record : *vRecords)
			{
				if (!F::Backtrack.WithinRewind(record))
					continue;
				if (pEntity->GetAbsOrigin().DistTo(record.vOrigin) <= 0.1f)
					continue;

				DrawModel(Vars::Chams::Backtrack::Chams.Value, pState, pInfo, reinterpret_cast<matrix3x4*>(&record.BoneMatrix), false);
			}
		}
	}
	}
}

bool CChams::Render(const DrawModelState_t& pState, const ModelRenderInfo_t& pInfo, matrix3x4* pBoneToWorld)
{
	m_bRendering = false;
	if (I::EngineVGui->IsGameUIVisible())
		return false;

	const auto ModelRender_DrawModelExecute = g_HookManager.GetMapHooks()["ModelRender_DrawModelExecute"];
	const auto& pRenderContext = I::MaterialSystem->GetRenderContext();
	if (!ModelRender_DrawModelExecute || !pRenderContext)
		return false;

	RenderBacktrack(pState, pInfo, pBoneToWorld);
	const int iIndex = pInfo.m_nEntIndex;
	if (iIndex == I::EngineClient->GetLocalPlayer())
		RenderFakeAng(pState, pInfo, pBoneToWorld);

	Chams_t chams = {}; bool bViewmodel = false;
	if (!GetChams(iIndex, I::ModelInfoClient->GetModelName(pInfo.m_pModel), &chams, &bViewmodel))
		return false;



	m_bRendering = true;

	DrawModel(chams, pState, pInfo, pBoneToWorld, !bViewmodel);

	m_bRendering = false;

	return true;
}