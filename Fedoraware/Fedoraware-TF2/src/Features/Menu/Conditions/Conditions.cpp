#include "Conditions.h"

#include <functional>
#include <string>
#include <vector>
#include <map>

// Replaced preprocessor macros with inline functions
template<typename T>
bool IsType(const Variable<T>* var)
{
    return var->m_iType == typeid(T).hash_code();
}

template<typename T, typename F>
void SetType(Variable<T>* var, F cond)
{
    if (var->Map.contains(cond))
    {
        var->Value = var->Map[cond];
    }
}

template<typename T, typename F>
void SetT(Variable<T>* var, F cond)
{
    if (IsType(var))
    {
        SetType(var, cond);
    }
}

template<typename T, typename F>
void RemoveType(Variable<T>* var, F cond)
{
    if (var->Map.contains(cond))
    {
        auto it = var->Map.find(cond);
        var->Map.erase(it);
    }
}

template<typename T, typename F>
void RemoveT(Variable<T>* var, F cond)
{
    if (IsType(var))
    {
        RemoveType(var, cond);
    }
}

void CConditions::Run()
{
    if (G::UnloadWndProcHook)
    {
        return;
    }

    auto setVars = [&](const std::string& sCond)
    {
        for (const auto& var : g_Vars)
        {
            if ((var->m_iFlags & (NOSAVE | NOCOND)) && sCond != "default")
            {
                continue;
            }

            SetT<bool>(var, sCond);
            SetT<int>(var, sCond);
            SetT<float>(var, sCond);
            SetT<IntRange_t>(var, sCond);
            SetT<FloatRange_t>(var, sCond);
            SetT<std::string>(var, sCond);
            SetT<std::vector<std::string>>(var, sCond);
            SetT<Color_t>(var, sCond);
            SetT<Gradient_t>(var, sCond);
            SetT<Vec3>(var, sCond);
            SetT<DragBox_t>(var, sCond);
            SetT<WindowBox_t>(var, sCond);
        }
    };

    setVars("default");

    std::function<void(const std::string&)> getConds = [&](const std::string& sParent)
    {
        for (const auto& sCond : vConditions)
        {
            const auto& tCond = mConditions.at(sCond);
            if (tCond.Parent != sParent)
            {
                continue;
            }

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
                {
                    bKey = !bKey;
                }

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
                {
                    tCond.Active = !tCond.Active;
                }
                break;
            }
            // weapon type
            case 2:
            {
                const auto& pWeapon = g_EntityCache.GetWeapon();
                tCond.Active = tCond.Info + 1 == int(Utils::GetWeaponType(pWeapon));
                if (tCond.Not)
                {
                    tCond.Active = !tCond.Active;
                }
                break;
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

bool CConditions::Exists(const std::string& sCondition)
{
    return std::find(vConditions.begin(), vConditions.end(), sCondition) != vConditions.end();
}

bool CConditions::HasChildren(const std::string& sCondition)
{
    for (const auto& sCond : vConditions)
    {
        if (mConditions[sCond].Parent == sCondition)
        {
            return true;
        }
    }
    return false;
}

std::string CConditions::GetParent(const std::string& sCondition)
{
    for (const auto& sCond : vConditions)
    {
        if (sCond == sCondition && mConditions[sCond].Parent != "")
        {
            return mConditions[sCond].Parent;
        }
    }
    return "default";
}

void CConditions::AddCondition(const std::string& sCondition, const Condition_t& tCond)
{
    mConditions[sCondition] = tCond;
    if (!Exists(sCondition))
    {
        vConditions.push_back(sCondition);
    }
}

void CConditions::RemoveCondition(const std::string& sCondition)
{
    for (const auto var : g_Vars)
    {
        RemoveT<bool>(var, sCondition);
        RemoveT<int>(var, sCondition);
        RemoveT<float>(var, sCondition);
        RemoveT<IntRange_t>(var, sCondition);
        RemoveT<FloatRange_t>(var, sCondition);
        RemoveT<std::string>(var, sCondition);
        RemoveT<std::vector<std::string>>
