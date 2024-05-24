#pragma once
#include "../IAppSystem/IAppSystem.h"
#include "../MatRenderContext/MatRenderContext.h"
#include "../Texture/Texture.h"

struct MaterialSystem_Config_t;
struct MaterialSystemHardwareIdentifier_t;
class ITextureCompositor;
class KeyValues;
class IShader;
class IFileList;

enum HDRType_t
{
	HDR_TYPE_NONE,
	HDR_TYPE_INTEGER,
	HDR_TYPE_FLOAT
};

enum MaterialContextType_t
{
	MATERIAL_HARDWARE_CONTEXT,
	MATERIAL_QUEUED_CONTEXT,
	MATERIAL_NULL_CONTEXT
};

struct MaterialSystem_SortInfo_t
{
	IMaterial* material;
	int lightmapPageID;
};

enum
{
	MATERIAL_ADAPTER_NAME_LENGTH = 512
};
struct MaterialAdapterInfo_t
{
	char m_pDriverName[MATERIAL_ADAPTER_NAME_LENGTH];
	unsigned int m_VendorID;
	unsigned int m_DeviceID;
	unsigned int m_SubSysID;
	unsigned int m_Revision;
	int m_nDXSupportLevel;
	int m_nMaxDXSupportLevel;
	unsigned int m_nDriverVersionHigh;
	unsigned int m_nDriverVersionLow;
};

struct MaterialVideoMode_t
{
	int m_Width;
	int m_Height;
	EImageFormat m_Format;
	int m_RefreshRate;
};

enum MaterialThreadMode_t
{
	MATERIAL_SINGLE_THREADED,
	MATERIAL_QUEUED_SINGLE_THREADED,
	MATERIAL_QUEUED_THREADED
};

typedef void (*MaterialBufferReleaseFunc_t)();
typedef void (*MaterialBufferRestoreFunc_t)(int nChangeFlags);
typedef void (*ModeChangeCallbackFunc_t)(void);

typedef unsigned short MaterialHandle_t;

class IMaterialProxyFactory;
class IMaterialSystemHardwareConfig;

using MaterialLock_t = void*;

