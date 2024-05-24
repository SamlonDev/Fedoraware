#pragma once
#include "../../Includes/Includes.h"
#include "../IAppSystem/IAppSystem.h"

#undef CreateFont
#undef PlaySound

typedef unsigned int VPANEL;
typedef unsigned long HScheme;
typedef unsigned long HTexture;
typedef unsigned long HCursor;
typedef unsigned long HPanel;
typedef unsigned long HFont;
class IHTML;
class IHTMLEvents;
class IVguiMatInfo;

enum FontDrawType_t
{
	FONT_DRAW_DEFAULT = 0,
	FONT_DRAW_NONADDITIVE,
	FONT_DRAW_ADDITIVE,
	FONT_DRAW_TYPE_COUNT = 2,
};

struct CharRenderInfo
{
	int x = 0, y = 0;
	Vertex_t* verts = nullptr;
	int textureId = 0;
	int abcA = 0;
	int abcB = 0;
	int abcC = 0;
	int fontTall = 0;
	HFont currentFont = {};
	FontDrawType_t drawType = {};
	wchar_t ch = {};
	bool valid = false;
	bool shouldclip = false;
};

struct IntRect
{
	int x0 = 0;
	int y0 = 0;
	int x1 = 0;
	int y1 = 0;
};

enum ETextureFormat
{
	eTextureFormat_RGBA,
	eTextureFormat_BGRA,
	eTextureFormat_BGRA_Opaque
};

enum SurfaceFeature_e
{
	ANTIALIASED_FONTS = 1,
	DROPSHADOW_FONTS = 2,
	ESCAPE_KEY = 3,
	OPENING_NEW_HTML_WINDOWS = 4,
	FRAME_MINIMIZE_MAXIMIZE = 5,
	OUTLINE_FONTS = 6,
	DIRECT_HWND_RENDER = 7,
};

