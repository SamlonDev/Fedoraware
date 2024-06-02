#pragma once

#include <Windows.h>
#include "../../Includes/Includes.h"

#ifdef ENGINETRACE_H
#	error "engineTrace.h included more than once!"
#endif

#define ENGINETRACE_H

// Forward declarations
class CTraceListData;
class IHandleEntity;
class CPhysCollide;
class CGameTrace;

// Interface for enumerating entities
class IEntityEnumerator
{
public:
	virtual bool EnumEntity(IHandleEntity* pHandleEntity) = 0;
};

// Engine tracing interface
class IEngineTrace
{
public:
	virtual int GetPointContents(const Vector& vecAbsPosition, IHandleEntity** ppEntity = NULL) const = 0;
	virtual int GetPointContents_Collideable(ICollideable* pCollide, const Vector& vecAbsPosition) const = 0;
	virtual void ClipRayToEntity(const Ray_t& ray, unsigned int fMask, IHandleEntity* pEnt, CGameTrace* pTrace) const = 0;
	virtual void ClipRayToCollideable(const Ray_t& ray, unsigned int fMask, ICollideable* pCollide, CGameTrace* pTrace) const = 0;
	virtual void TraceRay(const Ray_t& ray, unsigned int fMask, CTraceFilter* pTraceFilter, CGameTrace* pTrace) const = 0;
};

// Version macro for the interface
#define VENGINE_TRACE_CLIENT_INTERFACE_VERSION "EngineTraceClient003"
