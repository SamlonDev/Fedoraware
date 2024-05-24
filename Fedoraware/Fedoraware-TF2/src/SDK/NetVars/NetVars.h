#pragma once
#include "../Includes/Includes.h"

class CNetVars
{
public:
	int GetOffset(RecvTable* pTable, const char* szNetVar);
	int GetNetVar(const char* szClass, const char* szNetVar);
};

inline CNetVars g_NetVars;

#define NETVAR(_name, type, table, name) inline type &_name() \
{ \
	static int nOffset = g_NetVars.GetNetVar(table, name); \
	return *reinterpret_cast<type *>(reinterpret_cast<DWORD>(this) + nOffset); \
}

#define NETVAR_OFF(_name, type, table, name, offset) inline type &_name() \
{ \
	static int nOffset = g_NetVars.GetNetVar(table, name) + offset; \
	return *reinterpret_cast<type *>(reinterpret_cast<DWORD>(this) + nOffset); \
}

#define OFFSET(_name, type, offset) inline type &_name() \
{ \
	return *reinterpret_cast<type *>(reinterpret_cast<DWORD>(this) + offset); \
}

#define VIRTUAL(name, type, base, fn, index) __inline type Get##name() \
{ \
	void* pBase = base; \
	return GetVFunc<fn>(pBase, index)(pBase); \
}

#define CONDGET(name, conditions, cond) __inline bool Is##name() \
{ \
	return (conditions & cond); \
}