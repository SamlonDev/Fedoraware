#pragma once

#if !defined(PACKET_MANIP_H)
#define PACKET_MANIP_H

#include "../Feature.h"
#include "FakeLag/FakeLag.h"
#include "AntiAim/AntiAim.h"

class CPacketManip : public Feature
{
    bool WillTimeOut();
    bool AntiAimCheck();

public:
    void Run(CUserCmd* pCmd, bool* pSendPacket) override;
};

ADD_FEATURE(CPacketManip, PacketManip)

#endif //PACKET_MANIP_H


#pragma once

#if !defined(FEATURE_H)
#define FEATURE_H

#include <unordered_map>
#include <string>

class IFeature
{
public:
    virtual void Run(CUserCmd* pCmd, bool* pSendPacket) = 0;
};

template <typename T>
class CFeature : public IFeature
{
public:
    void Run(CUserCmd* pCmd, bool* pSendPacket) override
    {
        static_cast<T*>(this)->Run(pCmd, pSendPacket);
    }
};

#define ADD_FEATURE(ClassName, FeatureName) std::unordered_map<std::string, IFeature*>& GetFeatures() { \
    static std::unordered_map<std::string, IFeature*> Features; \
    return Features; \
} \
\
void RegisterFeature() \
{ \
    GetFeatures()[#FeatureName] = new ClassName(); \
} \
\
RegisterFeature();

#endif //FEATURE_H
