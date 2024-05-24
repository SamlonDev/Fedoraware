#include "Conditions.h"

#include <functional>

#define IsType(type) var->m_iType == typeid(type).hash_code()
#define SetType(type, cond)\
{\
	if (var->GetVar<type>()->Map.contains(cond))\
		var->GetVar<type>()->Value = var->GetVar<type>()->Map[cond];\
}
#define SetT(type, cond) if (IsType(type)) SetType(type, cond)

void CConditions::Run()
{
	if (G::UnloadWndProcHook)
		return;

	auto setVars = [](std::string sCond)
		{
			for (const auto var : g_Vars)
			{
				if (var->m_iFlags & (NOSAVE | NOCOND) && sCond != "default")
					continue;

				SetT(bool, sCond)
				else SetT(int, sCond)
				else SetT(float, sCond)
				else SetT(IntRange_t, sCond)
				else SetT(FloatRange_t, sCond)
				else SetT(std::string, sCond)
				else SetT(std::vector<std::string>, sCond)
				else SetT(Color_t, sCond)
				else SetT(Gradient_t, sCond)
				else SetT(Vec3, sCond)
				else SetT(DragBox_t, sCond)
				else SetT(WindowBox_t, sCond)
			}
		};

	setVars("default");

	std::function<void(std::string)> getConds = [&](std::string sParent)
		{
			for (auto& sCond : vConditions)
			{
				auto& tCond = mConditions[sCond];
				if (tCond.Parent != sParent)
					continue;
				
				switch (tCond.Type)
				{
				// key
				case 0:
				{
					bool bKey = false;
					switch (tCond.Info)
					{
					case 0: bKey = F::KeyHandler.Down(tCond.Key, true, &tCond.Storage); break;
					case 1: bKey = F::KeyHandler.Pressed(tCond.Key, true, &tCond.Storage); break;
					case 2: bKey = F::KeyHandler.Double(tCond.Key, true, &tCond.Storage); break;
					}
					const bool bUIOpen = I::EngineVGui->IsGameUIVisible() || I::MatSystemSurface->IsCursorVisible();
					bKey = !bUIOpen && bKey;
					if (tCond.Not)
						bKey = !bKey;

					switch (tCond.Info)
					{
					case 0: tCond.Active = bKey; break;
					case 1:
					case 2: if (bKey) tCond.Active = !tCond.Active;
					}
					break;
				}
				// class
				case 1:
				{
					const auto& pLocal = g_EntityCache.GetLocal();
					const int iClass = pLocal ? pLocal->m_iClass() : 0;
					switch (tCond.Info)
					{
					case 0: { tCond.Active = iClass == 1; break; }
					case 1: { tCond.Active = iClass == 3; break; }
					case 2: { tCond.Active = iClass == 7; break; }
					case 3: { tCond.Active = iClass == 4; break; }
					case 4: { tCond.Active = iClass == 6; break; }
					case 5: { tCond.Active = iClass == 9; break; }
					case 6: { tCond.Active = iClass == 5; break; }
					case 7: { tCond.Active = iClass == 2; break; }
					case 8: { tCond.Active = iClass == 8; break; }
					}
					if (tCond.Not)
						tCond.Active = !tCond.Active;
					break;
				}
				// weapon type
				case 2:
				{
					const auto& pWeapon = g_EntityCache.GetWeapon();
					tCond.Active = tCond.Info + 1 == int(Utils::GetWeaponType(pWeapon));
					if (tCond.Not)
						tCond.Active = !tCond.Active;
				}
				}

				if (tCond.Active)
				{
					setVars(sCond);
					getConds(sCond);
				}
			}
		};
	getConds("");
}

bool CConditions::Exists(std::string sCondition)
{
	for (const auto& sCond : vConditions)
	{
		if (sCond == sCondition)
			return true;
	}
	return false;
}

bool CConditions::HasChildren(std::string sCondition)
{
	for (auto& sCond : vConditions)
	{
		if (mConditions[sCond].Parent == sCondition)
			return true;
	}
	return false;
}

std::string CConditions::GetParent(std::string sCondition)
{
	for (auto& sCond : vConditions)
	{
		if (sCond == sCondition && mConditions[sCond].Parent != "")
			return mConditions[sCond].Parent;
	}
	return "default";
}

void CConditions::AddCondition(std::string sCondition, Condition_t tCond)
{
	mConditions[sCondition] = tCond;
	if (!Exists(sCondition))
		vConditions.push_back(sCondition);
}

// fun!
#define RemoveType(type, cond)\
{\
	if (var->GetVar<type>()->Map.contains(cond))\
	{\
		for (auto it = var->GetVar<type>()->Map.begin(); it != var->GetVar<type>()->Map.end();)\
		{\
			if (it->first == cond)\
				it = var->GetVar<type>()->Map.erase(it);\
			else\
				++it;\
		}\
	}\
}
#define RemoveT(type, cond) if (IsType(type)) RemoveType(type, cond)

void CConditions::RemoveCondition(std::string sCondition)
{
	for (const auto var : g_Vars)
	{
		RemoveT(bool, sCondition)
		else RemoveT(int, sCondition)
		else RemoveT(float, sCondition)
		else RemoveT(IntRange_t, sCondition)
		else RemoveT(FloatRange_t, sCondition)
		else RemoveT(std::string, sCondition)
		else RemoveT(std::vector<std::string>, sCondition)
		else RemoveT(Color_t, sCondition)
		else RemoveT(Gradient_t, sCondition)
		else RemoveT(Vec3, sCondition)
		else RemoveT(DragBox_t, sCondition)
		else RemoveT(WindowBox_t, sCondition)
	}

	auto removeCond = [&](std::string cond)
		{
			for (auto it = vConditions.begin(); it != vConditions.end();)
			{
				if (*it == cond)
					it = vConditions.erase(it);
				else
					++it;
			}
			for (auto it = mConditions.begin(); it != mConditions.end();)
			{
				if (it->first == cond)
					it = mConditions.erase(it);
				else
					++it;
			}
		};
	removeCond(sCondition);

	std::function<void(std::string)> removeChildren = [&](std::string parent)
		{
			for (auto& sCond : vConditions)
			{
				auto& cCond = mConditions[sCond];
				if (cCond.Parent != parent)
					continue;

				removeCond(sCond);
				removeChildren(sCond);
			}
		};
	removeChildren(sCondition);
}