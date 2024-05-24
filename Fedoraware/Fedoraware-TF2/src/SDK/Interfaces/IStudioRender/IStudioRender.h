#pragma once
#include "../../Includes/Includes.h"
#include "../IAppSystem/IAppSystem.h"
#include "../MatRenderContext/MatRenderContext.h"

struct studiohdr_t;
struct studiomeshdata_t;
class IMaterial;
struct studiohwdata_t;
struct Ray_t;
class IMaterialSystem;
struct vertexFileHeader_t;
struct FlashlightState_t;
struct FileHeader_t;
class IPooledVBAllocator;
class CUtlBuffer;

typedef void (*StudioRender_Printf_t)(const char* fmt, ...);
typedef unsigned short MDLHandle_t;

enum LightType_t
{
	MATERIAL_LIGHT_DISABLE = 0,
	MATERIAL_LIGHT_POINT,
	MATERIAL_LIGHT_DIRECTIONAL,
	MATERIAL_LIGHT_SPOT
};

enum LightType_OptimizationFlags_t
{
	LIGHTTYPE_OPTIMIZATIONFLAGS_HAS_ATTENUATION0 = 1,
	LIGHTTYPE_OPTIMIZATIONFLAGS_HAS_ATTENUATION1 = 2,
	LIGHTTYPE_OPTIMIZATIONFLAGS_HAS_ATTENUATION2 = 4,
	LIGHTTYPE_OPTIMIZATIONFLAGS_DERIVED_VALUES_CALCED = 8
};

struct LightDesc_t
{
public:
	LightType_t m_Type;
	Vector m_Color;
	Vector m_Position;
	Vector m_Direction;
	float  m_Range;
	float m_Falloff;
	float m_Attenuation0;
	float m_Attenuation1;
	float m_Attenuation2;
	float m_Theta;
	float m_Phi;
	float m_ThetaDot;
	float m_PhiDot;
	unsigned int m_Flags;
	float OneOver_ThetaDot_Minus_PhiDot;
	float m_RangeSquared;

	bool IsDirectionWithinLightCone(const Vector& rdir) const
	{
		return ((m_Type != MATERIAL_LIGHT_SPOT) || (rdir.Dot(m_Direction) >= m_PhiDot));
	}

	float OneOverThetaDotMinusPhiDot() const
	{
		return OneOver_ThetaDot_Minus_PhiDot;
	}
};

struct StudioRenderConfig_t
{
	float fEyeShiftX;
	float fEyeShiftY;
	float fEyeShiftZ;
	float fEyeSize;
	float fEyeGlintPixelWidthLODThreshold;
	int maxDecalsPerModel;
	int drawEntities;
	int skin;
	int fullbright;
	bool bEyeMove : 1;
	bool bSoftwareSkin : 1;
	bool bNoHardware : 1;
	bool bNoSoftware : 1;
	bool bTeeth : 1;
	bool bEyes : 1;
	bool bFlex : 1;
	bool bWireframe : 1;
	bool bDrawNormals : 1;
	bool bDrawTangentFrame : 1;
	bool bDrawZBufferedWireframe : 1;
	bool bSoftwareLighting : 1;
	bool bShowEnvCubemapOnly : 1;
	bool bWireframeDecals : 1;
	int m_nReserved[4];
};

#define STUDIORENDER_DECAL_INVALID  ( (StudioDecalHandle_t)0 )

enum
{
	ADDDECAL_TO_ALL_LODS = -1
};

enum
{
	STUDIORENDER_DRAW_ENTIRE_MODEL = 0,
	STUDIORENDER_DRAW_OPAQUE_ONLY = 0x01,
	STUDIORENDER_DRAW_TRANSLUCENT_ONLY = 0x02,
	STUDIORENDER_DRAW_GROUP_MASK = 0x03,
	STUDIORENDER_DRAW_NO_FLEXES = 0x04,
	STUDIORENDER_DRAW_STATIC_LIGHTING = 0x08,
	STUDIORENDER_DRAW_ACCURATETIME = 0x10,
	STUDIORENDER_DRAW_NO_SHADOWS = 0x20,
	STUDIORENDER_DRAW_GET_PERF_STATS = 0x40,
	STUDIORENDER_DRAW_WIREFRAME = 0x80,
	STUDIORENDER_DRAW_ITEM_BLINK = 0x100,
	STUDIORENDER_SHADOWDEPTHTEXTURE = 0x200,
	STUDIORENDER_SSAODEPTHTEXTURE = 0x1000,
	STUDIORENDER_GENERATE_STATS = 0x8000
};

