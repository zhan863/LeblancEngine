#include "LeblancEngine/Render/Basics/LeblancRenderPlatformDefineD3D11.h"

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