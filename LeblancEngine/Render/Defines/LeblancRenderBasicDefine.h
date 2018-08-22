#ifndef LEBLANC_RENDER_BASIC_DEFINE_H
#define LEBLANC_RENDER_BASIC_DEFINE_H

#include <stdint.h>

namespace Leblanc 
{
	enum DrawMode
	{
		Point = 1,
		Wireframe = 2,
		Filled = 3
	};

	enum PrimitiveType
	{
		UndefinedPrimitiveType = 0,
		PointList = 1,
		LineList = 2,
		LineStrip = 3,
		TriangleList = 4,
		TriangleStrip = 5,
		LineListAdj = 10,
		LineStripAdj = 11,
		TriangleListAdj = 12,
		TringleStripAdj = 13,

		OneControlPointPatchList = 33,
		TwoControlPointPatchList = 34,
		ThreeControlPointPatchList = 35,
		FourControlPointPatchList = 36,
		FiveControlPointPatchList = 37,
		SixControlPointPatchList = 38,
		SevenControlPointPatchList = 39,
		EightControlPointPatchList = 40,
		NineControlPointPatchList = 41,
		TenControlPointPatchList = 42,
		ElevenControlPointPatchList = 43,
		TwelveControlPointPatchList = 44,
		ThirteenControlPointPatchList = 45,
		FourteenControlPointPatchList = 46,
		FifteenControlPointPatchList = 47,
		SixteenControlPointPatchList = 48,
		SeventeenControlPointPatchList = 49,
		EighteenControlPointPatchList = 50,
		NineteenControlPointPatchList = 51,
		TwentyControlPointPatchList = 52,
		TwentyOneControlPointPatchList = 53,
		TwentyTwoControlPointPatchList = 54,
		TwentyThreeControlPointPatchList = 55,
		TwentyFourControlPointPatchList = 56,
		TwentyFiveControlPointPatchList = 57,
		TwentySixControlPointPatchList = 58,
		TwentySevenControlPointPatchList = 59,
		TwentyEightControlPointPatchList = 60,
		TwentyNineControlPointPatchList = 61,
		ThirtyControlPointPatchList = 62,
		ThirtyOneControlPointPatchList = 63,
		ThirtyTwoControlPointPatchList = 64
	};

	enum BlendOp
	{
		OpAdd = 1,
		OpSubtract = 2,
		OpRevSubtract = 3,
		OpMin = 4,
		OpMax = 5
	};

	// Below are directx9 format types.
	enum AlphaFunction
	{
		BlendZero = 1,
		BlendOne = 2,
		SourceColor = 3,
		InverseSourceColor = 4,
		SourceAlpha = 5,
		InverseSourceAlpha = 6,
		DestinationAlpha = 7,
		InverseDestinationAlpha = 8,
		DestinationColor = 9,
		InverseDestinationColor = 10,
		SourceAlphaSaturated = 11,
		BlendFactor = 14,
		InverseBlendFactor = 15,
		Source1Color = 16,
		InverseSource1Color = 17,
		BlendSource1Alpha = 18,
		BlendInverseSource1Alpha = 19,
		BlendError = 20
	};


	enum BlendPipelineType
	{
		UnknownBlendPipelineType = 4,
		BlendOver = 0,
		BlendModulate = 1,
		BlendAlpha = 2,
		BlendPipelineOne = 3
		// Others go here.
	};

	enum CompareFunction
	{
		Never = 1,
		Less = 2,
		Equal = 3,
		LessEqual = 4,
		Greater = 5,
		NotEqual = 6,
		GreaterEqual = 7,
		Always = 8
	};

	enum StencilOp
	{
		StencilKeep = 1,
		StencilZero = 2,
		StencilReplace = 3,
		StencilIncrementSat = 4,
		StencilDecrementSat = 5,
		StencilInvert = 6,
		StencilIncrement = 7,
		StencilDecrement = 8
	};

