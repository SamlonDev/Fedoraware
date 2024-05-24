#include "NetVars.h"
#include "../Interfaces/Interfaces.h"

int CNetVars::GetOffset(RecvTable* pTable, const char* szNetVar)
{
	for (int i = 0; i < pTable->m_nProps; i++)
	{
		RecvProp Prop = pTable->m_pProps[i];

		if (std::string_view(Prop.m_pVarName).compare(szNetVar) == 0)
			return Prop.GetOffset();

		if (auto DataTable = Prop.GetDataTable())
		{
			if (auto nOffset = GetOffset(DataTable, szNetVar))
				return nOffset + Prop.GetOffset();
		}
	}

	return 0;
}

int CNetVars::GetNetVar(const char* szClass, const char* szNetVar)
{
	CClientClass* pClasses = I::BaseClientDLL->GetAllClasses();

	for (auto pCurrNode = pClasses; pCurrNode; pCurrNode = pCurrNode->m_pNext)
	{
		if (std::string_view(szClass).compare(pCurrNode->m_pNetworkName) == 0)
			return GetOffset(pCurrNode->m_pRecvTable, szNetVar);
	}

	return 0;
}