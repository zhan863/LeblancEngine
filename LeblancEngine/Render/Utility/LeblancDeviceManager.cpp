#include "LeblancEngine/Render/Utility/LeblancDeviceManager.h"

DeviceManager::DeviceManager()
{

}

DeviceManager::~DeviceManager()
{
	release();
}

void DeviceManager::initialize(HWND window)
{
	if (!m_device)
	{
		m_device = new DeviceD3D11();
		m_device->initialize(window);
	}
}

void DeviceManager::release()
{
	m_device->release();
}
