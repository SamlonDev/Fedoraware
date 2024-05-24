#pragma once

// Steamworks SDK
#pragma warning(push, 0)
#include <Steam/steam_api.h>
#pragma warning(pop)

#include "Classes.h"
#include "Const.h"
#include "Enums.h"
#include "Structs.h"
#include "bitbuf.h"
#include "MD5.h"
#include "worldsize.h"

#include <deque>

typedef void* (*CreateInterfaceFn)(const char* pName, int* pReturnCode);

using Vector = Vec3;
using QAngle = Vec3;
class IRefCounted
{
public:
	virtual int AddRef() = 0;
	virtual int Release() = 0;
};

FORCEINLINE void VectorCopy(const Vector& src, Vector& dst)
{
	dst.x = src.x;
	dst.y = src.y;
	dst.z = src.z;
}