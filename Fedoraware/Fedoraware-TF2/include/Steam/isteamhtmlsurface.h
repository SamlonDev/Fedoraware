//=== Copyright 1996-2013, Valve Corporation, All rights reserved. =======
//
// Purpose: An interface for displaying HTML pages in a texture.
//
//=============================================================================

#ifndef ISTEAMHTMLSURFACE_H
#define ISTEAMHTMLSURFACE_H

#ifdef _WIN32
#pragma once
#endif

#include "steam_api_common.h"

//-----------------------------------------------------------------------------
// Purpose: Functions for displaying HTML pages and interacting with them.
//-----------------------------------------------------------------------------
class ISteamHTMLSurface
{
public:
    virtual ~ISteamHTMLSurface() noexcept = default;

    // Must call init and shutdown when starting/ending use of the interface.
    virtual bool Init() noexcept = 0;
    virtual bool Shutdown() noexcept = 
