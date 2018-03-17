#ifndef LEBLANC_RENDER_PLATFORM_UTILITY_DEFINE_D3D11_H
#define LEBLANC_RENDER_PLATFORM_UTILITY_DEFINE_D3D11_H

#include "LeblancEngine/BasicInclude/LeblancPCH.h"

const std::string& delcarationUsageToString(BYTE usage);

DXGI_FORMAT declarationTypeToFormat(BYTE type);
#endif