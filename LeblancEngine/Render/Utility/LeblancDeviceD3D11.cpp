#include "LeblancEngine/Render/Utility/LeblancDeviceD3D11.h"
#include "LeblancEngine/Render/Basics/LeblancGeometry.h"
#include "LeblancEngine/Render/RenderEntity/LeblancIndexMesh.h"
#include "LeblancEngine/Global/LeblancGlobalContext.h"
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

	HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, flags,
		NULL, 0, D3D11_SDK_VERSION, &desc, &m_swap_chain, &m_device, NULL, &m_device_context);

	if (m_swap_chain && hr == S_OK)
	{
		ID3D11Texture2D* back_buffer = nullptr;
		m_swap_chain->GetBuffer(0, __uuidof(back_buffer), reinterpret_cast<void**>(&back_buffer));
		m_device->CreateRenderTargetView(back_buffer, nullptr, &m_back_buffer_view);
	}
}

ID3D11RenderTargetView* DeviceD3D11::getBackBufferView()
{
	return m_back_buffer_view;
}

Mesh1* DeviceD3D11::createMesh(vector<Vertex>& vertices, vector<UINT>& indices)
{
	// Create the encapsulated mesh
	Mesh1 *p_mesh = new Mesh1;

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
		index_buffer_desc.ByteWidth = sizeof(UINT) * indices.size();
		index_buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		index_buffer_desc.CPUAccessFlags = 0;
		index_buffer_desc.MiscFlags = 0;

		// Define the resource data.
		D3D11_SUBRESOURCE_DATA init_index_data;
		init_index_data.pSysMem = &indices[0];
		init_index_data.SysMemPitch = 0;
		init_index_data.SysMemSlicePitch = 0;

		p_mesh->m_stride = sizeof(Vertex);
		p_mesh->m_index_size = indices.size();

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


void DeviceD3D11::setRenderTargets(UINT num_targets, ID3D11RenderTargetView** render_targets, DepthStencilTexture* depth_stentil_texture)
{
	ID3D11DepthStencilView* depth_stencil_view = depth_stentil_texture ? (ID3D11DepthStencilView*)depth_stentil_texture->getRenderTargetView() : nullptr;
	m_device_context->OMSetRenderTargets(num_targets, render_targets, depth_stencil_view);
}

void DeviceD3D11::clearRenderTarget(ID3D11RenderTargetView* render_target)
{
	float red[4] = { 1.0, 0.0 ,0.0, 1.0 };
	m_device_context->ClearRenderTargetView(render_target, red);
}

void DeviceD3D11::setVertexShader(VertexShader* vertex_shader)
{
	m_device_context->VSSetShader(vertex_shader->getVertexShader(), nullptr, 0);
}

void DeviceD3D11::setPixelShader(PixelShader* pixel_shader)
{
	m_device_context->PSSetShader(pixel_shader->getPixelShader(), nullptr, 0);
}

void DeviceD3D11::setInputLayout(ID3D11InputLayout* input_layout)
{
	m_device_context->IASetInputLayout(input_layout);
}

void DeviceD3D11::renderIndexMesh(IndexMesh* mesh)
{
	UINT offset = 0;
	mesh->getVertexBuffer()->bind();
	mesh->getIndexBuffer()->bind();
	m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_device_context->DrawIndexed(mesh->getIndexCount(), 0, 0);
}

void DeviceD3D11::setViewPort(FLOAT left_x, FLOAT tp_y, FLOAT width, FLOAT height)
{
	D3D11_VIEWPORT view_port;
	view_port.TopLeftX = left_x;
	view_port.TopLeftY = tp_y;
	view_port.Width = width;
	view_port.Height = height;
	view_port.MinDepth = 0;
	view_port.MaxDepth = 1;

	m_device_context->RSSetViewports(1, &view_port);
}

void DeviceD3D11::setRasterizerState(RasterizerState rasterizer_mode)
{
	RasterizerStateD3D11* rasterizer_state = g_global_context.m_render_state_manager.getOrCreateRasterizerState(rasterizer_mode);
	if (rasterizer_state)
	{
		m_device_context->RSSetState(rasterizer_state->getRasterizerState());
	}
}

void DeviceD3D11::setDepthStencilState(DepthStencilState depth_stencil_mode)
{
	DepthStencilStateD3D11* depth_stencil_state = g_global_context.m_render_state_manager.getOrCreateDepthStencilState(depth_stencil_mode);
	if (depth_stencil_state)
	{
		m_device_context->OMSetDepthStencilState(depth_stencil_state->getDepthStencilState(), 0);
	}
}

void DeviceD3D11::setBlendState(BlendState blend_mode)
{
	BlendStateD3D11* blend_state = g_global_context.m_render_state_manager.getOrCreateBlendState(blend_mode);
	if (blend_state)
	{
		FLOAT blend_factor[4] = { 1, 1, 1, 1 };
		m_device_context->OMSetBlendState(blend_state->getBlendState(), blend_factor, 0xffffffff);
	}
}

ID3D11InputLayout* DeviceD3D11::createInputLayout(D3D11_INPUT_ELEMENT_DESC* input_layout_desc, UINT layout_desc_count, VertexShader* vertex_shader)
{
	ID3D11InputLayout* layout = nullptr;
	m_device->CreateInputLayout(input_layout_desc, layout_desc_count, vertex_shader->getBlob()->GetBufferPointer(), vertex_shader->getBlob()->GetBufferSize(), &layout);
	return layout;
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
			index_buffer = new IndexBufferD3D11(this);
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
			vertex_buffer = new VertexBufferD3D11(this);
			vertex_buffer->initialize(vertex_buffer_declaration);
		}
	}

	return vertex_buffer;
}