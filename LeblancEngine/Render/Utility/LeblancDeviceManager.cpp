#include "LeblancEngine/Render/Utility/LeblancDeviceManager.h"
#include "LeblancEngine/BasicInclude/LeblancMemoryOperation.h"

#include "LeblancEngine/Render/Utility/LeblancDeviceD3D11.h"
#include "LeblancEngine/Render/Utility/LeblancDeviceContextD3D11.h"

DeviceManager::DeviceManager()
{

}

DeviceManager::~DeviceManager()
{
	release();
}

void DeviceManager::initialize(Window& window)
{
	release();

	m_device = new DeviceD3D11;
	m_device->initialize(window);
}

void DeviceManager::release()
{
	safe_delete(m_device);
	safe_delete(m_immediate_device_context);

	for (int i = 0; i < m_deferred_device_contexts.size(); i++)
	{
		safe_delete(m_deferred_device_contexts[i]);
	}
	m_deferred_device_contexts.clear();
}

void DeviceManager::registerImmediateContext(DeviceContextD3D11* context)
{
	if (context && !m_immediate_device_context)
	{
		m_immediate_device_context = context;
	}
	else
	{
		// LOG ERROR!
	}
}