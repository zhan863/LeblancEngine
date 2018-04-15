#include "LeblancEngine/Render/Utility/LeblancDeviceContextD3D11.h"
#include "LeblancEngine/BasicInclude/LeblancMemoryOperation.h"
#include "LeblancEngine/Render/Resource/Texture/DepthStencilTexture.h"
#include "LeblancEngine/Render/Resource/Texture/Texture2D.h"
#include "LeblancEngine/Render/RenderEntity/LeblancIndexMesh.h"
#include "LeblancEngine/Global/LeblancGlobalContext.h"

const static UINT k_max_render_target_view = 8;

DeviceContextD3D11::DeviceContextD3D11(DeviceD3D11* device) : m_device(device)
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

void DeviceContextD3D11::setRenderTargets(UINT num_targets, Texture2D** render_targets, DepthStencilTexture* depth_stentil_texture)
{
	ID3D11DepthStencilView* depth_stencil_view = depth_stentil_texture ? (ID3D11DepthStencilView*)depth_stentil_texture->getRenderTargetView() : nullptr;
	ID3D11RenderTargetView* render_target_views[k_max_render_target_view] = { nullptr };
	for (UINT i = 0; i < num_targets; i++)
	{
		render_target_views[i] = (ID3D11RenderTargetView*)render_targets[i]->getRenderTargetView();
	}

	m_device_context->OMSetRenderTargets(num_targets, render_target_views, depth_stencil_view);
}


void DeviceContextD3D11::setRenderTargets(UINT num_targets, ID3D11RenderTargetView** render_targets, DepthStencilTexture* depth_stentil_texture)
{
	ID3D11DepthStencilView* depth_stencil_view = depth_stentil_texture ? (ID3D11DepthStencilView*)depth_stentil_texture->getRenderTargetView() : nullptr;
	m_device_context->OMSetRenderTargets(num_targets, render_targets, depth_stencil_view);
}

void DeviceContextD3D11::clearRenderTarget(ID3D11RenderTargetView* render_target)
{
	float red[4] = { 1.0, 0.0 ,0.0, 1.0 };
	m_device_context->ClearRenderTargetView(render_target, red);
}

void DeviceContextD3D11::setInputLayout(ID3D11InputLayout* input_layout)
{
	m_device_context->IASetInputLayout(input_layout);
}

void DeviceContextD3D11::renderIndexMesh(IndexMesh* mesh)
{
	UINT offset = 0;
	mesh->getVertexBuffer()->bind();
	mesh->getIndexBuffer()->bind();
	m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_device_context->DrawIndexed(mesh->getIndexCount(), 0, 0);
}

void DeviceContextD3D11::setViewPort(FLOAT left_x, FLOAT tp_y, FLOAT width, FLOAT height)
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
	RasterizerStateD3D11* rasterizer_state = g_global_context.m_render_state_manager.getOrCreateRasterizerState(rasterizer_mode);
	if (rasterizer_state)
	{
		m_device_context->RSSetState(rasterizer_state->getRasterizerState());
	}
}

void DeviceContextD3D11::setDepthStencilState(DepthStencilState depth_stencil_mode)
{
	DepthStencilStateD3D11* depth_stencil_state = g_global_context.m_render_state_manager.getOrCreateDepthStencilState(depth_stencil_mode);
	if (depth_stencil_state)
	{
		m_device_context->OMSetDepthStencilState(depth_stencil_state->getDepthStencilState(), 0);
	}
}

void DeviceContextD3D11::setBlendState(BlendState blend_mode)
{
	BlendStateD3D11* blend_state = g_global_context.m_render_state_manager.getOrCreateBlendState(blend_mode);
	if (blend_state)
	{
		FLOAT blend_factor[4] = { 1, 1, 1, 1 };
		m_device_context->OMSetBlendState(blend_state->getBlendState(), blend_factor, 0xffffffff);
	}
}