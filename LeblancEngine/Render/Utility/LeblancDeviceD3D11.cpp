#include "LeblancEngine/Render/Utility/LeblancDeviceD3D11.h"

DeviceD3D11::DeviceD3D11()
{

}

DeviceD3D11::~DeviceD3D11()
{
	release();
}

void DeviceD3D11::initialize(HWND window)
{
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC));

	DXGI_RATIONAL refresh_rate;
	refresh_rate.Numerator = 60;
	refresh_rate.Denominator = 1;

	desc.BufferCount = 2;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	desc.BufferDesc.Width = 1280;
	desc.BufferDesc.Height = 720;
	desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	desc.BufferDesc.RefreshRate = refresh_rate;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	desc.OutputWindow = window;
	desc.Windowed = true;

	UINT flags = D3D11_CREATE_DEVICE_SINGLETHREADED;

#ifdef _DEBUG
	flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, flags,
		NULL, 0, D3D11_SDK_VERSION, &desc, &m_swap_chain, &m_device, NULL, &m_device_context);
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
	{
		m_device_context->Release();
		m_device_context = nullptr;
	}

	if (m_device)
	{
		m_device->Release();
		m_device = nullptr;
	}

	if (m_swap_chain)
	{
		m_swap_chain->Release();
		m_swap_chain = nullptr;
	}
}
