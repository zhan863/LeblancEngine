#ifndef LEBLANC_DEVICE_MANAGER_H
#define LEBLANC_DEVICE_MANAGER_H

#include "LeblancEngine/Render/Utility/LeblancDeviceD3D11.h"
class DeviceManager
{
public:
	DeviceManager();

	~DeviceManager();
	
	void initialize();
	
	void release();

	DeviceD3D11* getCurrentDevice() { return m_device; }

private:
	DeviceD3D11* m_device;
};


#endif