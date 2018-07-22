#include "LeblancEngine/Render/Material/D3D11/LeblancConstantBufferD3D11.h"
#include "LeblancEngine/BasicInclude/LeblancMemoryOperation.h"

namespace Leblanc
{
	ConstantBufferD3D11::ConstantBufferD3D11(IDevice* device, IDeviceContext* device_context) :
		ConstantBuffer(device, device_context)
	{

	}

	ConstantBufferD3D11::~ConstantBufferD3D11()
	{
		release();
	}

	void ConstantBufferD3D11::initialize(ID3DX11Effect* effect, int index)
	{
		if (effect)
		{
			m_buffer_handle = effect->GetConstantBufferByIndex(index);
			m_buffer_handle->GetDesc(&m_desc);
			m_name = m_desc.Name;
		}
	}

	void ConstantBufferD3D11::release()
	{
		safe_Release(m_buffer_handle);
	}
}