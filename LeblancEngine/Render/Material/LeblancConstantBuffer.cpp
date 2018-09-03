#include "LeblancEngine/Render/Material/LeblancConstantBuffer.h"

namespace Leblanc
{
	EffectConstantBuffer::EffectConstantBuffer(IDevice* device, IDeviceContext* device_context) :
		m_device(device), m_device_context(device_context)
	{

	}

	EffectConstantBuffer::~EffectConstantBuffer()
	{
		release();
	}

	void EffectConstantBuffer::release()
	{
	}
}