class ISurface : public IAppSystem
{
public:
	virtual void Shutdown() = 0;
	virtual void RunFrame() = 0;
	virtual VPANEL GetEmbeddedPanel() = 0;
	virtual void SetEmbeddedPanel(VPANEL pPanel) = 0;
	virtual void PushMakeCurrent(VPANEL panel, bool useInsets) = 0;
	virtual void PopMakeCurrent(VPANEL panel) = 0;
	virtual void DrawSetColor(int r, int g, int b, int a) = 0;
	virtual void DrawSetColor(Color_t col) = 0;
	virtual void DrawFilledRect(int x0, int y0, int x1, int y1) = 0;
	virtual void DrawFilledRectArray(IntRect* pRects, int numRects) = 0;
	virtual void DrawOutlinedRect(int x0, int y0, int x1, int y1) = 0;
	virtual void DrawLine(int x0, int y0, int x1, int y1) = 0;
	virtual void DrawPolyLine(int* px, int* py, int numPoints) = 0;
	virtual void DrawSetTextFont(HFont font) = 0;
	virtual void DrawSetTextColor(int r, int g, int b, int a) = 0;
	virtual void DrawSetTextColor(Color_t col) = 0;
	virtual void DrawSetTextPos(int x, int y) = 0;
	virtual void DrawGetTextPos(int& x, int& y) = 0;
	virtual void DrawPrintText(const wchar_t* text, int textLen, FontDrawType_t drawType = FONT_DRAW_DEFAULT) = 0;
	virtual void DrawUnicodeChar(wchar_t wch, FontDrawType_t drawType = FONT_DRAW_DEFAULT) = 0;
	virtual void DrawFlushText() = 0;
	virtual IHTML* CreateHTMLWindow(IHTMLEvents* events, VPANEL context) = 0;
	virtual void PaintHTMLWindow(IHTML* htmlwin) = 0;
	virtual void DeleteHTMLWindow(IHTML* htmlwin) = 0;
	virtual int	DrawGetTextureId(char const* filename) = 0;
	virtual bool DrawGetTextureFile(int id, char* filename, int maxlen) = 0;
	virtual void DrawSetTextureFile(int id, const char* filename, int hardwareFilter, bool forceReload) = 0;
	virtual void DrawSetTextureRGBA(int id, const unsigned char* rgba, int wide, int tall, int hardwareFilter, bool forceReload) = 0;
	virtual void DrawSetTexture(int id) = 0;
	virtual void DrawGetTextureSize(int id, int& wide, int& tall) = 0;
	virtual void DrawTexturedRect(int x0, int y0, int x1, int y1) = 0;
	virtual bool IsTextureIDValid(int id) = 0;
	virtual bool DeleteTextureByID(int id) = 0;
	virtual int CreateNewTextureID(bool procedural = false) = 0;
	virtual void GetScreenSize(int& wide, int& tall) = 0;
	virtual void SetAsTopMost(VPANEL panel, bool state) = 0;
	virtual void BringToFront(VPANEL panel) = 0;
	virtual void SetForegroundWindow(VPANEL panel) = 0;
	virtual void SetPanelVisible(VPANEL panel, bool state) = 0;
	virtual void SetMinimized(VPANEL panel, bool state) = 0;
	virtual bool IsMinimized(VPANEL panel) = 0;
	virtual void FlashWindow(VPANEL panel, bool state) = 0;
	virtual void SetTitle(VPANEL panel, const wchar_t* title) = 0;
	virtual void SetAsToolBar(VPANEL panel, bool state) = 0;
	virtual void CreatePopup(VPANEL panel, bool minimised, bool showTaskbarIcon = true, bool disabled = false, bool mouseInput = true, bool kbInput = true) = 0;
	virtual void SwapBuffers(VPANEL panel) = 0;
	virtual void Invalidate(VPANEL panel) = 0;
	virtual void SetCursor(HCursor cursor) = 0;
	virtual void SetCursorAlwaysVisible(bool visible) = 0;
	virtual bool IsCursorVisible() = 0;
	virtual void ApplyChanges() = 0;
	virtual bool IsWithin(int x, int y) = 0;
	virtual bool HasFocus() = 0;
	virtual bool SupportsFeature(SurfaceFeature_e feature) = 0;
	virtual void RestrictPaintToSinglePanel(VPANEL panel) = 0;
	virtual void SetModalPanel(VPANEL) = 0;
	virtual VPANEL GetModalPanel() = 0;
	virtual void UnlockCursor() = 0;
	virtual void LockCursor() = 0;
	virtual void SetTranslateExtendedKeys(bool state) = 0;
	virtual VPANEL GetTopmostPopup() = 0;
	virtual void SetTopLevelFocus(VPANEL panel) = 0;
	virtual HFont CreateFont() = 0;
	virtual bool SetFontGlyphSet(HFont font, const char* windowsFontName, int tall, int weight, int blur, int scanlines, int flags, int nRangeMin = 0, int nRangeMax = 0) = 0;
	virtual bool AddCustomFontFile(const char* fontName, const char* fontFileName) = 0;
	virtual int GetFontTall(HFont font) = 0;
	virtual int GetFontTallRequested(HFont font) = 0;
	virtual int GetFontAscent(HFont font, wchar_t wch) = 0;
	virtual bool IsFontAdditive(HFont font) = 0;
	virtual void GetCharABCwide(HFont font, int ch, int& a, int& b, int& c) = 0;
	virtual int GetCharacterWidth(HFont font, int ch) = 0;
	virtual void GetTextSize(HFont font, const wchar_t* text, int& wide, int& tall) = 0;
	virtual VPANEL GetNotifyPanel() = 0;
	virtual void SetNotifyIcon(VPANEL context, HTexture icon, VPANEL panelToReceiveMessages, const char* text) = 0;
	virtual void PlaySound(const char* fileName) = 0;
	virtual int GetPopupCount() = 0;
	virtual VPANEL GetPopup(int index) = 0;
	virtual bool ShouldPaintChildPanel(VPANEL childPanel) = 0;
	virtual bool RecreateContext(VPANEL panel) = 0;
	virtual void AddPanel(VPANEL panel) = 0;
	virtual void ReleasePanel(VPANEL panel) = 0;
	virtual void MovePopupToFront(VPANEL panel) = 0;
	virtual void MovePopupToBack(VPANEL panel) = 0;
	virtual void SolveTraverse(VPANEL panel, bool forceApplySchemeSettings = false) = 0;
	virtual void PaintTraverse(VPANEL panel) = 0;
	virtual void EnableMouseCapture(VPANEL panel, bool state) = 0;
	virtual void GetWorkspaceBounds(int& x, int& y, int& wide, int& tall) = 0;
	virtual void GetAbsoluteWindowBounds(int& x, int& y, int& wide, int& tall) = 0;
	virtual void GetProportionalBase(int& width, int& height) = 0;
	virtual void CalculateMouseVisible() = 0;
	virtual bool NeedKBInput() = 0;
	virtual bool HasCursorPosFunctions() = 0;
	virtual void SurfaceGetCursorPos(int& x, int& y) = 0;
	virtual void SurfaceSetCursorPos(int x, int y) = 0;
	virtual void DrawTexturedLine(const Vertex_t& a, const Vertex_t& b) = 0;
	virtual void DrawOutlinedCircle(int x, int y, int radius, int segments) = 0;
	virtual void DrawTexturedPolyLine(const Vertex_t* p, int n) = 0;
	virtual void DrawTexturedSubRect(int x0, int y0, int x1, int y1, float texs0, float text0, float texs1, float text1) = 0;
	virtual void DrawTexturedPolygon(int n, Vertex_t* pVertice, bool bClipVertices = true) = 0;
	virtual const wchar_t* GetTitle(VPANEL panel) = 0;
	virtual bool IsCursorLocked(void) const = 0;
	virtual void SetWorkspaceInsets(int left, int top, int right, int bottom) = 0;
	virtual bool DrawGetUnicodeCharRenderInfo(wchar_t ch, CharRenderInfo& info) = 0;
	virtual void DrawRenderCharFromInfo(const CharRenderInfo& info) = 0;
	virtual void DrawSetAlphaMultiplier(float alpha) = 0;
	virtual float DrawGetAlphaMultiplier() = 0;
	virtual void SetAllowHTMLJavaScript(bool state) = 0;
	virtual void OnScreenSizeChanged(int nOldWidth, int nOldHeight) = 0;
	virtual HCursor CreateCursorFromFile(char const* curOrAniFile, char const* pPathID = 0) = 0;
	virtual IVguiMatInfo* DrawGetTextureMatInfoFactory(int id) = 0;
	virtual void PaintTraverseEx(VPANEL panel, bool paintPopups = false) = 0;
	virtual float GetZPos() const = 0;
	virtual void SetPanelForInput(VPANEL vpanel) = 0;
	virtual void DrawFilledRectFastFade(int x0, int y0, int x1, int y1, int fadeStartPt, int fadeEndPt, unsigned int alpha0, unsigned int alpha1, bool bHorizontal) = 0;
	virtual void DrawFilledRectFade(int x0, int y0, int x1, int y1, unsigned int alpha0, unsigned int alpha1, bool bHorizontal) = 0;
	virtual void DrawSetTextureRGBAEx(int id, const unsigned char* rgba, int wide, int tall, EImageFormat imageFormat) = 0;
	virtual void DrawSetTextScale(float sx, float sy) = 0;
	virtual bool SetBitmapFontGlyphSet(HFont font, const char* windowsFontName, float scalex, float scaley, int flags) = 0;
	virtual bool AddBitmapFontFile(const char* fontFileName) = 0;
	virtual void SetBitmapFontName(const char* pName, const char* pFontFilename) = 0;
	virtual const char* GetBitmapFontName(const char* pName) = 0;
	virtual void ClearTemporaryFontCache(void) = 0;
	virtual IImage* GetIconImageForFullPath(char const* pFullPath) = 0;
	virtual void DrawUnicodeString(const wchar_t* pwString, FontDrawType_t drawType = FONT_DRAW_DEFAULT) = 0;
	virtual void PrecacheFontCharacters(HFont font, const wchar_t* pCharacters) = 0;
	virtual const char* GetResolutionKey(void) const = 0;
	virtual const char* GetFontName(HFont font) = 0;
	virtual const char* GetFontFamilyName(HFont font) = 0;
	virtual void GetKernedCharWidth(HFont font, wchar_t ch, wchar_t chBefore, wchar_t chAfter, float& wide, float& abcA) = 0;
	virtual bool ForceScreenSizeOverride(bool bState, int wide, int tall) = 0;
	virtual bool ForceScreenPosOffset(bool bState, int x, int y) = 0;
	virtual void OffsetAbsPos(int& x, int& y) = 0;
	virtual void ResetFontCaches() = 0;
	virtual int GetTextureNumFrames(int id) = 0;
	virtual void DrawSetTextureFrame(int id, int nFrame, unsigned int* pFrameCache) = 0;
	virtual bool IsScreenSizeOverrideActive(void) = 0;
	virtual bool IsScreenPosOverrideActive(void) = 0;
	virtual void DestroyTextureID(int id) = 0;
	virtual void DrawUpdateRegionTextureRGBA(int nTextureID, int x, int y, const unsigned char* pchData, int wide, int tall, EImageFormat imageFormat) = 0;
	virtual bool BHTMLWindowNeedsPaint(IHTML* htmlwin) = 0;
	virtual const char* GetWebkitHTMLUserAgentString() = 0;
	virtual void* Deprecated_AccessChromeHTMLController() = 0;
	virtual void SetFullscreenViewport(int x, int y, int w, int h) = 0;
	virtual void GetFullscreenViewport(int& x, int& y, int& w, int& h) = 0;
	virtual void PushFullscreenViewport() = 0;
	virtual void PopFullscreenViewport() = 0;
	virtual void SetSoftwareCursor(bool bUseSoftwareCursor) = 0;
	virtual void PaintSoftwareCursor() = 0;

