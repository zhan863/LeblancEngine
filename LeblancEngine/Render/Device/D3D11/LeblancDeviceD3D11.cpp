#include "LeblancEngine/Render/Device/D3D11/LeblancDeviceD3D11.h"

#include "LeblancEngine/BasicInclude/LeblancMemoryOperation.h"
#include "LeblancEngine/BasicInclude/LeblancPCH.h"
#include "LeblancEngine/Render/Window/LeblancWindow.h"

#include "LeblancEngine/Render/Buffer/D3D11/LeblancBufferD3D11.h"
#include "LeblancEngine/Render/Texture/D3D11/LeblancBackBufferSurfaceD3D11.h"
#include "LeblancEngine/Render/Device/D3D11/LeblancDeviceContextD3D11.h"
#include "LeblancEngine/Render/Texture/D3D11/LeblancRenderTargetD3D11.h"
#include "LeblancEngine/Render/Texture/D3D11/LeblancTextureD3D11.h"
#include "LeblancEngine/Render/Texture/D3D11/LeblancDepthSurfaceD3D11.h"
#include "LeblancEngine/Render/Texture/LeblancFrameBuffer.h"

#include "LeblancEngine/Render/Material/D3D11/LeblancShadersD3D11.h"

#include "LeblancEngine/Global/LeblancGlobalContext.h"
#include "LeblancEngine/Render/RenderState/D3D11/LeblancRenderStateD3D11.h"

#include "LeblancEngine/Render/Material/D3D11/LeblancConstantBufferD3D11.h"

#include "LeblancEngine/Render/Include/LeblancDirectInclude.h"

namespace Leblanc
{
	DeviceD3D11::DeviceD3D11()
	{

	}

	DeviceD3D11::~DeviceD3D11()
	{
		release();
	}

	void DeviceD3D11::release()
	{
		safe_Release(m_swap_chain);
		safe_Release(m_device);
		safe_delete(m_back_buffer);
	}

	void DeviceD3D11::initialize(Window* window)
	{
		DXGI_SWAP_CHAIN_DESC desc;
		ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC));

		DXGI_RATIONAL refresh_rate;
		refresh_rate.Numerator = 60;
		refresh_rate.Denominator = 1;

		desc.BufferCount = 2;
		desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		INT width = 0, height = 0;
		window->GetClientArea(width, height);
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
		desc.OutputWindow = (HWND)(*window);
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
			m_back_buffer = new BackBufferSurfaceD3D11(this, m_swap_chain);
			m_back_buffer->initialize();
			m_depth_surface = new DepthSurfaceD3D11(this);

			const ISurface* render_targets[1] = { m_back_buffer };
			m_frame_buffer = new FrameBuffer(render_targets, m_depth_surface, 1);

			DeviceContextD3D11* immediate_device_context = new DeviceContextD3D11(this);
			immediate_device_context->setHandle(device_context);
			m_immediate_context = immediate_device_context;
			g_global_context.m_device_manager.registerImmediateContext(m_immediate_context);
		}
	}

	ITexture* DeviceD3D11::createTexture(const ResourceDeclaration* resource_declaration)
	{
		if (!m_device)
			return nullptr;

		if (const TextureParameters* texture_parameters = dynamic_cast<const TextureParameters*>(resource_declaration))
		{
			ITexture* texture = nullptr;
			
			if (texture_parameters->type() == FromFile ||
				texture_parameters->type() == StagingFromFile ||
				texture_parameters->type() == Procedural)
			{
				if (texture_parameters->dimension() == TwoD || texture_parameters->dimension() == CubeMap)
				{
					texture = new TextureD3D11(this);
				}
				else
				{
					// LOG ERROR UNSUPPORT TYPE;
				}
			}
			else if (texture_parameters->type() == RenderTarget)
			{
				texture = new RenderTargetTextureD3D11(this);
			}

			if (texture)
			{
				if (texture->initialize(texture_parameters))
					return texture;
				else
				{
					safe_delete(texture);
				}
			}
		}
		return nullptr;
	}

	IVertexDeclaration* DeviceD3D11::createVertexDeclaration(const VertexLayoutDeclaration* layout_declaration)
	{
		VertexDeclarationD3D11* new_vertex_declaration = new VertexDeclarationD3D11(this, m_immediate_context);
		if (new_vertex_declaration)
		{
			new_vertex_declaration->initialize(layout_declaration);
			return new_vertex_declaration;
		}

		return nullptr;
	}

	IRasterizerState* DeviceD3D11::createRasterizerState(const RasterizerState* rasterizer_state_declaration)
	{
		RasterizerStateD3D11* new_rast_state = new RasterizerStateD3D11(this, m_immediate_context);
		if (new_rast_state)
		{
			new_rast_state->initialize(rasterizer_state_declaration);
		}

		return new_rast_state;
	}

	IDepthStencilState* DeviceD3D11::createDepthStencilState(const DepthStencilState* depth_stencil_declaration)
	{
		DepthStencilStateD3D11* new_depth_state = new DepthStencilStateD3D11(this, m_immediate_context);
		if (new_depth_state)
		{
			new_depth_state->initialize(depth_stencil_declaration);
		}

		return new_depth_state;
	}
	
	IBlendState* DeviceD3D11::createBlendState(const BlendState* blend_declaration)
	{
		BlendStateD3D11* new_blend_state = new BlendStateD3D11(this, m_immediate_context);
		if (new_blend_state)
		{
			new_blend_state->initialize(blend_declaration);
		}

		return new_blend_state;
	}

	IShader* DeviceD3D11::createShader(string file_name)
	{
		ShaderD3D11* new_shader = new ShaderD3D11(g_global_context.m_device_manager.getDevice(), g_global_context.m_device_manager.getImmediateContext());
		if (new_shader)
			new_shader->initialize(file_name);

		return new_shader;
	}

	void DeviceD3D11::present()
	{
		if (m_swap_chain)
		{
			m_swap_chain->Present(0, 0);
		}
	}

	// new create resource data
	void DeviceD3D11::createBuffer(const D3D11_BUFFER_DESC *desc, D3D11_SUBRESOURCE_DATA *initial_data, ID3D11Buffer **buffer)
	{
		m_device->CreateBuffer(desc, initial_data, buffer);
	}

	IIndexBuffer* DeviceD3D11::createIndexBuffer(const ResourceDeclaration* declaration)
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

	IVertexBuffer* DeviceD3D11::createVertexBuffer(const ResourceDeclaration* declaration)
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

	ConstantBuffer* DeviceD3D11::createConstantBuffer(int slot_index, size_t buffer_size)
	{
		ConstantBufferD3D11* constantBuffer = new ConstantBufferD3D11(this, g_global_context.m_device_manager.getImmediateContext(), slot_index, buffer_size);
		return constantBuffer;
	}

	void DeviceD3D11::createRasterizerState(D3D11_RASTERIZER_DESC* desc, ID3D11RasterizerState** state)
	{
		m_device->CreateRasterizerState(desc, state);
	}

	void DeviceD3D11::createBlendState(D3D11_BLEND_DESC* desc, ID3D11BlendState** state)
	{
		m_device->CreateBlendState(desc, state);
	}

	void DeviceD3D11::createDepthStencilState(D3D11_DEPTH_STENCIL_DESC* desc, ID3D11DepthStencilState** state)
	{
		m_device->CreateDepthStencilState(desc, state);
	}
}