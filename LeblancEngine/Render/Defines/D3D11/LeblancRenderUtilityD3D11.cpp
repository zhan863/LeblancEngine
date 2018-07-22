#include "LeblancEngine/Render/Defines/D3D11/LeblancRenderUtilityD3D11.h"
#include "LeblancEngine/Render/Defines/LeblancRenderBasicDefine.h"

namespace Leblanc
{
	static std::string k_position_semantic = "POSITION";
	static std::string k_normal_semantic = "NORMAL";
	static std::string k_texcoord_semantic = "TEXCOORD";
	static std::string k_tangent_semantic = "TANGENT";
	static std::string k_binormal_semantic = "BINORMAL";
	static std::string k_bones_semantic = "BLENDINDICES";
	static std::string k_weights_semantic = "BLENDWEIGHT";
	static std::string k_color_semantic = "COLOR";
	static std::string k_unknow_semantic = "UNKNOWN";

	const std::string& delcarationUsageToString(BYTE byte_usage)
	{
		DeclarationUsage usage = (DeclarationUsage)byte_usage;
		if (usage == DeclarationUsage::POSITION)
		{
			return k_position_semantic;
		}
		if (usage == DeclarationUsage::NORMAL)
		{
			return k_normal_semantic;
		}
		if (usage == DeclarationUsage::TEXCOORD)
		{
			return k_texcoord_semantic;
		}
		if (usage == DeclarationUsage::TANGENT)
		{
			return k_tangent_semantic;
		}
		if (usage == DeclarationUsage::BINORMAL)
		{
			return k_binormal_semantic;
		}
		if (usage == DeclarationUsage::BLENDINDICES)
		{
			return k_bones_semantic;
		}
		if (usage == DeclarationUsage::BLENDWEIGHT)
		{
			return k_weights_semantic;
		}
		if (usage == DeclarationUsage::COLOR)
		{
			return k_color_semantic;
		}
		return k_unknow_semantic;
	}

	DXGI_FORMAT declarationTypeToFormat(BYTE type)
	{
		switch (type)
		{
		case DeclarationType::FLOAT4:
			return DXGI_FORMAT_R32G32B32A32_FLOAT;
		case DeclarationType::FLOAT3:
			return DXGI_FORMAT_R32G32B32_FLOAT;
		case DeclarationType::FLOAT2:
			return DXGI_FORMAT_R32G32_FLOAT;
		case DeclarationType::FLOAT1:
			return DXGI_FORMAT_R32_FLOAT;
		case DeclarationType::UBYTE4:
			return DXGI_FORMAT_R8G8B8A8_UNORM;
		case DeclarationType::UINT8:
			return DXGI_FORMAT_R8_UINT;
		case DeclarationType::UINT32:
			return DXGI_FORMAT_R32_UINT;
		default:
			break;
		};
		return DXGI_FORMAT_UNKNOWN;
	}

	DXGI_FORMAT findFormat(PixelFormat format)
	{
		switch (format)
		{
		case PF_L8:
			return DXGI_FORMAT_R8_UNORM;
		case PF_L16:
			return DXGI_FORMAT_R16_UNORM;
		case PF_A8:
			return DXGI_FORMAT_UNKNOWN;
		case PF_A4L4:
			return DXGI_FORMAT_UNKNOWN;
		case PF_BYTE_LA:
			return DXGI_FORMAT_UNKNOWN;
		case PF_R3G3B2:
			return DXGI_FORMAT_UNKNOWN;
		case PF_A1R5G5B5:
			return DXGI_FORMAT_UNKNOWN;
		case PF_R5G6B5:
			return DXGI_FORMAT_UNKNOWN;
		case PF_A4R4G4B4:
			return DXGI_FORMAT_UNKNOWN;
		case PF_R8G8B8:
			return DXGI_FORMAT_UNKNOWN;
		case PF_A8R8G8B8:
			return DXGI_FORMAT_R8G8B8A8_UNORM;
		case PF_A8B8G8R8:
			return DXGI_FORMAT_B8G8R8A8_UNORM;
		case PF_X8R8G8B8:
			return DXGI_FORMAT_UNKNOWN;
		case PF_X8B8G8R8:
			return DXGI_FORMAT_UNKNOWN;
		case PF_A2B10G10R10:
			return DXGI_FORMAT_R10G10B10A2_TYPELESS;
		case PF_A2R10G10B10:
			return DXGI_FORMAT_UNKNOWN;
		case PF_FLOAT16_R:
			return DXGI_FORMAT_R16_FLOAT;
		case PF_FLOAT16_RGBA:
			return DXGI_FORMAT_R16G16B16A16_FLOAT;
		case PF_FLOAT32_R:
			return DXGI_FORMAT_R32_FLOAT;
		case PF_FLOAT32_RGB:
			return DXGI_FORMAT_R32G32B32_FLOAT;
		case PF_FLOAT32_RGBA:
			return DXGI_FORMAT_R32G32B32A32_FLOAT;
		case PF_SHORT_RGBA:
			return DXGI_FORMAT_R16G16B16A16_UNORM;
		case PF_DXT1:
			return DXGI_FORMAT_BC1_UNORM;
		case PF_DXT2:
			return DXGI_FORMAT_BC2_UNORM;
		case PF_DXT3:
			return DXGI_FORMAT_BC2_UNORM;
		case PF_DXT4:
			return DXGI_FORMAT_BC3_UNORM;
		case PF_DXT5:
			return DXGI_FORMAT_BC3_UNORM;
		case PF_DEPTH16:
			return DXGI_FORMAT_R16_TYPELESS;
		case PF_DEPTH32:
			return DXGI_FORMAT_R32_TYPELESS;
		case PF_DEPTH24S8:
			return  DXGI_FORMAT_R24G8_TYPELESS;
		case PF_UNKNOWN:
		default:
			return DXGI_FORMAT_UNKNOWN;
		}
	}
}