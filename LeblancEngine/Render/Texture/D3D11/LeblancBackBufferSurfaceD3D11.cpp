#include "LeblancEngine/Render/Texture/D3D11/LeblancBackBufferSurfaceD3D11.h"
#include "LeblancEngine/BasicInclude/LeblancMemoryOperation.h"
#include "LeblancEngine/Render/Device/D3D11/LeblancDeviceD3D11.h"
#include "LeblancEngine/Render/Include/LeblancDirectInclude.h"

namespace Leblanc
{
	BackBufferSurfaceD3D11::BackBufferSurfaceD3D11(IDevice* device, IDXGISwapChain* swap_chain) : SurfaceD3D11(device), m_swap_chain(swap_chain)
	{

	}

	BackBufferSurfaceD3D11::~BackBufferSurfaceD3D11()
	{
		release();
	}

	void BackBufferSurfaceD3D11::release()
	{
	}

	bool BackBufferSurfaceD3D11::create()
	{
		if (m_swap_chain)
		{
			m_swap_chain->GetBuffer(0, __uuidof(*m_resource), (LPVOID*)&m_resource);
			m_resource->GetDesc(&m_texture_desc);

			DeviceD3D11* device_d3d11 = dynamic_cast<DeviceD3D11*>(m_device);
			if (device_d3d11)
			{
				ID3D11Device* device_ptr = (ID3D11Device*)device_d3d11->device();
				device_ptr->CreateRenderTargetView(m_resource, nullptr, &m_render_target_view);
				m_mip_render_target_views.push_back(m_render_target_view);
			}
		}

		return m_render_target_view != nullptr;
	}
}