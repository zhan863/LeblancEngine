#include "LeblancEngine/Render/Basics/LeblancRenderPlatformDefineD3D11.h"
#include "LeblancEngine/Render/Utility/LeblancDeviceD3D11.h"

void IndexBufferD3D11::release()
{
	if (m_index_buffer)
	{
		m_index_buffer->Release();
	}
}

void IndexBufferD3D11::initialize(const IndexBufferDeclaration* declaration)
{
	if (declaration)
	{
		// Fill in a buffer description.
		D3D11_BUFFER_DESC buffer_desc;
		buffer_desc.Usage = D3D11_USAGE_DEFAULT;
		buffer_desc.ByteWidth = declaration->getIndexCount() * sizeof(uint32_t);
		buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		buffer_desc.CPUAccessFlags = 0;
		buffer_desc.MiscFlags = 0;

		// Create the buffer with the device.
		m_device->createBuffer(&buffer_desc, 0, &m_index_buffer);
	}
}

void* IndexBufferD3D11::lock()
{
	ID3D11DeviceContext* device_context = m_device->getImmediateDeviceContext();
	if (device_context)
	{
		D3D11_MAPPED_SUBRESOURCE mapped_resource;

		if (SUCCEEDED(device_context->Map(m_index_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource)))
		{
			return mapped_resource.pData;
		}
	}

	return nullptr;
}

void IndexBufferD3D11::unlock()
{
}