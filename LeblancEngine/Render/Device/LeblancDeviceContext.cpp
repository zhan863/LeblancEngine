#include "LeblancEngine/Render/Device/LeblancDeviceContext.h"
#include "LeblancEngine/Render/Device/LeblancDevice.h"

namespace Leblanc
{
	IDeviceContext::IDeviceContext(IDevice* device) : m_device(device)
	{

	}

	IDeviceContext::~IDeviceContext()
	{
		release();
	}

	void IDeviceContext::release()
	{
	}
}