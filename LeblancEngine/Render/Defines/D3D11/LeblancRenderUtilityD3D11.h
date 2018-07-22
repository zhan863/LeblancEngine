#ifndef LEBLANC_RENDER_UTILITY_D3D11_H
#define LEBLANC_RENDER_UTILITY_D3D11_H

#include "LeblancEngine/BasicInclude/LeblancPCH.h"
#include "LeblancEngine/Render/Defines/LeblancRenderBasicDefine.h"
namespace Leblanc
{
	const std::string& delcarationUsageToString(BYTE usage);

	DXGI_FORMAT declarationTypeToFormat(BYTE type);

	DXGI_FORMAT findFormat(PixelFormat format);
}
#endif