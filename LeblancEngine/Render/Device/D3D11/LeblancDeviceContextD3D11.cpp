#include "LeblancEngine/Render/Device/D3D11/LeblancDeviceContextD3D11.h"
#include "LeblancEngine/Render/RenderState/LeblancRenderState.h"
#include "LeblancEngine/Render/RenderState/D3D11/LeblancRenderStateD3D11.h"
#include "LeblancEngine/Render/RenderState/LeblancRenderStateManager.h"
#include "LeblancEngine/Render/Include/LeblancDirectInclude.h"
#include "LeblancEngine/Global/LeblancGlobalContext.h"
#include "LeblancEngine/BasicInclude/LeblancMemoryOperation.h"
#include "LeblancEngine/Render/RenderEntity/LeblancIndexMesh.h"
#include "LeblancEngine/Render/Buffer/LeblancBuffer.h"
#include "LeblancEngine/Render/Texture/D3D11/LeblancSurfaceD3D11.h"
#include "LeblancEngine/Render/Texture/D3D11/LeblancDepthSurfaceD3D11.h"
#include "LeblancEngine/Render/Texture/D3D11/LeblancDepthSurfaceD3D11.h"
#include "LeblancEngine/Render/Texture/LeblancFrameBuffer.h"

namespace Leblanc
{
	const int k_max_render_target_view = 4;
	DeviceContextD3D11::DeviceContextD3D11(IDevice* device) : IDeviceContext(device)
	{

	}

	DeviceContextD3D11::~DeviceContextD3D11()
	{
		release();
	}

	void DeviceContextD3D11::release()
	{
		safe_Release(m_device_context);
	}

	void DeviceContextD3D11::bindTargets()
	{
		ID3D11RenderTargetView* rtv[MAX_RENDER_TARGETS];
		memset(&rtv[0], 0, sizeof(ID3D11RenderTargetView*) * MAX_RENDER_TARGETS);

		// unbind all shader resources
		ID3D11ShaderResourceView * const p_srv[16] = { nullptr };
		m_device_context->PSSetShaderResources(0, 16, p_srv);

		UINT initial_count[D3D11_PS_CS_UAV_REGISTER_COUNT] = { 0, 0, 0, 0, 0, 0, 0, 0 };
		m_device_context->OMSetRenderTargets(MAX_RENDER_TARGETS,
			rtv,
			nullptr);

		m_current_target_count = 0;
		for (int i = 0; i < MAX_RENDER_TARGETS; i++)
		{
			if (m_current_render_targets[i] == nullptr)
			{
				break;
			}
			else
			{
				if (const SurfaceD3D11 * surface =
					dynamic_cast<const SurfaceD3D11*>(m_current_render_targets[i]))
				{
					rtv[i] = surface->renderTargetView();
					m_current_target_count++;
				}
			}
		}

		ID3D11DepthStencilView* depth_view = nullptr;
		if (m_current_depth_target)
		{
			if (const DepthSurfaceD3D11* depth_surface_d3d11 = dynamic_cast<const DepthSurfaceD3D11*>(m_current_depth_target))
			{
				depth_view = depth_surface_d3d11->depthStencilView();
			}
		}

		m_device_context->OMSetRenderTargets(m_current_target_count,
			&rtv[0],
			depth_view);
	}

	void DeviceContextD3D11::clearSurface(ISurface* surface, float* color) const
	{
		SurfaceD3D11* surface_d3d11 = dynamic_cast<SurfaceD3D11*>(surface);
		if (nullptr != surface_d3d11)
		{
			m_device_context->ClearRenderTargetView(surface_d3d11->renderTargetView(), color);
		}
	}

	void DeviceContextD3D11::clearDepthSurface(IDepthSurface* surface, unsigned long clear_type) const
	{
		SurfaceD3D11* surface_d3d11 = dynamic_cast<SurfaceD3D11*>(surface);
		if (nullptr != surface_d3d11)
		{
			DepthSurfaceD3D11* depth_surface_d3d11 = dynamic_cast<DepthSurfaceD3D11*>(surface);
			if (depth_surface_d3d11)
			{
				m_device_context->ClearDepthStencilView(depth_surface_d3d11->depthStencilView(), 
					((clear_type & CLEAR_DEPTH) ? D3D11_CLEAR_DEPTH : 0) | ((clear_type & CLEAR_STENCIL) ? D3D11_CLEAR_STENCIL : 0), 1, 0);
			}
		}
	}

