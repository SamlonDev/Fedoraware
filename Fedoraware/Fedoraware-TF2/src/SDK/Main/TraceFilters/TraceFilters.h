#pragma once
#include "../BaseEntity/BaseEntity.h"

class CTraceFilter : public CTraceFilterBase
{
public:
    virtual ETraceType GetTraceType() const = 0;
};

class CTraceFilterBase
{
public:
    CBaseEntity* pSkip = nullptr;

    virtual bool ShouldHitEntity(void* pEntityHandle, int nContentsMask)
    {
        if (pSkip && pSkip == CBaseEntity::GetPointer((IHandleEntity*)pEntityHandle))
            return false;

        return true;
    }
};

class CTraceFilterHitscan : public CTraceFilter
{
public:
    virtual ETraceType GetTraceType() const
    {
        return TRACE_EVERYTHING;
    }
};

class CTraceFilterProjectile : public CTraceFilter
{
public:
    virtual ETraceType GetTraceType() const
    {
        return TRACE_WORLD_ONLY;
    }
};

class CTraceFilterWorldAndPropsOnly : public CTraceFilter
{
public:
    virtual ETraceType GetTraceType() const
    {
        return TRACE_WORLD_AND_STATICS;
    }
};
