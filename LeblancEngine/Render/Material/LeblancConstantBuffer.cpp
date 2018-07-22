#include "LeblancEngine/Render/Material/LeblancConstantBuffer.h"

namespace Leblanc
{
	ConstantBuffer::ConstantBuffer(IDevice* device, IDeviceContext* device_context) :
		m_device(device), m_device_context(device_context)
	{

	}

	ConstantBuffer::~ConstantBuffer()
	{
		release();
	}

	void ConstantBuffer::release()
	{
	}
}