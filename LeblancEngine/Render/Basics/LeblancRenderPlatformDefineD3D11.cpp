#include "LeblancEngine/Render/Basics/LeblancRenderPlatformDefineD3D11.h"
#include "LeblancEngine/Render/Basics/LeblancRenderPlatformUtilityDefineD3D11.h"
#include "LeblancEngine/Render/Basics/LeblancRenderBasicDefine.h"
#include "LeblancEngine/Render/Utility/LeblancDeviceD3D11.h"
#include "LeblancEngine/BasicInclude/LeblancMemoryOperation.h"

// Declaration data
VertexLayoutDeclaration::VertexLayoutDeclaration(const std::vector<VertexElement>& vertex_elements)
{
	for (uint32_t i = 0; i < vertex_elements.size(); i++)
	{
		m_elements.push_back(VertexElement(vertex_elements[i]));
	}
}

VertexLayoutDeclaration::VertexLayoutDeclaration(const VertexLayoutDeclaration& in)
{
	for (uint32_t i = 0; i < in.m_elements.size(); i++)
	{
		m_elements.push_back(VertexElement(in.m_elements[i]));
	}
};

VertexLayoutDeclaration::~VertexLayoutDeclaration()
{
	m_elements.clear();
}

const std::vector<VertexElement>& VertexLayoutDeclaration::elements() const
{
	return m_elements;
}

// D3D11 data
// Index buffer
void IndexBufferD3D11::release()
{
	if (m_index_buffer)
	{
		m_index_buffer->Release();
	}
}

