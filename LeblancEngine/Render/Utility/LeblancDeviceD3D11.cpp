#include "LeblancEngine/Render/Utility/LeblancDeviceD3D11.h"

DeviceD3D11::DeviceD3D11()
{

}

DeviceD3D11::~DeviceD3D11()
{
	release();
}

void DeviceD3D11::initialize()
{

}

ID3D11Resource* DeviceD3D11::createTexture()
{
	return nullptr;
}

void DeviceD3D11::release()
{
	if (m_device_context)
		m_device_context->Release();

	if (m_device)
		m_device->Release();
}
