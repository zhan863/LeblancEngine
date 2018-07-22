#ifndef LEBLANC_DEVICE_MANAGER_H
#define LEBLANC_DEVICE_MANAGER_H
#include "LeblancEngine/Render/Include/LeblancDirectInclude.h"
#include "LeblancEngine/Render/Window/LeblancWindow.h"
#include <vector>

namespace Leblanc
{
	class IDevice;
	class IDeviceContext;

	using namespace std;

	class DeviceManager
	{
	public:
		DeviceManager();

		~DeviceManager();

		void initialize(Window& window);

		IDevice* getDevice() { return m_device; }

		IDeviceContext* getImmediateContext() { return m_immediate_device_context; }

		void registerImmediateContext(IDeviceContext* context);

	private:
		void release();

		// data
		IDevice* m_device = nullptr;
		IDeviceContext* m_immediate_device_context = nullptr;
		vector<IDeviceContext*> m_deferred_device_contexts;
	};
}

#endif