#ifndef LEBLANC_DEVICE_MANAGER_H
#define LEBLANC_DEVICE_MANAGER_H
#include "LeblancEngine/Render/Include/LeblancDirectInclude.h"
#include "LeblancEngine/Render/Basics/LeblancWindow.h"
#include <vector>

class DeviceD3D11;
class DeviceContextD3D11;

using namespace std;

class DeviceManager
{
public:
	DeviceManager();

	~DeviceManager();
	
	void initialize(Window& window);

	DeviceD3D11* getDevice() { return m_device; }

	void registerImmediateContext(DeviceContextD3D11* context);

	DeviceContextD3D11* getImmediateContext() { return m_immediate_device_context; }

private:
	void release();

	// data
	DeviceD3D11* m_device = nullptr;
	DeviceContextD3D11* m_immediate_device_context = nullptr;
	vector<DeviceContextD3D11*> m_deferred_device_contexts;
};


#endif