#define	VERTEX_POSITION 0x0001
#define	VERTEX_NORMAL 0x0002
#define	VERTEX_COLOR 0x0004
#define	VERTEX_SPECULAR 0x0008
#define	VERTEX_TANGENT_S 0x0010
#define	VERTEX_TANGENT_T 0x0020
#define	VERTEX_TANGENT_SPACE ( VERTEX_TANGENT_S | VERTEX_TANGENT_T )
#define	VERTEX_WRINKLE 0x0040
#define	VERTEX_BONE_INDEX 0x0080
#define	VERTEX_FORMAT_VERTEX_SHADER 0x0100
#define	VERTEX_FORMAT_USE_EXACT_FORMAT 0x0200
#define	VERTEX_FORMAT_COMPRESSED 0x400
#define	VERTEX_LAST_BIT 10
#define	VERTEX_BONE_WEIGHT_BIT (VERTEX_LAST_BIT + 1)
#define	USER_DATA_SIZE_BIT (VERTEX_LAST_BIT + 4)
#define	TEX_COORD_SIZE_BIT (VERTEX_LAST_BIT + 7)
#define	VERTEX_BONE_WEIGHT_MASK ( 0x7 << VERTEX_BONE_WEIGHT_BIT )
#define	USER_DATA_SIZE_MASK ( 0x7 << USER_DATA_SIZE_BIT )
#define	VERTEX_FORMAT_FIELD_MASK 0x0FF
#define	VERTEX_FORMAT_UNKNOWN 0

#define VERTEX_BONEWEIGHT( _n ) ((_n) << VERTEX_BONE_WEIGHT_BIT)
#define VERTEX_USERDATA_SIZE( _n ) ((_n) << USER_DATA_SIZE_BIT)
#define VERTEX_TEXCOORD_MASK( _coord ) (( 0x7ULL ) << ( TEX_COORD_SIZE_BIT + 3 * (_coord) ))

#define VERTEX_TEXCOORD0_2D ( ( (uint64) 2 ) << ( TEX_COORD_SIZE_BIT + ( 3*0 ) ) )

enum MaterialVertexFormat_t
{
	MATERIAL_VERTEX_FORMAT_MODEL_SKINNED = (VertexFormat_t)VERTEX_POSITION | VERTEX_COLOR | VERTEX_NORMAL | VERTEX_TEXCOORD0_2D | VERTEX_BONEWEIGHT(2) | VERTEX_BONE_INDEX | VERTEX_USERDATA_SIZE(4),
	MATERIAL_VERTEX_FORMAT_MODEL_SKINNED_DX7 = (VertexFormat_t)VERTEX_POSITION | VERTEX_COLOR | VERTEX_NORMAL | VERTEX_TEXCOORD0_2D | VERTEX_BONEWEIGHT(2) | VERTEX_BONE_INDEX,
	MATERIAL_VERTEX_FORMAT_MODEL = (VertexFormat_t)VERTEX_POSITION | VERTEX_COLOR | VERTEX_NORMAL | VERTEX_TEXCOORD0_2D | VERTEX_USERDATA_SIZE(4),
	MATERIAL_VERTEX_FORMAT_MODEL_DX7 = (VertexFormat_t)VERTEX_POSITION | VERTEX_COLOR | VERTEX_NORMAL | VERTEX_TEXCOORD0_2D,
	MATERIAL_VERTEX_FORMAT_COLOR = (VertexFormat_t)VERTEX_SPECULAR
};

enum OverrideType_t
{
	OVERRIDE_NORMAL = 0,
	OVERRIDE_BUILD_SHADOWS,
	OVERRIDE_DEPTH_WRITE,
	OVERRIDE_SSAO_DEPTH_WRITE
};

enum
{
	USESHADOWLOD = -2
};

#define MAX_DRAW_MODEL_INFO_MATERIALS 8

