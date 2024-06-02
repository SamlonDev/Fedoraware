#ifndef CAMERAWINDOW_H
#define CAMERAWINDOW_H

#include "../../../Interfaces.h"

class CCameraWindow
{
public:
    void Init();
    void Draw();
    void RenderView(void* ecx, const CViewSetup& pViewSetup);

    virtual ~CCameraWindow();

private:
    void RenderCustomView(void* ecx, const CViewSetup& pViewSetup, ITexture* pTexture);

    ITexture* CameraTex;
    IMaterial* CameraMat;
    Vector CameraOrigin;
    QAngle CameraAngles;
};

#endif // CAMERAWINDOW_H


#include "CameraWindow.h"

#include "Hooks/HookManager.h"
#include "Hooks/Hooks.h"
#include "materialsystem/imaterialsystem.h"

#include <memory>

using namespace Hooks;
using namespace MaterialSystem;

void CCameraWindow::Init()
{
    // Create camera texture
    CameraTex = I::MaterialSystem->CreateNamedRenderTargetTextureEx("mirrorcam_rt", 1, 1, RT_SIZE_FULL_FRAME_BUFFER, IMAGE_FORMAT_RGB888, MATERIAL_RT_DEPTH_SHARED, TEXTUREFLAGS_CLAMPS | TEXTUREFLAGS_CLAMPT, CREATERENDERTARGETFLAGS_HDR);

    // Create camera material
    static std::unique_ptr<KeyValues> kv = std::make_unique<KeyValues>("UnlitGeneric");
    kv->SetString("$basetexture", "mirrorcam_rt");
    CameraMat = I::MaterialSystem->CreateMaterial("m_cameraMat", kv.get());
}

void CCameraWindow::Draw()
{
    if (!CameraMat || !I::EngineClient->IsInGame() || !ShouldDraw)
        return;

    const WindowBox_t& info = Vars::Visuals::Simulation::ProjectileWindow.Value;

    if (!info.valid)
        return;

    // Draw to screen
    CMatRenderContextPtr pRenderContext(I::MaterialSystem->GetRenderContext());
    if (!pRenderContext)
        return;

    pRenderContext->DrawScreenSpaceRectangle(
        CameraMat,
        info.x, info.y, info.w, info.h,
        0, 0, info.w, info.h,
        CameraTex->GetActualWidth(), CameraTex->GetActualHeight(),
        nullptr, 1, 1
    );
}

void CCameraWindow::RenderView(void* ecx, const CViewSetup& pViewSetup)
{
    if (!CameraTex || !ShouldDraw)
        return;

    const WindowBox_t& info = Vars::Visuals::Simulation::ProjectileWindow.Value;

    if (!info.valid)
        return;

    CViewSetup viewSetup = pViewSetup;
    viewSetup.x = 0;
    viewSetup.y = 0;

    viewSetup.origin = CameraOrigin;
    viewSetup.angles = CameraAngles;

    viewSetup.width = info.w + 1;
    viewSetup.height = info.h + 1;
    viewSetup.m_flAspectRatio = static_cast<float>(viewSetup.width) / static_cast<float>(viewSetup.height);
    viewSetup.fov = 90;

    if (!I::MaterialSystem->IsRenderContextValid())
        return;

    RenderCustomView(ecx, viewSetup, CameraTex);
}

void CCameraWindow::RenderCustomView(void* ecx, const CViewSetup& pViewSetup, ITexture* pTexture)
{
    if (!pTexture || !I::MaterialSystem->IsRenderContextValid())
        return;

    CMatRenderContextPtr pRenderContext(I::MaterialSystem->GetRenderContext());
    if (!pRenderContext)
        return;

    pRenderContext->PushRenderTargetAndViewport();
    pRenderContext->SetRenderTarget(pTexture);

    if (g_HookManager.GetMapHooks().find("ViewRender_RenderView") != g_HookManager.GetMapHooks().end())
    {
        auto renderViewHook = g_HookManager.GetMapHooks()["ViewRender_RenderView"];
        if (renderViewHook)
            renderViewHook->Original<void(__thiscall*)(void*, const CViewSetup&, int, int)>()(ecx, pViewSetup, VIEW_CLEAR_COLOR | VIEW_CLEAR_DEPTH, RENDERVIEW_UNSPECIFIED);
    }

    pRenderContext->PopRenderTargetAndViewport();
    pRenderContext->Release();
}

CCameraWindow::~CCameraWindow()
{
    if (CameraTex)
    {
        I::MaterialSystem->DestroyRenderTarget(CameraTex, TEXTURE_GROUP_RENDER_TARGET);
        CameraTex = nullptr;
    }

    if (CameraMat)
    {
        I::MaterialSystem->DestroyMaterial(CameraMat);
        CameraMat = nullptr;
    }
}
