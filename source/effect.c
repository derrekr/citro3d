#include "context.h"

static inline C3D_Effect* getEffect()
{
	C3D_Context* ctx = C3Di_GetContext();
	ctx->flags |= C3DiF_Effect;
	return &ctx->effect;
}

void C3D_DepthRange(float near, float far)
{
	C3D_Effect* e = getEffect();
	e->drNear = C3Di_Float24(near);
	e->drFar = C3Di_Float24(far);
}

void C3D_CullFace(GPU_CULLMODE mode)
{
	C3D_Effect* e = getEffect();
	e->cullMode = mode;
}

void C3D_StencilTest(bool enable, GPU_TESTFUNC function, int ref, int mask, int replace)
{
	C3D_Effect* e = getEffect();
	e->stencilMode = (!!enable) | ((function & 7) << 4) | (replace << 8) | (ref << 16) | (mask << 24);
}

void C3D_StencilOp(GPU_STENCILOP sfail, GPU_STENCILOP dfail, GPU_STENCILOP pass)
{
	C3D_Effect* e = getEffect();
	e->stencilOp = sfail | (dfail << 4) | (pass << 8);
}

void C3D_BlendingColor(u32 color)
{
	C3D_Effect* e = getEffect();
	e->blendClr = color;
}

void C3D_DepthTest(bool enable, GPU_TESTFUNC function, GPU_WRITEMASK writemask)
{
	C3D_Effect* e = getEffect();
	e->depthTest = (!!enable) | ((function & 7) << 4) | (writemask << 8);
}

void C3D_AlphaTest(bool enable, GPU_TESTFUNC function, int ref)
{
	C3D_Effect* e = getEffect();
	e->alphaTest = (!!enable) | ((function & 7) << 4) | (ref << 8);
}

void C3D_AlphaBlend(GPU_BLENDEQUATION colorEq, GPU_BLENDEQUATION alphaEq, GPU_BLENDFACTOR srcClr, GPU_BLENDFACTOR dstClr, GPU_BLENDFACTOR srcAlpha, GPU_BLENDFACTOR dstAlpha)
{
	C3D_Effect* e = getEffect();
	e->alphaBlend = colorEq | (alphaEq << 8) | (srcClr << 16) | (dstClr << 20) | (srcAlpha << 24) | (dstAlpha << 28);
}

void C3Di_EffectBind(C3D_Effect* e)
{
	GPUCMD_AddSingleParam(0x000F006D, 0x00000001);
	GPUCMD_AddSingleParam(0x000F0040, e->cullMode & 0x3);
	GPUCMD_Add(0x800F004D, (u32*)&e->drNear, 2);
	GPUCMD_Add(0x800F0104, (u32*)&e->alphaTest, 4);
	GPUCMD_AddSingleParam(0x000F0103, e->blendClr);
	GPUCMD_AddSingleParam(0x000F0101, e->alphaBlend);
	GPUCMD_AddSingleParam(0x00020100, 0x00000100);

	// Wat
	GPUCMD_AddSingleParam(0x00010062, 0);
	GPUCMD_AddSingleParam(0x000F0118, 0);
}
