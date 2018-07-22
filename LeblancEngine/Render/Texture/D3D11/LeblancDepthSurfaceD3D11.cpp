#include "LeblancEngine/Render/Texture/D3D11/LeblancDepthSurfaceD3D11.h"
#include "LeblancEngine/BasicInclude/LeblancMemoryOperation.h"
#include "LeblancEngine/Render/Defines/LeblancRenderBasicDefine.h"
#include "LeblancEngine/Render/Defines/D3D11/LeblancRenderUtilityD3D11.h"
#include "LeblancEngine/Render/Device/D3D11/LeblancDeviceD3D11.h"
#include "LeblancEngine/Render/Device/D3D11/LeblancDeviceContextD3D11.h"
#include "LeblancEngine/Render/Texture/LeblancSurface.h"

namespace Leblanc
{
	DepthSurfaceD3D11::DepthSurfaceD3D11(IDevice* device) : IDepthSurface(device)
	{

	}

	DepthSurfaceD3D11::~DepthSurfaceD3D11()
	{
		release();
	}

	void DepthSurfaceD3D11::release()
	{
		safe_Release(m_depth_surface);
		safe_Release(m_depth_shader_resource_view);
		safe_Release(m_depth_texture);
	}

	bool DepthSurfaceD3D11::initialize(const DepthSurfaceParameters* depth_surface_parameters)
	{
		m_depth_surface_parameters = *depth_surface_parameters;
		return create();
	}

	bool DepthSurfaceD3D11::create()
	{
		if (m_depth_surface_parameters.width() == MIRROR_BACK_BUFFER)
		{
			m_texture_desc.Width = m_device->backBuffer()->width();
		}
		else if (m_depth_surface_parameters.width() == MIRROR_BACK_BUFFER_HALF)
		{
			m_texture_desc.Width = m_device->backBuffer()->width() / 2;
		}
		else if (m_depth_surface_parameters.width() == MIRROR_BACK_BUFFER_QUARTER)
		{
			m_texture_desc.Width = m_device->backBuffer()->width() / 4;
		}
		else if (m_depth_surface_parameters.width() == MIRROR_BACK_BUFFER_EIGTH)
		{
			m_texture_desc.Width = m_device->backBuffer()->width() / 8;
		}
		else
		{
			m_texture_desc.Width = m_depth_surface_parameters.width();
		}

		if (m_depth_surface_parameters.width() == MIRROR_BACK_BUFFER)
		{
			m_texture_desc.Height = m_device->backBuffer()->height();
		}
		else if (m_depth_surface_parameters.width() == MIRROR_BACK_BUFFER_HALF)
		{
			m_texture_desc.Height = m_device->backBuffer()->height() / 2;
		}
		else if (m_depth_surface_parameters.width() == MIRROR_BACK_BUFFER_QUARTER)
		{
			m_texture_desc.Height = m_device->backBuffer()->height() / 4;
		}
		else if (m_depth_surface_parameters.width() == MIRROR_BACK_BUFFER_EIGTH)
		{
			m_texture_desc.Height = m_device->backBuffer()->height() / 8;
		}
		else
		{
			m_texture_desc.Height = m_depth_surface_parameters.height();
		}

		m_texture_desc.MipLevels = 1;
		m_texture_desc.ArraySize = 1;

		m_texture_desc.Format = findFormat(m_depth_surface_parameters.format());
		m_texture_desc.SampleDesc.Count = m_depth_surface_parameters.multiSampleCount();
		m_texture_desc.SampleDesc.Quality = m_depth_surface_parameters.multiSampleQuality();
		m_texture_desc.Usage = D3D11_USAGE_DEFAULT;
		m_texture_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;

		m_texture_desc.CPUAccessFlags = 0;
		m_texture_desc.MiscFlags = 0;

		DeviceD3D11* device_d3d11 = dynamic_cast<DeviceD3D11*>(m_device);
		if (device_d3d11)
		{
			ID3D11Device* device_ptr = (ID3D11Device*)device_d3d11->device();
			HRESULT hr = device_ptr->CreateTexture2D(&m_texture_desc, nullptr, &m_depth_texture);
			if (FAILED(hr))
			{
				//LOG_CRITICAL("Failed to create Texture2D for depth!\n");
			}

			DXGI_FORMAT srv_format = DXGI_FORMAT_UNKNOWN;
			DXGI_FORMAT dsv_format = DXGI_FORMAT_UNKNOWN;

			if (m_texture_desc.Format == DXGI_FORMAT_R16_TYPELESS)
			{
				srv_format = DXGI_FORMAT_R16_UNORM;
				dsv_format = DXGI_FORMAT_D16_UNORM;
			}
			else if (m_texture_desc.Format == DXGI_FORMAT_R32_TYPELESS)
			{
				srv_format = DXGI_FORMAT_R32_FLOAT;
				dsv_format = DXGI_FORMAT_D32_FLOAT;
			}
			else if (m_texture_desc.Format == DXGI_FORMAT_R24G8_TYPELESS ||
				m_texture_desc.Format == DXGI_FORMAT_D24_UNORM_S8_UINT)
			{
				dsv_format = DXGI_FORMAT_D24_UNORM_S8_UINT;
				srv_format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
			}

			// Create the depth stencil view
			D3D11_DEPTH_STENCIL_VIEW_DESC desc_dsv;
			memset(&desc_dsv, 0, sizeof(desc_dsv));
			desc_dsv.Format = dsv_format;

			desc_dsv.ViewDimension = m_depth_surface_parameters.multiSampleCount() > 1 ? D3D11_DSV_DIMENSION_TEXTURE2DMS : D3D11_DSV_DIMENSION_TEXTURE2D;
			desc_dsv.Texture2D.MipSlice = 0;
			desc_dsv.Flags = 0;

			if (SUCCEEDED(device_ptr->CreateDepthStencilView(m_depth_texture, &desc_dsv, &m_depth_surface)))
			{
				m_depth_surface->GetDesc(&m_depth_view_desc);
			}
			else
			{
				//LOG("Failed to to create depth surface view");
			}

			// Create a shader resource view
			D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;

			srv_desc.Format = srv_format;
			srv_desc.ViewDimension = m_depth_surface_parameters.multiSampleCount() > 1 ? D3D11_SRV_DIMENSION_TEXTURE2DMS : D3D11_SRV_DIMENSION_TEXTURE2D;
			srv_desc.Texture2D.MipLevels = 1;
			srv_desc.Texture2D.MostDetailedMip = 0;

			ID3D11ShaderResourceView * resource_view = nullptr;
			if (SUCCEEDED(device_ptr->CreateShaderResourceView(m_depth_texture,
				&srv_desc,
				&resource_view)))
			{
				m_depth_shader_resource_view = resource_view;
			}
			else
			{
				//LOG("FAILED in create shader resource view for depthTexture");
			}

			return SUCCEEDED(hr);
		}
		return false;
	}