	enum TextureDimension
	{
		OneD,
		TwoD,
		CubeMap,
		FourD,
		TwoDHDR,
		ThreeD,
		TextureLUT
	};

	enum TextureType
	{
		FromFile,
		RenderTarget,
		Procedural,
		StagingFromFile
	};

	enum DeclarationMethod
	{
		METHOD_DEFAULT = 0,
		METHOD_PARTIALU,
		METHOD_PARTIALV,
		METHOD_CROSSUV,              // Normal
		METHOD_UV,
		METHOD_LOOKUP,               // Lookup a displacement map
		METHOD_LOOKUPPRESAMPLED,     // Lookup a pre-sampled displacement map
	};

	enum DeclarationType
	{
		FLOAT1 = 0,  // 1D float expanded to (value, 0., 0., 1.)
		FLOAT2 = 1,  // 2D float expanded to (value, value, 0., 1.)
		FLOAT3 = 2,  // 3D float expanded to (value, value, value, 1.)
		FLOAT4 = 3,  // 4D float
		D3DCOLOR = 4,  // 4D packed unsigned bytes mapped to 0. to 1. range
					   // Input is in D3DCOLOR format (ARGB) expanded to (R, G, B, A)
					   UBYTE4 = 5,  // 4D unsigned byte
					   SHORT2 = 6,  // 2D signed short expanded to (value, value, 0., 1.)
					   SHORT4 = 7,  // 4D signed short
					   UBYTE4N = 8,  // Each of 4 bytes is normalized by dividing to 255.0
					   SHORT2N = 9,  // 2D signed short normalized (v[0]/32767.0,v[1]/32767.0,0,1)
					   SHORT4N = 10,  // 4D signed short normalized (v[0]/32767.0,v[1]/32767.0,v[2]/32767.0,v[3]/32767.0)
					   USHORT2N = 11,  // 2D unsigned short normalized (v[0]/65535.0,v[1]/65535.0,0,1)
					   USHORT4N = 12,  // 4D unsigned short normalized (v[0]/65535.0,v[1]/65535.0,v[2]/65535.0,v[3]/65535.0)
					   UDEC3 = 13,  // 3D unsigned 10 10 10 format expanded to (value, value, value, 1)
					   DEC3N = 14,  // 3D signed 10 10 10 format normalized and expanded to (v[0]/511.0, v[1]/511.0, v[2]/511.0, 1)
					   FLOAT16_2 = 15,  // Two 16-bit floating point values, expanded to (value, value, 0, 1)
					   FLOAT16_4 = 16,  // Four 16-bit floating point values
					   UNUSED = 17,  // When the type field in a decl is unused.
					   UINT8 = 18,
					   UINT32 = 19
	};

	enum DeclarationUsage
	{
		POSITION = 0,
		BLENDWEIGHT = 1,   // 1
		BLENDINDICES = 2,  // 2
		NORMAL = 3,        // 3
		PSIZE = 4,         // 4
		TEXCOORD = 5,      // 5
		TANGENT = 6,       // 6
		BINORMAL = 7,      // 7
		TESSFACTOR = 8,    // 8
		POSITIONT = 9,     // 9
		COLOR = 10,         // 10
		FOG = 11,           // 11
		DEPTH = 12,         // 12
		SAMPLE = 13,        // 13
		SEED = 14,
		SPEED = 15,
		RAND = 16,
		TYPE = 17,
		STATE = 18,
		OLDSTATE = 19
	};

	enum TextureFilter
	{
		// filtering disabled (valid for mip filter only)
		TEXFILTER_NONE = 0,
		// nearest
		TEXFILTER_POINT = 1,
		// linear interpolation
		TEXFILTER_LINEAR = 2,
		// anisotropic
		TEXFILTER_ANISOTROPIC = 3,
		// 4-sample tent
		TEXFILTER_PYRAMIDALQUAD = 6,
		// 4-sample gaussian
		TEXFILTER_GAUSSIANQUAD = 7,
	};