struct DrawModelResults_t
{
	int m_ActualTriCount;
	int m_TextureMemoryBytes;
	int m_NumHardwareBones;
	int m_NumBatches;
	int m_NumMaterials;
	int m_nLODUsed;
	int m_flLODMetric;
	//CFastTimer m_RenderTime;
	//CUtlVector<IMaterial *> m_Materials;
};

struct ColorTexelsInfo_t
{
	int m_nWidth;
	int m_nHeight;
	int m_nMipmapCount;
	EImageFormat m_ImageFormat;
	int m_nByteCount;
	byte* m_pTexelData;
};

struct ColorMeshInfo_t
{
	IMesh* m_pMesh;
	IPooledVBAllocator* m_pPooledVBAllocator;
	int m_nVertOffsetInBytes;
	int m_nNumVerts;
	ITexture* m_pLightmap;
	ColorTexelsInfo_t* m_pLightmapData;
};

struct DrawModelInfo_t
{
	studiohdr_t* m_pStudioHdr;
	studiohwdata_t* m_pHardwareData;
	StudioDecalHandle_t m_Decals;
	int m_Skin;
	int m_Body;
	int m_HitboxSet;
	void* m_pClientEntity;
	int m_Lod;
	ColorMeshInfo_t* m_pColorMeshes;
	bool m_bStaticLighting;
	Vec3 m_vecAmbientCube[6];
	int m_nLocalLightCount;
	LightDesc_t m_LocalLightDescs[4];
};

struct GetTriangles_Vertex_t
{
	Vec3 m_Position;
	Vec3 m_Normal;
	Vector4D m_TangentS;
	Vector2D m_TexCoord;
	Vector4D m_BoneWeight;
	int m_BoneIndex[4];
	int m_NumBones;
};

struct GetTriangles_MaterialBatch_t
{
	IMaterial* m_pMaterial;
	CUtlVector<GetTriangles_Vertex_t> m_Verts;
	CUtlVector<int> m_TriListIndices;
};

struct GetTriangles_Output_t
{
	CUtlVector<GetTriangles_MaterialBatch_t> m_MaterialBatches;
	matrix3x4 m_PoseToWorld[MAXSTUDIOBONES];
};

struct model_array_instance_t
{
	matrix3x4 modelToWorld;
};

class IStudioDataCache : public IAppSystem
{
public:
	virtual bool VerifyHeaders(studiohdr_t* pStudioHdr) = 0;
	virtual vertexFileHeader_t* CacheVertexData(studiohdr_t* pStudioHdr) = 0;
};