	void DeviceContextD3D11::clearRenderTarget(unsigned int index_to_clear_to, unsigned long clear_type, float* color) const
	{
		if (clear_type & CLEAR_TARGET)
		{
			for (unsigned int i = 0; i < index_to_clear_to + 1; i++)
			{
				if (const SurfaceD3D11* surface = dynamic_cast <const SurfaceD3D11*>(m_current_render_targets[i]))
				{
					m_device_context->ClearRenderTargetView(surface->renderTargetView(), color);
				}
			}
		}

		if (m_current_depth_target && ((clear_type & CLEAR_DEPTH) || (clear_type & CLEAR_STENCIL)))
		{
			if (const DepthSurfaceD3D11* depth_surface_d3d11 = dynamic_cast<const DepthSurfaceD3D11*>(m_current_depth_target))
			{
				m_device_context->ClearDepthStencilView(depth_surface_d3d11->depthStencilView(),
					((clear_type & CLEAR_DEPTH) ? D3D11_CLEAR_DEPTH : 0) | ((clear_type & CLEAR_STENCIL) ? D3D11_CLEAR_STENCIL : 0), 1, 0);
			}
		}
	}

	void DeviceContextD3D11::bindRenderTarget(int index, const ISurface* surface)
	{
		m_current_render_targets[index] = surface;

		bindTargets();
	}

	void DeviceContextD3D11::bindRenderTargets(int target_size, const ISurface* surface)
	{
		for (int i = 0; i < target_size && i < MAX_RENDER_TARGETS; i++)
		{
			m_current_render_targets[i] = surface;
		}

		bindTargets();
	}

	void DeviceContextD3D11::bindDepthTarget(const IDepthSurface* surface)
	{
		m_current_depth_target = surface;
		
		bindTargets();
	}

	void DeviceContextD3D11::bindNullFrameBuffer()
	{
		for (int i = 0; i < k_max_render_target_view; i++)
		{
			m_current_render_targets[i] = nullptr;
		}
		m_current_target_count = 0;

		m_current_depth_target = nullptr;
	}

	void DeviceContextD3D11::bindFrameBuffer(const FrameBuffer* frame_buffer)
	{
		bindNullFrameBuffer();

		m_current_target_count = frame_buffer->getRenderTargetNum();
		for (int i = 0; i < m_current_target_count; i++)
		{
			m_current_render_targets[i] = frame_buffer->getColorSurfaces(i);
		}

		m_current_depth_target = frame_buffer->getDepthSurface();

		bindTargets();
	}

	void DeviceContextD3D11::setViewPort(int left_x, int tp_y, int width, int height)
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

	void DeviceContextD3D11::setRasterizerState(RasterizerState rasterizer_mode)
	{
		RasterizerStateD3D11* rasterizer_state = dynamic_cast<RasterizerStateD3D11*>(g_global_context.m_render_state_manager.getOrCreateRasterizerState(rasterizer_mode));
		if (rasterizer_state)
		{
			m_device_context->RSSetState(rasterizer_state->getRasterizerState());
		}
	}

	void DeviceContextD3D11::setDepthStencilState(DepthStencilState depth_stencil_mode)
	{
		DepthStencilStateD3D11* depth_stencil_state = dynamic_cast<DepthStencilStateD3D11*>(g_global_context.m_render_state_manager.getOrCreateDepthStencilState(depth_stencil_mode));
		if (depth_stencil_state)
		{
			m_device_context->OMSetDepthStencilState(depth_stencil_state->getDepthStencilState(), 0);
		}
	}

	void DeviceContextD3D11::setBlendState(BlendState blend_mode)
	{
		BlendStateD3D11* blend_state = dynamic_cast<BlendStateD3D11*>(g_global_context.m_render_state_manager.getOrCreateBlendState(blend_mode));
		if (blend_state)
		{
			FLOAT blend_factor[4] = { 1, 1, 1, 1 };
			m_device_context->OMSetBlendState(blend_state->getBlendState(), blend_factor, 0xffffffff);
		}
	}

	void DeviceContextD3D11::renderIndexMesh(IndexMesh* mesh)
	{
		UINT offset = 0;
		mesh->getVertexBuffer()->bind();
		mesh->getIndexBuffer()->bind();
		m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_device_context->DrawIndexed(mesh->getIndexCount(), 0, 0);
	}
}