class IMaterialSystem : public IAppSystem
{
public:
	virtual bool Connect(CreateInterfaceFn factory) = 0;
	virtual void Disconnect() = 0;
	virtual void* QueryInterface(const char* pInterfaceName) = 0;
	virtual InitReturnVal_t Init() = 0;
	virtual void Shutdown() = 0;
	virtual CreateInterfaceFn	Init(char const* pShaderAPIDLL, IMaterialProxyFactory* pMaterialProxyFactory, CreateInterfaceFn fileSystemFactory, CreateInterfaceFn cvarFactory = NULL) = 0;
	virtual void SetShaderAPI(char const* pShaderAPIDLL) = 0;
	virtual void SetAdapter(int nAdapter, int nFlags) = 0;
	virtual void ModInit() = 0;
	virtual void ModShutdown() = 0;
	virtual void SetThreadMode(MaterialThreadMode_t mode, int nServiceThread = -1) = 0;
	virtual MaterialThreadMode_t GetThreadMode() = 0;
	virtual bool IsRenderThreadSafe() = 0;
	virtual void ExecuteQueued() = 0;
	virtual IMaterialSystemHardwareConfig* GetHardwareConfig(const char* pVersion, int* returnCode) = 0;
	virtual bool UpdateConfig(bool bForceUpdate) = 0;
	virtual bool OverrideConfig(const MaterialSystem_Config_t& config, bool bForceUpdate) = 0;
	virtual const MaterialSystem_Config_t& GetCurrentConfigForVideoCard() const = 0;
	virtual bool GetRecommendedConfigurationInfo(int nDXLevel, KeyValues* pKeyValues) = 0;
	virtual int GetDisplayAdapterCount() const = 0;
	virtual int GetCurrentAdapter() const = 0;
	virtual void GetDisplayAdapterInfo(int adapter, MaterialAdapterInfo_t& info) const = 0;
	virtual int GetModeCount(int adapter) const = 0;
	virtual void GetModeInfo(int adapter, int mode, MaterialVideoMode_t& info) const = 0;
	virtual void AddModeChangeCallBack(ModeChangeCallbackFunc_t func) = 0;
	virtual void GetDisplayMode(MaterialVideoMode_t& mode) const = 0;
	virtual bool SetMode(void* hwnd, const MaterialSystem_Config_t& config) = 0;
	virtual bool SupportsMSAAMode(int nMSAAMode) = 0;
	virtual const MaterialSystemHardwareIdentifier_t& GetVideoCardIdentifier(void) const = 0;
	virtual void SpewDriverInfo() const = 0;
	virtual void GetDXLevelDefaults(unsigned int& max_dxlevel, unsigned int& recommended_dxlevel) = 0;
	virtual void GetBackBufferDimensions(int& width, int& height) const = 0;
	virtual EImageFormat GetBackBufferFormat() const = 0;
	virtual bool SupportsHDRMode(HDRType_t nHDRModede) = 0;
	virtual bool AddView(void* hwnd) = 0;
	virtual void RemoveView(void* hwnd) = 0;
	virtual void SetView(void* hwnd) = 0;
	virtual void BeginFrame(float frameTime) = 0;
	virtual void EndFrame() = 0;
	virtual void Flush(bool flushHardware = false) = 0;
	virtual void SwapBuffers() = 0;
	virtual void EvictManagedResources() = 0;
	virtual void ReleaseResources(void) = 0;
	virtual void ReacquireResources(void) = 0;
	virtual void AddReleaseFunc(MaterialBufferReleaseFunc_t func) = 0;
	virtual void RemoveReleaseFunc(MaterialBufferReleaseFunc_t func) = 0;
	virtual void AddRestoreFunc(MaterialBufferRestoreFunc_t func) = 0;
	virtual void RemoveRestoreFunc(MaterialBufferRestoreFunc_t func) = 0;
	virtual void ResetTempHWMemory(bool bExitingLevel = false) = 0;
	virtual void HandleDeviceLost() = 0;
	virtual int ShaderCount() const = 0;
	virtual int GetShaders(int nFirstShader, int nMaxCount, IShader** ppShaderList) const = 0;
	virtual int ShaderFlagCount() const = 0;
	virtual const char* ShaderFlagName(int nIndex) const = 0;
	virtual void GetShaderFallback(const char* pShaderName, char* pFallbackShader, int nFallbackLength) = 0;
	virtual IMaterialProxyFactory* GetMaterialProxyFactory() = 0;
	virtual void SetMaterialProxyFactory(IMaterialProxyFactory* pFactory) = 0;
	virtual void EnableEditorMaterials() = 0;
	virtual void SetInStubMode(bool bInStubMode) = 0;
	virtual void DebugPrintUsedMaterials(const char* pSearchSubString, bool bVerbose) = 0;
	virtual void DebugPrintUsedTextures(void) = 0;
	virtual void ToggleSuppressMaterial(char const* pMaterialName) = 0;
	virtual void ToggleDebugMaterial(char const* pMaterialName) = 0;
	virtual bool UsingFastClipping(void) = 0;
	virtual int StencilBufferBits(void) = 0;
	virtual void SuspendTextureStreaming() = 0;
	virtual void ResumeTextureStreaming() = 0;
	virtual void UncacheAllMaterials() = 0;
	virtual void UncacheUnusedMaterials(bool bRecomputeStateSnapshots = false) = 0;
	virtual void CacheUsedMaterials() = 0;
	virtual void ReloadTextures() = 0;
	virtual void ReloadMaterials(const char* pSubString = NULL) = 0;
	virtual IMaterial* CreateMaterial(const char* pMaterialName, KeyValues* pVMTKeyValues) = 0;
	virtual IMaterial* FindMaterial(char const* pMaterialName, const char* pTextureGroupName, bool complain = true, const char* pComplainPrefix = NULL) = 0;
	virtual bool IsMaterialLoaded(char const* pMaterialName) = 0;
	virtual MaterialHandle_t FirstMaterial() const = 0;
	virtual MaterialHandle_t NextMaterial(MaterialHandle_t h) const = 0;
	virtual MaterialHandle_t InvalidMaterial() const = 0;
	virtual IMaterial* GetMaterial(MaterialHandle_t h) const = 0;
	virtual int GetNumMaterials() const = 0;
	virtual void SetAsyncTextureLoadCache(void* hFileCache) = 0;
	virtual ITexture* FindTexture(char const* pTextureName, const char* pTextureGroupName, bool complain = true, int nAdditionalCreationFlags = 0) = 0;
	virtual bool IsTextureLoaded(char const* pTextureName) const = 0;
	virtual ITexture* CreateProceduralTexture(const char* pTextureName, const char* pTextureGroupName, int w, int h, EImageFormat fmt, int nFlags) = 0;
	virtual void BeginRenderTargetAllocation() = 0;
	virtual void EndRenderTargetAllocation() = 0;
	virtual ITexture* CreateRenderTargetTexture(int w, int h, RenderTargetSizeMode_t sizeMode, EImageFormat	format, MaterialRenderTargetDepth_t depth = MATERIAL_RT_DEPTH_SHARED) = 0;
	virtual ITexture* CreateNamedRenderTargetTextureEx(const char* pRTName, int w, int h, RenderTargetSizeMode_t sizeMode, EImageFormat format, MaterialRenderTargetDepth_t depth = MATERIAL_RT_DEPTH_SHARED, unsigned int textureFlags = TEXTUREFLAGS_CLAMPS | TEXTUREFLAGS_CLAMPT, unsigned int renderTargetFlags = 0) = 0;
	virtual ITexture* CreateNamedRenderTargetTexture(const char* pRTName, int w, int h, RenderTargetSizeMode_t sizeMode, EImageFormat format, MaterialRenderTargetDepth_t depth = MATERIAL_RT_DEPTH_SHARED, bool bClampTexCoords = true, bool bAutoMipMap = false) = 0;
	virtual ITexture* CreateNamedRenderTargetTextureEx2(const char* pRTName, int w, int h, RenderTargetSizeMode_t sizeMode, EImageFormat format, MaterialRenderTargetDepth_t depth = MATERIAL_RT_DEPTH_SHARED, unsigned int textureFlags = TEXTUREFLAGS_CLAMPS | TEXTUREFLAGS_CLAMPT, unsigned int renderTargetFlags = 0) = 0;
	virtual void BeginLightmapAllocation() = 0;
	virtual void EndLightmapAllocation() = 0;
	virtual int AllocateLightmap(int width, int height, int offsetIntoLightmapPage[2], IMaterial* pMaterial) = 0;
	virtual int AllocateWhiteLightmap(IMaterial* pMaterial) = 0;
	virtual void UpdateLightmap(int lightmapPageID, int lightmapSize[2], int offsetIntoLightmapPage[2], float* pFloatImage, float* pFloatImageBump1, float* pFloatImageBump2, float* pFloatImageBump3) = 0;
	virtual int GetNumSortIDs() = 0;
	virtual void GetSortInfo(MaterialSystem_SortInfo_t* sortInfoArray) = 0;
	virtual void GetLightmapPageSize(int lightmap, int* width, int* height) const = 0;
	virtual void ResetMaterialLightmapPageInfo() = 0;
	virtual void ClearBuffers(bool bClearColor, bool bClearDepth, bool bClearStencil = false) = 0;
	virtual IMatRenderContext* GetRenderContext() = 0;
	virtual bool SupportsShadowDepthTextures(void) = 0;
	virtual void BeginUpdateLightmaps(void) = 0;
	virtual void EndUpdateLightmaps(void) = 0;
	virtual MaterialLock_t Lock() = 0;
	virtual void Unlock(MaterialLock_t) = 0;
	virtual EImageFormat GetShadowDepthTextureFormat() = 0;
	virtual bool SupportsFetch4(void) = 0;
	virtual IMatRenderContext* CreateRenderContext(MaterialContextType_t type) = 0;
	virtual IMatRenderContext* SetRenderContext(IMatRenderContext*) = 0;
	virtual bool SupportsCSAAMode(int nNumSamples, int nQualityLevel) = 0;
	virtual void RemoveModeChangeCallBack(ModeChangeCallbackFunc_t func) = 0;
	virtual IMaterial* FindProceduralMaterial(const char* pMaterialName, const char* pTextureGroupName, KeyValues* pVMTKeyValues) = 0;
	virtual EImageFormat GetNullTextureFormat() = 0;
	virtual void AddTextureAlias(const char* pAlias, const char* pRealName) = 0;
	virtual void RemoveTextureAlias(const char* pAlias) = 0;
	virtual int AllocateDynamicLightmap(int lightmapSize[2], int* pOutOffsetIntoPage, int frameID) = 0;
	virtual void SetExcludedTextures(const char* pScriptName) = 0;
	virtual void UpdateExcludedTextures(void) = 0;
	virtual bool IsInFrame() const = 0;
	virtual void CompactMemory() = 0;
	virtual void ReloadFilesInList(IFileList* pFilesToReload) = 0;
	virtual	bool AllowThreading(bool bAllow, int nServiceThread) = 0;
	virtual IMaterial* FindMaterialEx(char const* pMaterialName, const char* pTextureGroupName, int nContext, bool complain = true, const char* pComplainPrefix = NULL) = 0;
	virtual void SetRenderTargetFrameBufferSizeOverrides(int nWidth, int nHeight) = 0;
	virtual void GetRenderTargetFrameBufferDimensions(int& nWidth, int& nHeight) = 0;
	virtual char* GetDisplayDeviceName() const = 0;
	virtual ITexture* CreateTextureFromBits(int w, int h, int mips, EImageFormat fmt, int srcBufferSize, byte* srcBits) = 0;
	virtual void OverrideRenderTargetAllocation(bool rtAlloc) = 0;
	virtual ITextureCompositor* NewTextureCompositor(int w, int h, const char* pCompositeName, int nTeamNum, uint64 randomSeed, KeyValues* stageDesc, uint32 texCompositeCreateFlags = 0) = 0;
	virtual void AsyncFindTexture(const char* pFilename, const char* pTextureGroupName, IAsyncTextureOperationReceiver* pRecipient, void* pExtraArgs, bool bComplain = true, int nAdditionalCreationFlags = 0) = 0;
	virtual ITexture* CreateNamedTextureFromBitsEx(const char* pName, const char* pTextureGroupName, int w, int h, int mips, EImageFormat fmt, int srcBufferSize, byte* srcBits, int nFlags) = 0;
	virtual bool AddTextureCompositorTemplate(const char* pName, KeyValues* pTmplDesc, int nTexCompositeTemplateFlags = 0) = 0;
	virtual bool VerifyTextureCompositorTemplates() = 0;
};

#define VMATERIALSYSTEM_INTERFACE "VMaterialSystem081"