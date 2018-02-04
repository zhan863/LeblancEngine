#include "LeblancEngine/Render/Utility/LeblancDeviceD3D11.h"
#include "LeblancEngine/Render/Basics/LeblancGeometry.h"
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

LeblancMesh* DeviceD3D11::createMesh(vector<Vertex>& vertices, vector<DWORD>& indices)
{
	// Create the encapsulated mesh
	LeblancMesh *p_mesh = new LeblancMesh;

	if (p_mesh)
	{
		// Fill in a buffer description.
		D3D11_BUFFER_DESC vertex_buffer_desc;
		vertex_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
		vertex_buffer_desc.ByteWidth = sizeof(Vertex) * vertices.size();
		vertex_buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertex_buffer_desc.CPUAccessFlags = 0;
		vertex_buffer_desc.MiscFlags = 0;

		// Fill in the subresource data.
		D3D11_SUBRESOURCE_DATA Init_vertex_data;
		Init_vertex_data.pSysMem = &vertices[0];
		Init_vertex_data.SysMemPitch = 0;
		Init_vertex_data.SysMemSlicePitch = 0;

		// Create the vertex buffer.
		HRESULT hr_vertex = m_device->CreateBuffer(&vertex_buffer_desc, &Init_vertex_data, &p_mesh->m_vertex_buffer);
		
		// Fill in a buffer description.
		D3D11_BUFFER_DESC index_buffer_desc;
		index_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
		index_buffer_desc.ByteWidth = sizeof(DWORD) * indices.size();
		index_buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		index_buffer_desc.CPUAccessFlags = 0;
		index_buffer_desc.MiscFlags = 0;

		// Define the resource data.
		D3D11_SUBRESOURCE_DATA init_index_data;
		init_index_data.pSysMem = &indices[0];
		init_index_data.SysMemPitch = 0;
		init_index_data.SysMemSlicePitch = 0;

		// Create the buffer with the device.
		HRESULT hr_index = m_device->CreateBuffer(&index_buffer_desc, &init_index_data, &p_mesh->m_index_buffer);
		
		if (hr_index != S_OK || hr_vertex != S_OK)
		{
			if (p_mesh->m_vertex_buffer)
			{
				p_mesh->m_vertex_buffer->Release();
			}

			if (p_mesh->m_index_buffer)
			{
				p_mesh->m_index_buffer->Release();
			}

			return nullptr;
		}
	}

	return p_mesh;
}

ID3D11Resource* DeviceD3D11::createTexture(TextureType texture_type, UINT width, UINT height)
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

ID3D11View* DeviceD3D11::createRenderTargetView(TextureType texture_type, UINT width, UINT height, ID3D11Resource* resource)
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

void DeviceD3D11::setRenderTargets(UINT num_targets, Texture2D** render_targets, DepthStencilTexture* depth_stentil_texture)
{
	ID3D11DepthStencilView* depth_stencil_view = depth_stentil_texture ? (ID3D11DepthStencilView*)depth_stentil_texture->getRenderTargetView() : nullptr;
	ID3D11RenderTargetView* render_target_views[k_max_render_target_view] = { nullptr };
	for (UINT i = 0; i < num_targets; i++)
	{
		render_target_views[i] = (ID3D11RenderTargetView*)render_targets[i]->getRenderTargetView();
	}

	m_device_context->OMSetRenderTargets(num_targets, render_target_views, depth_stencil_view);
}