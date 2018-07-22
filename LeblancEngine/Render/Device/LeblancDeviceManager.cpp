#include "LeblancEngine/BasicInclude/LeblancMemoryOperation.h"

#include "LeblancEngine/Render/Device/LeblancDeviceManager.h"
#include "LeblancEngine/Render/Device/LeblancDevice.h"
#include "LeblancEngine/Render/Device/LeblancDeviceContext.h"

#include "LeblancEngine/Render/Device/D3D11/LeblancDeviceD3D11.h"

namespace Leblanc
{
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
		m_device->initialize(&window);
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

	void DeviceManager::registerImmediateContext(IDeviceContext* context)
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
}