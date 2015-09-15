#pragma once
#include "types.h"
#include "lightlut.h"

//-----------------------------------------------------------------------------
// Material
//-----------------------------------------------------------------------------

typedef struct
{
	float ambient[3];
	float diffuse[3];
	float specular0[3];
	float specular1[3];
	float emission[3];
} C3D_Material;

//-----------------------------------------------------------------------------
// Light environment
//-----------------------------------------------------------------------------

// Forward declarations
typedef struct C3D_Light_t C3D_Light;
typedef struct C3D_LightEnv_t C3D_LightEnv;

typedef struct
{
	u32 abs, select, scale;
} C3D_LightLutInputConf;

typedef struct
{
	u32 ambient;
	u32 numLights;
	u32 config[2];
	C3D_LightLutInputConf lutInput;
	u32 permutation;
} C3D_LightEnvConf;

enum
{
	C3DF_LightEnv_Dirty    = BIT(0),
	C3DF_LightEnv_MtlDirty = BIT(1),
	C3DF_LightEnv_LCDirty  = BIT(2),

#define C3DF_LightEnv_LutDirty(n) BIT(26+(n))
#define C3DF_LightEnv_LutDirtyAll (0x3F<<26)
};

struct C3D_LightEnv_t
{
	void (* Update)(C3D_LightEnv* env);
	void (* Dirty)(C3D_LightEnv* env);
	u32 flags;
	C3D_LightLut* luts[6];
	float ambient[3];
	C3D_Light* lights[8];
	C3D_LightEnvConf conf;
	C3D_Material material;
};

void C3D_LightEnvInit(C3D_LightEnv* env);
void C3D_LightEnvBind(C3D_LightEnv* env);

void C3D_LightEnvMaterial(C3D_LightEnv* env, const C3D_Material* mtl);
void C3D_LightEnvAmbient(C3D_LightEnv* env, float r, float g, float b);
void C3D_LightEnvLut(C3D_LightEnv* env, int lutId, int input, bool abs, C3D_LightLut* lut);

//-----------------------------------------------------------------------------
// Light
//-----------------------------------------------------------------------------

typedef struct
{
	u32 specular0, specular1, diffuse, ambient;
} C3D_LightMatConf;

typedef struct
{
	C3D_LightMatConf material;
	u16 position[3]; u16 padding0;
	u16 spotDir[3];  u16 padding1;
	u32 padding2;
	u32 config;
	u32 distAttnBias, distAttnScale;
} C3D_LightConf;

enum
{
	C3DF_Light_Enabled  = BIT(0),
	C3DF_Light_Dirty    = BIT(1),
	C3DF_Light_MatDirty = BIT(2),
	//C3DF_Light_Shadow   = BIT(3),
	//C3DF_Light_Spot     = BIT(4),
	//C3DF_Light_DistAttn = BIT(5),

	C3DF_Light_SPDirty  = BIT(14),
	C3DF_Light_DADirty  = BIT(15),
};

struct C3D_Light_t
{
	u16 flags, id;
	C3D_LightEnv* parent;
	C3D_LightLut *lut_SP, *lut_DA;
	float color[3];
	C3D_LightConf conf;
};

int  C3D_LightInit(C3D_Light* light, C3D_LightEnv* env);
void C3D_LightEnable(C3D_Light* light, bool enable);
void C3D_LightTwoSideDiffuse(C3D_Light* light, bool enable);
void C3D_LightGeoFactor(C3D_Light* light, int id, bool enable);
void C3D_LightColor(C3D_Light* light, float r, float g, float b);
void C3D_LightPosition(C3D_Light* light, C3D_FVec* pos);
void C3D_LightShadowEnable(C3D_Light* light, bool enable);
void C3D_LightSpotEnable(C3D_Light* light, bool enable);
void C3D_LightSpotDir(C3D_Light* light, float x, float y, float z);
void C3D_LightSpotLut(C3D_Light* light, C3D_LightLut* lut);
void C3D_LightDistAttnEnable(C3D_Light* light, bool enable);
void C3D_LightDistAttn(C3D_Light* light, float bias, float scale);
void C3D_LightDistAttnLut(C3D_Light* light, C3D_LightLut* lut);
