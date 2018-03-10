#include "LeblancEngine/Render/Utility/LeblancDeviceManager.h"
#include "LeblancEngine/BasicInclude/LeblancMemoryOperation.h"

DeviceManager::DeviceManager()
{

}

DeviceManager::~DeviceManager()
{
	release();
}

void DeviceManager::initialize(Window& window)
{
	if (!m_device)
	{
		m_device = new DeviceD3D11;
	}

	if (!m_device->initialized())
	{
		m_device->initialize(window);
	}
}

void DeviceManager::release()
{
	safe_delete(m_device);
}

void DeviceManager::present()
{
	m_device->present();
}

ID3D11RenderTargetView* DeviceManager::getBackBufferView()
{
	return m_device->getBackBufferView();
}