#include "LeblancEngine/Render/Utility/LeblancDeviceD3D11.h"
#include "LeblancEngine/Render/RenderEntity/LeblancIndexMesh.h"
#include "LeblancEngine/Global/LeblancGlobalContext.h"
#include "LeblancEngine/Render/Utility/LeblancDeviceContextD3D11.h"
#include "LeblancEngine/BasicInclude/LeblancMemoryOperation.h"
#include <vector>

using namespace std;
const static UINT k_max_render_target_view = 8;

DeviceD3D11::DeviceD3D11()
{

}

DeviceD3D11::~DeviceD3D11()
{
	release();
}

void DeviceD3D11::initialize(Window& window)
{
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC));

	DXGI_RATIONAL refresh_rate;
	refresh_rate.Numerator = 60;
	refresh_rate.Denominator = 1;

	desc.BufferCount = 2;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	INT width = 0, height = 0;
	window.GetClientArea(width, height);
	desc.BufferDesc.Width = width;
	desc.BufferDesc.Height = height;
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

	ID3D11DeviceContext* device_context = nullptr;

	HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, flags,
		NULL, 0, D3D11_SDK_VERSION, &desc, &m_swap_chain, &m_device, NULL, &device_context);

	if (m_swap_chain && hr == S_OK)
	{
		ID3D11Texture2D* back_buffer = nullptr;
		m_swap_chain->GetBuffer(0, __uuidof(back_buffer), reinterpret_cast<void**>(&back_buffer));
		m_device->CreateRenderTargetView(back_buffer, nullptr, &m_back_buffer_view);

		DeviceContextD3D11* immediate_device_context = new DeviceContextD3D11(this);
		immediate_device_context->setHandle(device_context);
		g_global_context.m_device_manager.registerImmediateContext(immediate_device_context);
	}
}

ID3D11RenderTargetView* DeviceD3D11::getBackBufferView()
{
	return m_back_buffer_view;
}

ID3D11Resource* DeviceD3D11::createTexture(TextureTypes texture_type, UINT width, UINT height)
{
	if (!m_device)
		return nullptr;

	if (texture_type == Texture_2D)
	{
		D3D11_TEXTURE2D_DESC desc;
		desc.Width = width;
		desc.Height = height;
		desc.MipLevels = desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

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
	else if (texture_type == Texture_3D)
	{
		return nullptr;
	}
	else if (texture_type == Depth_Stencil_Texture)
	{
		D3D11_TEXTURE2D_DESC desc;
		desc.Width = width;
		desc.Height = height;
		desc.MipLevels = desc.ArraySize = 1;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		desc.Usage = D3D11_USAGE_DEFAULT;
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

ID3D11View* DeviceD3D11::createRenderTargetView(TextureTypes texture_type, UINT width, UINT height, ID3D11Resource* resource)
{
	if (texture_type == Depth_Stencil_Texture)
	{

	}
	else if (texture_type == Texture_2D)
	{
		ID3D11RenderTargetView* render_target_view = nullptr;

		D3D11_RENDER_TARGET_VIEW_DESC target_desc;
		target_desc.Texture2D.MipSlice = 0;
		target_desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		target_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

		m_device->CreateRenderTargetView(resource, &target_desc, &render_target_view);

		return render_target_view;
	}
	else if (texture_type == Texture_3D)
	{

	}

	return nullptr;
}

void DeviceD3D11::present()
{
	if (m_swap_chain)
	{
		m_swap_chain->Present(0, 0);
	}
}

void DeviceD3D11::release()
{
	safe_Release(m_swap_chain);
	safe_Release(m_back_buffer_view);
	safe_Release(m_device);
}

// new create resource data
void DeviceD3D11::createBuffer(const D3D11_BUFFER_DESC *desc, D3D11_SUBRESOURCE_DATA *initial_data, ID3D11Buffer **buffer)
{
	m_device->CreateBuffer(desc, initial_data, buffer);
}

IndexBufferD3D11* DeviceD3D11::createIndexBuffer(const ResourceDeclaration* declaration)
{
	IndexBufferD3D11* index_buffer = nullptr;
	if (declaration)
	{
		if (const IndexBufferDeclaration* index_buffer_declaration = dynamic_cast<const IndexBufferDeclaration*>(declaration))
		{
			index_buffer = new IndexBufferD3D11(this, g_global_context.m_device_manager.getImmediateContext());
			index_buffer->initialize(index_buffer_declaration);
		}
	}

	return index_buffer;
}

VertexBufferD3D11* DeviceD3D11::createVertexBuffer(const ResourceDeclaration* declaration)
{
	VertexBufferD3D11* vertex_buffer = nullptr;

	if (declaration)
	{
		if (const VertexBufferDeclaration* vertex_buffer_declaration = dynamic_cast<const VertexBufferDeclaration*>(declaration))
		{
			vertex_buffer = new VertexBufferD3D11(this, g_global_context.m_device_manager.getImmediateContext());
			vertex_buffer->initialize(vertex_buffer_declaration);
		}
	}

	return vertex_buffer;
}