	enum ClearType
	{
		CLEAR_TARGET = (int)0x00000001l,
		CLEAR_DEPTH = (int)0x00000002l,
		CLEAR_STENCIL = (int)0x00000004l,
	};

	// State Enum
	enum RasterizerState
	{
		CW_FRONT = 0,
		CW_BACK = 1,
		NO_CULL = 2
	};

	enum DepthStencilState
	{
		DEPTH_LESS_WRITE_STENCIL_PASS = 0,
		DEPTH_LESS_STENCIL_PASS = 1
	};

	enum BlendState
	{
		BLEND_OPAQUE = 0,
		BLEND_ADDITIVE = 1,
		BLEND_TRANSPARENT = 2
	};

	enum TextureTypes
	{
		Texture_2D,
		Depth_Stencil_Texture,
		Texture_3D
	};

	enum PixelFormat
	{
		/// Unknown pixel format.
		PF_UNKNOWN = 0,
		/// 8-bit pixel format, all bits luminance.
		PF_L8 = 1,
		PF_BYTE_L = PF_L8,
		/// 16-bit pixel format, all bits luminance.
		PF_L16 = 2,
		PF_SHORT_L = PF_L16,
		/// 8-bit pixel format, all bits alpha.
		PF_A8 = 3,
		PF_BYTE_A = PF_A8,
		/// 8-bit pixel format, 4 bits alpha, 4 bits luminance.
		PF_A4L4 = 4,
		/// 2 byte pixel format, 1 byte luminance, 1 byte alpha
		PF_BYTE_LA = 5,
		/// 16-bit pixel format, 5 bits red, 6 bits green, 5 bits blue.
		PF_R5G6B5 = 6,
		/// 16-bit pixel format, 5 bits red, 6 bits green, 5 bits blue.
		PF_B5G6R5 = 7,
		/// 8-bit pixel format, 2 bits blue, 3 bits green, 3 bits red.
		PF_R3G3B2 = 31,
		/// 16-bit pixel format, 4 bits for alpha, red, green and blue.
		PF_A4R4G4B4 = 8,
		/// 16-bit pixel format, 5 bits for blue, green, red and 1 for alpha.
		PF_A1R5G5B5 = 9,
		/// 24-bit pixel format, 8 bits for red, green and blue.
		PF_R8G8B8 = 10,
		/// 24-bit pixel format, 8 bits for blue, green and red.
		PF_B8G8R8 = 11,
		/// 32-bit pixel format, 8 bits for alpha, red, green and blue.
		PF_A8R8G8B8 = 12,
		/// 32-bit pixel format, 8 bits for blue, green, red and alpha.
		PF_A8B8G8R8 = 13,
		/// 32-bit pixel format, 8 bits for blue, green, red and alpha.
		PF_B8G8R8A8 = 14,
		/// 32-bit pixel format, 8 bits for red, green, blue and alpha.
		PF_R8G8B8A8 = 28,
		/// 32-bit pixel format, 8 bits for red, 8 bits for green, 8 bits for blue
		/// like PF_A8R8G8B8, but alpha will get discarded
		PF_X8R8G8B8 = 26,
		/// 32-bit pixel format, 8 bits for blue, 8 bits for green, 8 bits for red
		/// like PF_A8B8G8R8, but alpha will get discarded
		PF_X8B8G8R8 = 27,
		/// 3 byte pixel format, 1 byte for red, 1 byte for green, 1 byte for blue
		PF_BYTE_RGB = PF_B8G8R8,
		/// 3 byte pixel format, 1 byte for blue, 1 byte for green, 1 byte for red
		PF_BYTE_BGR = PF_R8G8B8,
		/// 4 byte pixel format, 1 byte for blue, 1 byte for green, 1 byte for red and one byte for alpha
		PF_BYTE_BGRA = PF_A8R8G8B8,
		/// 4 byte pixel format, 1 byte for red, 1 byte for green, 1 byte for blue, and one byte for alpha
		PF_BYTE_RGBA = PF_A8B8G8R8,
		/// 32-bit pixel format, 2 bits for alpha, 10 bits for red, green and blue.
		PF_A2R10G10B10 = 15,
		/// 32-bit pixel format, 10 bits for blue, green and red, 2 bits for alpha.
		PF_A2B10G10R10 = 16,
		/// DDS (DirectDraw Surface) DXT1 format
		PF_DXT1 = 17,
		/// DDS (DirectDraw Surface) DXT2 format
		PF_DXT2 = 18,
		/// DDS (DirectDraw Surface) DXT3 format
		PF_DXT3 = 19,
		/// DDS (DirectDraw Surface) DXT4 format
		PF_DXT4 = 20,
		/// DDS (DirectDraw Surface) DXT5 format
		PF_DXT5 = 21,
		// 16-bit pixel format, 16 bits (float) for red
		PF_FLOAT16_R = 32,
		// 48-bit pixel format, 16 bits (float) for red, 16 bits (float) for green, 16 bits (float) for blue
		PF_FLOAT16_RGB = 22,
		// 64-bit pixel format, 16 bits (float) for red, 16 bits (float) for green, 16 bits (float) for blue, 16 bits (float) for alpha
		PF_FLOAT16_RGBA = 23,
		// 32-bit pixel format, 32 bits (float) for red
		PF_FLOAT32_R = 33,
		// 96-bit pixel format, 32 bits (float) for red, 32 bits (float) for green, 32 bits (float) for blue
		PF_FLOAT32_RGB = 24,
		// 128-bit pixel format, 32 bits (float) for red, 32 bits (float) for green, 32 bits (float) for blue, 32 bits (float) for alpha
		PF_FLOAT32_RGBA = 25,
		// 32-bit, 2-channel s10e5 floating point pixel format, 16-bit green, 16-bit red
		PF_FLOAT16_GR = 35,
		// 64-bit, 2-channel floating point pixel format, 32-bit green, 32-bit red
		PF_FLOAT32_GR = 36,
		// Depth texture format
		PF_DEPTH = 29,
		// 64-bit pixel format, 16 bits for red, green, blue and alpha
		PF_SHORT_RGBA = 30,
		// 32-bit pixel format, 16-bit green, 16-bit red
		PF_SHORT_GR = 34,
		// 48-bit pixel format, 16 bits for red, green and blue
		PF_SHORT_RGB = 37,
		/// PVRTC (PowerVR) RGB 2 bpp
		PF_PVRTC_RGB2 = 38,
		/// PVRTC (PowerVR) RGBA 2 bpp
		PF_PVRTC_RGBA2 = 39,
		/// PVRTC (PowerVR) RGB 4 bpp
		PF_PVRTC_RGB4 = 40,
		/// PVRTC (PowerVR) RGBA 4 bpp
		PF_PVRTC_RGBA4 = 41,
		/// 8-bit pixel format, all bits red.
		PF_R8 = 42,
		/// 16-bit pixel format, 8 bits red, 8 bits green.
		PF_RG8 = 43,
		// Depth 16
		PF_DEPTH16 = 44,
		// Depth 32
		PF_DEPTH32 = 45,
		// Depth 24 Stencil 8
		PF_DEPTH24S8 = 46,
		// Number of pixel formats currently defined
		PF_COUNT = 47,
	};

	const unsigned int MIRROR_BACK_BUFFER = UINT32_MAX;
	const unsigned int MIRROR_BACK_BUFFER_HALF = UINT32_MAX - 1;
	const unsigned int MIRROR_BACK_BUFFER_QUARTER = UINT32_MAX - 2;
	const unsigned int MIRROR_BACK_BUFFER_EIGTH = UINT32_MAX - 3;
}

#endif