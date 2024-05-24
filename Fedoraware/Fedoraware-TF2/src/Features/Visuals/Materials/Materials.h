#pragma once
#include "../../Feature.h"

#define MaterialFolder (F::ConfigManager.GetConfigPath() + "\\Materials")

struct Material_t
{
	IMaterial* pMaterial;

	std::string sVMT = ""; // will be shown to user through material editor, for glow this won't matter
	bool bLocked = false;

};

class CMaterials
{
public:
	IMaterial* CreateNRef(char const* szName, KeyValues* pKV);

	void ReloadMaterials();
	void RemoveMaterials();

	IMaterial* GetMaterial(std::string sName);
	std::string GetVMT(std::string sName);

	void AddMaterial(std::string sName);
	void EditMaterial(std::string sName, std::string sVMT);
	void RemoveMaterial(std::string sName);

	void SetColor(IMaterial* material, Color_t color, bool bSetColor = true);

	std::unordered_map<std::string, Material_t> mChamMaterials;
	std::unordered_map<std::string, Material_t> mGlowMaterials;
	std::vector<IMaterial*> vMatList;
};

ADD_FEATURE(CMaterials, Materials)