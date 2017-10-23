#include "LeblancEngine/Render/Utility/LeblancDeviceManager.h"

DeviceManager::DeviceManager()
{

}

DeviceManager::~DeviceManager()
{
	release();
}

void DeviceManager::initialize()
{

}

void DeviceManager::release()
{
	m_device.release();
}
