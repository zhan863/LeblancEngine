#include "LeblancEngine/Render/Utility/LeblancDeviceD3D11.h"

DeviceD3D11::DeviceD3D11()
{

}

DeviceD3D11::~DeviceD3D11()
{
	release();
}

void DeviceD3D11::initialize()
{

}

ID3D11Resource* DeviceD3D11::createTexture(TextureType texture_type, UINT width, UINT height)
{
	if (!m_device)
		return nullptr;

	if (texture_type == Texture2D)
	{
		D3D11_TEXTURE2D_DESC desc;
		desc.Width = width;
		desc.Height = height;
		desc.MipLevels = desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

		ID3D11Texture2D* d3d11_texture_2d = nullptr;
		HRESULT hr = m_device->CreateTexture2D(&desc, nullptr, &d3d11_texture_2d);

		if (hr == S_OK)
		{
			return d3d11_texture_2d;
		}
		else
		{
			return nullptr;
		}
	}
	else if (texture_type == Texture3D)
	{
		return nullptr;
	}
	else if (texture_type == DepthStencilTexture)
	{
		D3D11_TEXTURE2D_DESC desc;
		desc.Width = width;
		desc.Height = height;
		desc.MipLevels = desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		desc.SampleDesc.Count = 1;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

		ID3D11Texture2D* depth_stencil_texture = nullptr;
		HRESULT hr = m_device->CreateTexture2D(&desc, nullptr, &depth_stencil_texture);

		if (hr == S_OK)
		{
			return depth_stencil_texture;
		}
		else
		{
			return nullptr;
		}
	}

	return nullptr;
}

void DeviceD3D11::release()
{
	if (m_device_context)
		m_device_context->Release();

	if (m_device)
		m_device->Release();
}
