#include "LeblancEngine/Render/Material/D3D11/LeblancConstantBufferD3D11.h"
#include "LeblancEngine/BasicInclude/LeblancMemoryOperation.h"
#include "LeblancEngine/Render/Device/D3D11/LeblancDeviceD3D11.h"
#include "LeblancEngine/Render/Device/D3D11/LeblancDeviceContextD3D11.h"
#include "LeblancEngine/Render/Defines/LeblancRenderBasicDefine.h"

namespace Leblanc
{
	EffectConstantBufferD3D11::EffectConstantBufferD3D11(IDevice* device, IDeviceContext* device_context) :
		EffectConstantBuffer(device, device_context)
	{

	}

	EffectConstantBufferD3D11::~EffectConstantBufferD3D11()
	{
		release();
	}

	void EffectConstantBufferD3D11::initialize(ID3DX11Effect* effect, int index)
	{
		if (effect)
		{
			m_buffer_handle = effect->GetConstantBufferByIndex(index);
			m_buffer_handle->GetDesc(&m_desc);
			m_name = m_desc.Name;
		}
	}

	void EffectConstantBufferD3D11::release()
	{
	}

	void ConstantBufferD3D11::release()
	{ 
		safe_Release(m_d3d11_buffer); 
	}

	void ConstantBufferD3D11::initialize()
	{
		DeviceD3D11* device_d3d11 = dynamic_cast<DeviceD3D11*>(m_device);
		if (device_d3d11)
		{
			D3D11_BUFFER_DESC cb_desc;
			cb_desc.ByteWidth = m_buffer_size;
			cb_desc.Usage = D3D11_USAGE_DYNAMIC;
			cb_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			cb_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			cb_desc.MiscFlags = 0;
			cb_desc.StructureByteStride = 0;

			device_d3d11->createBuffer(&cb_desc, nullptr, &m_d3d11_buffer);
		}
	}

	void ConstantBufferD3D11::update(void* data)
	{
		if (m_d3d11_buffer)
		{
			DeviceContextD3D11* device_context_d3d11 = dynamic_cast<DeviceContextD3D11*>(m_device_context);
			if (device_context_d3d11 && m_slot_index != -1)
			{
				ID3D11DeviceContext* device_context = (ID3D11DeviceContext*)(device_context_d3d11->getHandle());
				if (device_context)
				{
					D3D11_MAPPED_SUBRESOURCE mapped_data;
					device_context->Map(m_d3d11_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_data);
					memcpy(mapped_data.pData, &data, m_buffer_size);
					device_context->Unmap(m_d3d11_buffer, 0);
				}

				device_context_d3d11->setConstantBuffer<SF_Vertex>(m_slot_index, m_d3d11_buffer);
				device_context_d3d11->setConstantBuffer<SF_Pixel>(m_slot_index, m_d3d11_buffer);
			}
		}
	}
}