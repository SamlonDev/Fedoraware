#ifndef MYHEADER_H
#define MYHEADER_H

// Steamworks SDK
#pragma warning(push, 0)
#include <Steam/steam_api.h>
#pragma warning(pop)

#include "Vec3.h"
#include "Classes.h"
#include "Const.h"
#include "Enums.h"
#include "Structs.h"
#include "bitbuf.h"
#include "MD5.h"
#include "worldsize.h"

#include <deque>

namespace SteamworksSDK {

typedef void* (*CreateInterfaceFn)(const char* pName, int* pReturnCode);

// Vector is an alias for Vec3
using Vector = Vec3;

// QAngle is an alias for Vec3
using QAngle = Vec3;

class IRefCounted
{
public:
    virtual ~IRefCounted() {} // Added virtual destructor

    virtual int AddRef() = 0;
    virtual int Release() = 0;
};

// VectorCopy function
FORCEINLINE2 void VectorCopy(const Vector& src, Vector& dst)
{
    dst.x = src.x;
    dst.y = src.y;
    dst.z = src.z;
}

} // namespace SteamworksSDK

#endif // MYHEADER_H
