#ifndef LEBLANC_DEVICE_MANAGER_H
#define LEBLANC_DEVICE_MANAGER_H
#include "LeblancEngine/Render/Include/LeblancDirectInclude.h"
#include "LeblancEngine/Render/Utility/LeblancDeviceD3D11.h"
#include "LeblancEngine/Render/Basics/LeblancWindow.h"
class DeviceManager
{
public:
	DeviceManager();

	~DeviceManager();
	
	void initialize(Window& window);

	DeviceD3D11* getCurrentDevice() { return m_device; }

	void present();

	ID3D11RenderTargetView* getBackBufferView();

private:
	void release();

	// data
	DeviceD3D11* m_device;
};


#endif