void IndexBufferD3D11::bind() const
{
	ID3D11DeviceContext* device_context = m_device->getImmediateDeviceContext();
	device_context->IASetIndexBuffer(m_index_buffer, DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
}

void IndexBufferD3D11::initialize(const IndexBufferDeclaration* declaration)
{
	if (declaration)
	{
		// Fill in a buffer description.
		D3D11_BUFFER_DESC buffer_desc;
		buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
		buffer_desc.ByteWidth = declaration->getIndexCount() * sizeof(uint32_t);
		buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
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
	ID3D11DeviceContext* device_context = m_device->getImmediateDeviceContext();
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
	if (m_device)
	{
		ID3D11DeviceContext* device_context = m_device->getImmediateDeviceContext();
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
			ID3D11Device* d3d11_device = m_device->getD3D11Device();
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

uint32_t VertexDeclarationD3D11::elementToSize(const BYTE& type)
{
	switch (type)
	{
	case (UINT)DeclarationType::FLOAT1:
		return 1 * sizeof(float);
	case (UINT)DeclarationType::FLOAT2:
		return 2 * sizeof(float);
	case (UINT)DeclarationType::FLOAT3:
		return 3 * sizeof(float);
	case (UINT)DeclarationType::FLOAT4:
		return 4 * sizeof(float);
	case (UINT)DeclarationType::UBYTE4:
		return 4 * sizeof(uint8_t);
	}
	return 0;
}

uint32_t VertexDeclarationD3D11::vertexStride() const
{
	return m_vertex_stride;
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
	m_element_count = (uint32_t)(elements.size());
}

void RasterizerStateD3D11::release()
{
	safe_Release(m_rasterizer_state);
}

void RasterizerStateD3D11::initialize(RasterizerState rasterizer_state)
{
	release();

	D3D11_RASTERIZER_DESC rasterizer_desc;
	rasterizer_desc.CullMode = rasterizer_state == RasterizerState::NONE ? D3D11_CULL_NONE :
		(rasterizer_state == RasterizerState::CW_FRONT ? D3D11_CULL_FRONT : D3D11_CULL_BACK);
	rasterizer_desc.FillMode = D3D11_FILL_SOLID;
	rasterizer_desc.ScissorEnable = false;
	
	m_device->getD3D11Device()->CreateRasterizerState(&rasterizer_desc, &m_rasterizer_state);
}

// Declaration
// Vertex Element
VertexElement::VertexElement()
{

}

VertexElement::VertexElement(WORD offsetVal,
	BYTE typeVal,
	BYTE usageVal,
	BYTE usageIndexVal,
	BYTE streamIndexVal)
{
	m_offset = offsetVal;
	m_type = typeVal;
	m_usage = usageVal;
	m_usageIndex = usageIndexVal;
	m_streamIndex = streamIndexVal;
}

VertexElement::VertexElement(const VertexElement& element)
{
	m_offset = element.m_offset;
	m_type = element.m_type;
	m_usageIndex = element.m_usageIndex;
	m_usage = element.m_usage;
	m_streamIndex = element.m_streamIndex;
}

WORD VertexElement::offset() const
{
	return m_offset;
}

BYTE VertexElement::type() const
{
	return m_type;
}

BYTE VertexElement::usage() const
{
	return m_usage;
}

BYTE VertexElement::usageIndex() const
{
	return m_usageIndex;
}

BYTE VertexElement::streamIndex() const
{
	return m_streamIndex;
}

bool VertexElement::operator == (const VertexElement &src) const
{
	if (m_usageIndex != src.usageIndex())
		return false;
	if (m_usage != src.usage())
		return false;
	if (m_type != src.type())
		return false;
	if (m_offset != src.offset())
		return false;

	return true;
}

bool VertexElement::operator != (const VertexElement &src) const
{
	if (m_usageIndex != src.usageIndex())
		return true;
	if (m_usage != src.usage())
		return true;
	if (m_type != src.type())
		return true;
	if (m_offset != src.offset())
		return true;

	return false;
}

// VertexStream
VertexStream::VertexStream(DeclarationUsage usage,
	uint32_t usage_index,
	uint32_t stride_arg,
	uint32_t count_arg,
	const float* stream_src) :
	m_usage(usage),
	m_usage_index(usage_index),
	m_stream(nullptr),
	m_count(count_arg),
	m_stride(stride_arg)
{
	setStream(m_stride, m_count, stream_src);
}

VertexStream::~VertexStream()
{
	m_stride = 0;
	m_count = 0;
	
	safe_delete_array(m_stream);
}

uint32_t VertexStream::id(const VertexElement& element)
{
	uint32_t id = (1 << (element.usage() + 16));
	if (element.usageIndex() >= 0)
	{
		id |= (1 << (element.usage() + 1));
	}

	return id;
}

uint32_t VertexStream::id(const VertexStream& vertex_stream)
{
	uint32_t id = (1 << ((BYTE)vertex_stream.usage() + 16));

	if (vertex_stream.usage_index() >= 0)
	{
		id |= (1 << ((vertex_stream.usage_index() + 1)));
	}

	return id;
}

void VertexStream::setStream(uint32_t stride_arg,
	uint32_t count_arg,
	const float* stream_src)
{
	m_stride = stride_arg;
	m_count = count_arg;

	if (size() > 0)
	{
		safe_delete_array(m_stream);
		m_stream = new float[size()];
		setStream(stream_src);
	}
}

void VertexStream::setStream(const float* stream_src)
{
	if (m_stream)
	{
		memcpy(m_stream, stream_src, bufferSizeInBytes());
	}
}

InputLayoutCacheD3D11::InputLayoutCacheD3D11(DeviceD3D11* device) :
	m_device(device)
{
}

InputLayoutCacheD3D11::~InputLayoutCacheD3D11()
{
	release();
}

void InputLayoutCacheD3D11::release()
{
	for (auto it = m_layouts.begin(); it != m_layouts.end(); it++)
	{
		ID3D11InputLayout* layout = it->second;
		safe_Release(layout);
	}

	m_layouts.clear();
}

void InputLayoutCacheD3D11::initialize(ID3DX11EffectTechnique* technique, uint32_t pass_index)
{
	m_technique = technique;
	m_pass_index = pass_index;

	if (ID3DX11EffectPass* pass = m_technique->GetPassByIndex(pass_index))
	{
		pass->GetDesc(&m_pass_desc);
	}
}

bool InputLayoutCacheD3D11::bindLayout(const VertexDeclarationD3D11* vertex_declaration) const
{
	if (!vertex_declaration)
	{
		//LOG("vertex declaration null");
		return false;
	}

	auto it = m_layouts.find(vertex_declaration);

	if (it == m_layouts.end())
	{
		ID3D11InputLayout* inputLayout = 0;
		uint32_t elementCount = vertex_declaration->getElementCount();
		const D3D11_INPUT_ELEMENT_DESC* input_desc = vertex_declaration->getInputLayout();

		ID3D11Device* device = m_device->getD3D11Device();
		if (device)
		{
			if (FAILED(device->CreateInputLayout(input_desc,
				elementCount,
				m_pass_desc.pIAInputSignature,
				m_pass_desc.IAInputSignatureSize,
				&inputLayout)))
			{
				//LOG("Failed to setup input layout");
				return false;
			}
		}
		else
		{
			//LOG("Failed to find device");
			return false;
		}

		m_layouts.insert(std::make_pair(vertex_declaration, inputLayout));
		it = m_layouts.find(vertex_declaration);
	}

	if (ID3D11InputLayout* layout = it->second)
	{
		ID3D11DeviceContext* device_context = m_device->getImmediateDeviceContext();
		if(device_context)
			device_context->IASetInputLayout(layout);
		return true;
	}
	return false;
}