	bool DepthSurfaceD3D11::bind(IDeviceContext* device_context) const
	{
		DeviceContextD3D11* device_context_d3d11 = dynamic_cast<DeviceContextD3D11*>(device_context);
		if (device_context_d3d11)
		{
			device_context_d3d11->bindDepthTarget(this);
			return true;
		}
		return false;
	}

	bool DepthSurfaceD3D11::clear(IDeviceContext* device_context)
	{
		DeviceContextD3D11* device_context_d3d11 = dynamic_cast<DeviceContextD3D11*>(device_context);
		if (depthStencilView() && device_context_d3d11)
		{
			device_context_d3d11->clearDepthSurface(this, CLEAR_DEPTH | CLEAR_STENCIL);
			return true;
		}
		return false;
	}

	bool DepthSurfaceD3D11::clearStencil(IDeviceContext* device_context)
	{
		DeviceContextD3D11* device_context_d3d11 = dynamic_cast<DeviceContextD3D11*>(device_context);
		if (depthStencilView() && device_context_d3d11)
		{
			device_context_d3d11->clearDepthSurface(this, CLEAR_STENCIL);
			return true;
		}
		return false;
	}

	int DepthSurfaceD3D11::width() const
	{
		return m_texture_desc.Width;
	}

	int DepthSurfaceD3D11::height() const
	{
		return m_texture_desc.Height;
	}

	int DepthSurfaceD3D11::multiSampleCount() const
	{
		return m_texture_desc.SampleDesc.Count;
	}

	int DepthSurfaceD3D11::multiSampleQuality() const
	{
		return m_texture_desc.SampleDesc.Quality;
	}

	ID3D11DepthStencilView* DepthSurfaceD3D11::depthStencilView() const
	{
		return m_depth_surface;
	}

	ID3D11Texture2D* DepthSurfaceD3D11::depthTexture() const
	{
		return m_depth_texture;
	}

	ID3D11ShaderResourceView* DepthSurfaceD3D11::shaderResourceView() const
	{
		return m_depth_shader_resource_view;
	}

	D3D11_TEXTURE2D_DESC DepthSurfaceD3D11::textureDesc() const
	{
		return m_texture_desc;
	}
}