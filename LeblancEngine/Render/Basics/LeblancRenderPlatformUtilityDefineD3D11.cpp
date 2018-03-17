#include "LeblancEngine/Render/Basics/LeblancRenderPlatformUtilityDefineD3D11.h"
#include "LeblancEngine/Render/Basics/LeblancRenderBasicDefine.h"

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