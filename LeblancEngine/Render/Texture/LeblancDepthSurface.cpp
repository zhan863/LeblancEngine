#include "LeblancEngine/Render/Texture/LeblancDepthSurface.h"

namespace Leblanc
{
	IDepthSurface::IDepthSurface(IDevice* device) : m_device(device)
	{

	}

	IDepthSurface::~IDepthSurface()
	{
		release();
	}

	void IDepthSurface::release()
	{

	}
}