class IStudioRender : public IAppSystem
{
public:
	virtual void BeginFrame(void) = 0;
	virtual void EndFrame(void) = 0;
	virtual void Mat_Stub(IMaterialSystem* pMatSys) = 0;
	virtual void UpdateConfig(const StudioRenderConfig_t& config) = 0;
	virtual void GetCurrentConfig(StudioRenderConfig_t& config) = 0;
	virtual bool LoadModel(studiohdr_t* pStudioHdr, void* pVtxData, studiohwdata_t* pHardwareData) = 0;
	virtual void UnloadModel(studiohwdata_t* pHardwareData) = 0;
	virtual void RefreshStudioHdr(studiohdr_t* pStudioHdr, studiohwdata_t* pHardwareData) = 0;
	virtual void SetEyeViewTarget(const studiohdr_t* pStudioHdr, int nBodyIndex, const Vec3& worldPosition) = 0;
	virtual int GetNumAmbientLightSamples() = 0;
	virtual const Vec3* GetAmbientLightDirections() = 0;
	virtual void SetAmbientLightColors(const Vector4D* pAmbientOnlyColors) = 0;
	virtual void SetAmbientLightColors(const Vec3* pAmbientOnlyColors) = 0;
	virtual void SetLocalLights(int numLights, const LightDesc_t* pLights) = 0;
	virtual void SetViewState(const Vec3& viewOrigin, const Vec3& viewRight, const Vec3& viewUp, const Vec3& viewPlaneNormal) = 0;
	virtual void LockFlexWeights(int nWeightCount, float** ppFlexWeights, float** ppFlexDelayedWeights = NULL) = 0;
	virtual void UnlockFlexWeights() = 0;
	virtual matrix3x4* LockBoneMatrices(int nBoneCount) = 0;
	virtual void UnlockBoneMatrices() = 0;
	virtual int GetNumLODs(const studiohwdata_t& hardwareData) const = 0;
	virtual float GetLODSwitchValue(const studiohwdata_t& hardwareData, int lod) const = 0;
	virtual void SetLODSwitchValue(studiohwdata_t& hardwareData, int lod, float switchValue) = 0;
	virtual void SetColorModulation(float const* pColor) = 0;
	virtual void SetAlphaModulation(float flAlpha) = 0;
	virtual void DrawModel(DrawModelResults_t* pResults, const DrawModelInfo_t& info, matrix3x4* pBoneToWorld, float* pFlexWeights, float* pFlexDelayedWeights, const Vec3& modelOrigin, int flags = STUDIORENDER_DRAW_ENTIRE_MODEL) = 0;
	virtual void DrawModelStaticProp(const DrawModelInfo_t& drawInfo, const matrix3x4& modelToWorld, int flags = STUDIORENDER_DRAW_ENTIRE_MODEL) = 0;
	virtual void DrawStaticPropDecals(const DrawModelInfo_t& drawInfo, const matrix3x4& modelToWorld) = 0;
	virtual void DrawStaticPropShadows(const DrawModelInfo_t& drawInfo, const matrix3x4& modelToWorld, int flags) = 0;
	virtual void ForcedMaterialOverride(IMaterial* newMaterial, OverrideType_t nOverrideType = OVERRIDE_NORMAL) = 0;
	virtual StudioDecalHandle_t CreateDecalList(studiohwdata_t* pHardwareData) = 0;
	virtual void DestroyDecalList(StudioDecalHandle_t handle) = 0;
	virtual void AddDecal(StudioDecalHandle_t handle, studiohdr_t* pStudioHdr, matrix3x4* pBoneToWorld, const Ray_t& ray, const Vec3& decalUp, IMaterial* pDecalMaterial, float radius, int body, bool noPokethru = false, int maxLODToDecal = ADDDECAL_TO_ALL_LODS) = 0;
	virtual void ComputeLighting(const Vec3* pAmbient, int lightCount, LightDesc_t* pLights, const Vec3& pt, const Vec3& normal, Vec3& lighting) = 0;
	virtual void ComputeLightingConstDirectional(const Vec3* pAmbient, int lightCount, LightDesc_t* pLights, const Vec3& pt, const Vec3& normal, Vec3& lighting, float flDirectionalAmount) = 0;
	virtual void AddShadow(IMaterial* pMaterial, void* pProxyData, FlashlightState_t* m_pFlashlightState = NULL, VMatrix* pWorldToTexture = NULL, ITexture* pFlashlightDepthTexture = NULL) = 0;
	virtual void ClearAllShadows() = 0;
	virtual int ComputeModelLod(studiohwdata_t* pHardwareData, float unitSphereSize, float* pMetric = NULL) = 0;
	virtual void GetPerfStats(DrawModelResults_t* pResults, const DrawModelInfo_t& info, CUtlBuffer* pSpewBuf = NULL) const = 0;
	virtual void GetTriangles(const DrawModelInfo_t& info, matrix3x4* pBoneToWorld, GetTriangles_Output_t& out) = 0;
	virtual int GetMaterialList(studiohdr_t* pStudioHdr, int count, IMaterial** ppMaterials) = 0;
	virtual int GetMaterialListFromBodyAndSkin(MDLHandle_t studio, int nSkin, int nBody, int nCountOutputMaterials, IMaterial** ppOutputMaterials) = 0;
	virtual void DrawModelArray(const DrawModelInfo_t& drawInfo, int arrayCount, model_array_instance_t* pInstanceData, int instanceStride, int flags = STUDIORENDER_DRAW_ENTIRE_MODEL) = 0;
	virtual void GetMaterialOverride(IMaterial** ppOutForcedMaterial, OverrideType_t* pOutOverrideType) = 0;

	void SetColorModulation(float r, float g, float b)
	{
		const float clr[3] = { r, g, b };
		SetColorModulation(clr);
	}

	void SetColorModulation(Color_t clr)
	{
		float _clr[3] = { float(clr.r) / 255.f, float(clr.g) / 255.f, float(clr.b) / 255.f };
		SetColorModulation(_clr);
	}
};

#define VENGINE_STUDIORENDER_INTERFACE_VERSION	"VStudioRender025"