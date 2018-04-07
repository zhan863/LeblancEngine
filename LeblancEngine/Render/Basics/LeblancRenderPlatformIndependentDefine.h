#ifndef LEBLANC_RENDER_PLATFORM_INDEPENDENT_DEFINE_H
#define LEBLANC_RENDER_PLATFORM_INDEPENDENT_DEFINE_H

enum class RasterizerState : UINT
{
	CW_FRONT = 0,
	CW_BACK = 1,
	NONE = 2
};

enum class DepthStencilState : UINT
{
	ALL_PASS = 0
};

enum class BlendState : UINT
{
	BLEND_OPAQUE = 0
};

enum class GpuDataType : UINT 
{
	FLOAT = 0,
	FLOAT4 = 1,
	MATRIX = 2,
	FLOAT_ARRAY = 3,
	FLOAT4_ARRAY = 4,
	MATRIX_ARRAY = 5
};

#endif