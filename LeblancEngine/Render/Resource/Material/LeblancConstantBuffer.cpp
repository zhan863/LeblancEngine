#include "LeblancEngine/Render/Resource/Material/LeblancConstantBuffer.h"
#include "LeblancEngine/Render/Utility/LeblancDeviceD3D11.h"

#include "LeblancEngine/BasicInclude/LeblancMemoryOperation.h"

ConstantBuffer::ConstantBuffer(DeviceD3D11* device) : m_device(device)
{

}

ConstantBuffer::~ConstantBuffer()
{
	release();
}

void ConstantBuffer::initialize(ID3DX11Effect* effect, int index)
{
	if (effect)
	{
		m_buffer_handle = effect->GetConstantBufferByIndex(index);
		m_buffer_handle->GetDesc(&m_desc);
		m_name = m_desc.Name;
	}
}

void ConstantBuffer::release()
{
	safe_Release(m_buffer_handle);
}