#ifndef HOOKS_H
#define HOOKS_H

#include <hooks.h>  // Include the necessary headers
#include <KeyValues.h>
#include <sdktools/sdktools.h>

struct CEconItemDefinition_t
{
    std::uint32_t dword0;
    KeyValues* m_pKVItem;
    std::uint16_t m_nDefIndex;
    std::uint32_t dwordC;
    std::byte byte10;
    std::byte m_unMinItemLevel;
    std::byte m_unMaxItemLevel;
    std::byte gap13[3];
    std::uint16_t m_nDefaultDropQuantity;
    std::byte gap18[4];
    std::int32_t int1C;
    std::uint32_t dword20;
    std::byte gap24[4];
    std::uint32_t dword28;
    std::uint32_t dword2C;
    std::byte m_nPopularitySeed;
    const char* m_pszItemBaseName;
    bool m_bProperName;
    const char* m_pszItemTypeName;
    const char* m_pszItemDesc;
    std::uint32_t m_rtExpiration;
    const char* m_pszInventoryModel;
    const char* m_pszInventoryImage;
    std::byte m_pszInventoryOverlayImages[20];
    std::int32_t m_iInventoryImagePosition_0;
    std::int32_t m_iInventoryImagePosition_1;
    std::int32_t m_iInventoryImageSize_0;
    std::int32_t m_iInventoryImageSize_1;
    std::int32_t m_iInspectPanelDistance;
    const char* m_pszBaseDisplayModel;
    std::int32_t m_iDefaultSkin;
    bool m_bLoadOnDemand;
    bool m_bHideBodyGroupsDeployedOnly;
    const char* m_pszWorldDisplayModel;
    const char* m_pszWorldExtraWearableModel;
    const char* m_pszWorldExtraWearableViewModel;
    const char* m_pszVisionFilteredDisplayModel;
    const char* m_pszCollectionReference;
    bool m_bAttachToHands;
    bool m_bAttachToHandsVMOnly;
    bool m_bFlipViewModel;
    bool m_bActAsWearable;
    bool m_bActAsWeapon;
    bool m_bIsTool;
    std::byte gap9E[30];
    const char* m_pszBrassModelOverride;
    void* m_pTool;
    std::uint32_t dwordC4;
    std::int32_t m_iCapabilities;
    std::byte gapCC[8];
    const char* m_pszItemClassname;
    const char* m_pszItemLogClassname;
    const char* m_pszItemIconClassname;
    const char* m_pszDefinitionName;
    const char* m_pszDatabaseAuditTable;
    bool m_bHidden;
    bool m_bShouldShowInArmory;
    bool m_bBaseItem;
    bool m_bImported;
    bool m_bIsPackBundle;
    std::byte gapED[11];
    const char* m_pszArmoryDesc;
    const char* m_pszXifierRemapClass;
    const char* m_pszBaseFunctionalItemName;
    const char* m_pszParticleSuffix;
    std::byte gap108[8];
    const char* m_pszArmoryRemap;
    const char* m_pszStoreRemap;
    const char* m_pszClassToken;
    const char* m_pszSlotToken;
    std::int32_t m_iDropType;
    const char* m_pszHolidayRestriction;
    std::uint32_t m_nVisionFilterFlags;
    std::uint32_t m_iSubType;
    std::byte gap130[4];
    std::uint32_t m_unEquipRegionMask;
    std::uint32_t m_unEquipRegionConflictMask;
    std::uint16_t word13C;
    std::byte gap13E[22];
    bool m_bValidForShuffle;
    bool m_bValidForSelfMade;
    std::int32_t int158;
    std::uint32_t dword15C;
    std::byte gap160[4];
    char* pchar164;
    std::uint32_t dword168;
};

MAKE_HOOK(CEconItemSchema_GetItemDefinition,
    [](std::uint32_t* ecx, void* edx, int iItemIndex) -> CEconItemDefinition_t*
    {
        auto pItemDefinition = Hook.Original<FN>()(ecx, edx, iItemIndex);
        if (pItemDefinition && Vars::Misc::Exploits::EquipRegionUnlock.Value)
        {
            pItemDefinition->m_unEquipRegionMask = 0;
            pItemDefinition->m_unEquipRegionConflictMask = 0;
        }

        return pItemDefinition;
    }
, CEconItemSchema_GetItemDefinition, noexcept);

#endif // HOOKS_H
