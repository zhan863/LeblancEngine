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
	if (!m_device.initialized())
	{
		m_device.initialize(window);
	}
}

void DeviceManager::release()
{
	m_device.release();
}

void DeviceManager::present()
{
	m_device.present();
}