#include "LeblancEngine/Render/Buffer/D3D11/LeblancBufferD3D11.h"
#include "LeblancEngine/Render/Device/LeblancDeviceContext.h"
#include "LeblancEngine/Render/Device/D3D11/LeblancDeviceD3D11.h"
#include "LeblancEngine/Render/Defines/D3D11/LeblancRenderUtilityD3D11.h"
#include "LeblancEngine/BasicInclude/LeblancMemoryOperation.h"
// Index buffer

namespace Leblanc
{
	void IndexBufferD3D11::release()
	{
		if (m_index_buffer)
		{
			m_index_buffer->Release();
		}
	}

	void IndexBufferD3D11::bind() const
	{
		ID3D11DeviceContext* device_context = (ID3D11DeviceContext*)m_device_context->getHandle();
		device_context->IASetIndexBuffer(m_index_buffer, DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
	}

	void IndexBufferD3D11::initialize(const IndexBufferDeclaration* declaration)
	{
		if (declaration)
		{
			// Fill in a buffer description.
			D3D11_BUFFER_DESC buffer_desc;
			buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
			buffer_desc.ByteWidth = declaration->getIndexCount() * sizeof(unsigned int);
			buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			buffer_desc.MiscFlags = 0;

			// Create the buffer with the device.
			DeviceD3D11* device_d3d11 = dynamic_cast<DeviceD3D11*>(m_device);
			device_d3d11->createBuffer(&buffer_desc, 0, &m_index_buffer);
		}
	}

	void* IndexBufferD3D11::lock()
	{
		ID3D11DeviceContext* device_context = (ID3D11DeviceContext*)m_device_context->getHandle();
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
		ID3D11DeviceContext* device_context = (ID3D11DeviceContext*)m_device_context->getHandle();
		if (device_context)
		{
			D3D11_MAPPED_SUBRESOURCE mapped_resource;

			device_context->Unmap(m_index_buffer, 0);
		}
	}

	void VertexBufferD3D11::release()
	{
		safe_Release(m_vertex_buffer);
	}

	void VertexBufferD3D11::bind() const
	{
		ID3D11DeviceContext* device_context = (ID3D11DeviceContext*)m_device_context->getHandle();
		if (device_context)
		{
			UINT offset = 0;
			device_context->IASetVertexBuffers(0, 1, &m_vertex_buffer, &m_stride, &offset);
		}
	}

	bool VertexBufferD3D11::initialize(const VertexBufferDeclaration* data)
	{
		if (data)
		{
			m_size_in_bytes = data->sizeInBytes();
			m_stride = data->vertexStride();

			D3D11_BUFFER_DESC bd;
			memset(&bd, 0, sizeof(D3D11_BUFFER_DESC));
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = m_size_in_bytes;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.MiscFlags = 0;
			bd.CPUAccessFlags = 0;
			bd.StructureByteStride = sizeof(float);

			D3D11_SUBRESOURCE_DATA resource;
			memset(&resource, 0, sizeof(D3D11_SUBRESOURCE_DATA));
			resource.pSysMem = data->vertexPtr();
			resource.SysMemPitch = data->vertexStride();

			if (m_device)
			{
				ID3D11Device* d3d11_device = (ID3D11Device*)m_device->getHandle();
				if (d3d11_device)
				{
					if (SUCCEEDED(d3d11_device->CreateBuffer(&bd, &resource, &m_vertex_buffer)))
						return true;
				}
			}
			return false;
		}
	}

	void VertexDeclarationD3D11::release()
	{
		// data
		m_input_layout_declaration.clear();

		safe_delete_array(m_input_layout);
	}

	unsigned int VertexDeclarationD3D11::vertexStride() const
	{
		return m_vertex_stride;
	}

	void VertexBufferD3D11::setVertexDeclaration(const IVertexDeclaration* decl)
	{ 
		m_vertex_declaration = dynamic_cast<const VertexDeclarationD3D11*>(decl); 
	}

	const IVertexDeclaration*  VertexBufferD3D11::vertexDeclaration()
	{ 
		return m_vertex_declaration;
	}

	void VertexDeclarationD3D11::initialize(const VertexLayoutDeclaration* declaration)
	{
		const std::vector<VertexElement>& elements = declaration->elements();
		if (elements.size() == 0)
			return;

		m_input_layout = new D3D11_INPUT_ELEMENT_DESC[elements.size()];

		for (int i = 0; i < elements.size(); i++)
		{
			m_input_layout_declaration.push_back(elements[i]);
			D3D11_INPUT_ELEMENT_DESC element =
			{ delcarationUsageToString(elements[i].usage()).c_str(),
				elements[i].usageIndex(),
				declarationTypeToFormat(elements[i].type()),
				elements[i].streamIndex(),
				elements[i].offset(),
				D3D11_INPUT_PER_VERTEX_DATA,
				0 };

			m_input_layout[i] = element;
		}

		VertexElement element = elements[elements.size() - 1];
		m_vertex_stride = (element.offset() + VertexDeclarationD3D11::elementToSize(element.type()));
		m_element_count = (unsigned int)(elements.size());
	}
}

