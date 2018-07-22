#include "LeblancEngine/Render/Device/LeblancDevice.h"

namespace Leblanc
{
	IDevice::IDevice()
	{

	}

	IDevice::~IDevice()
	{
		release();
	}

	void IDevice::release()
	{

	}
}