	void StartDrawing()
	{
		reinterpret_cast<void(__thiscall*)(void*)>(S::CMatSystemSurface_StartDrawing())(this);
	}

	void FinishDrawing()
	{
		reinterpret_cast<void(__thiscall*)(void*)>(S::CMatSystemSurface_FinishDrawing())(this);
	}
};

struct InputEvent_t;
typedef void (*GetMouseCallback_t)(int& x, int& y);
typedef void (*SetMouseCallback_t)(int x, int y);
typedef void (*PlaySoundFunc_t)(const char* pFileName);

class IMatSystemSurface : public ISurface
{
public:
	virtual void AttachToWindow(void* hwnd, bool bLetAppDriveInput = false) = 0;
	virtual void EnableWindowsMessages(bool bEnable) = 0;
	virtual void Begin3DPaint(int iLeft, int iTop, int iRight, int iBottom, bool bRenderToTexture = true) = 0;
	virtual void End3DPaint() = 0;
	virtual void DisableClipping(bool bDisable) = 0;
	virtual void GetClippingRect(int& left, int& top, int& right, int& bottom, bool& bClippingDisabled) = 0;
	virtual void SetClippingRect(int left, int top, int right, int bottom) = 0;
	virtual bool IsCursorLocked() const = 0;
	virtual void SetMouseCallbacks(GetMouseCallback_t getFunc, SetMouseCallback_t setFunc) = 0;
	virtual void InstallPlaySoundFunc(PlaySoundFunc_t soundFunc) = 0;
	virtual void DrawColoredCircle(int centerx, int centery, float radius, int r, int g, int b, int a) = 0;
	virtual int DrawColoredText(HFont font, int x, int y, int r, int g, int b, int a, const char* fmt, ...) = 0;
	virtual void DrawColoredTextRect(HFont font, int x, int y, int w, int h, int r, int g, int b, int a, const char* fmt, ...) = 0;
	virtual void DrawTextHeight(HFont font, int w, int& h, const char* fmt, ...) = 0;
	virtual int	DrawTextLen(HFont font, const char* fmt, ...) = 0;
	virtual void DrawPanelIn3DSpace(VPANEL pRootPanel, const VMatrix& panelCenterToWorld, int nPixelWidth, int nPixelHeight, float flWorldWidth, float flWorldHeight) = 0;
	virtual void DrawSetTextureMaterial(int id, IMaterial* pMaterial) = 0;
	virtual bool HandleInputEvent(const InputEvent_t& event) = 0;
	virtual void Set3DPaintTempRenderTarget(const char* pRenderTargetName) = 0;
	virtual void Reset3DPaintTempRenderTarget(void) = 0;
	virtual IMaterial* DrawGetTextureMaterial(int id) = 0;
	virtual void GetFullscreenViewportAndRenderTarget(int& x, int& y, int& w, int& h, ITexture** ppRenderTarget) = 0;
	virtual void SetFullscreenViewportAndRenderTarget(int x, int y, int w, int h, ITexture* pRenderTarget) = 0;
	virtual int  DrawGetTextureId(ITexture* pTexture) = 0;
	virtual void BeginSkinCompositionPainting() = 0;
	virtual void EndSkinCompositionPainting() = 0;
};

#define VGUI_SURFACE_INTERFACE_VERSION "VGUI_Surface030"