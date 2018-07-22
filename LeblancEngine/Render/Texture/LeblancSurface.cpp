#include "LeblancEngine/Render/Texture/LeblancSurface.h"

namespace Leblanc
{
	ISurface::ISurface(IDevice* device) : m_device(device)
	{

	}

	ISurface::~ISurface()
	{
		release();
	}

	void ISurface::release()
	{

	}
}