#include "LeblancEngine/Global/LeblancGlobalContext.h"
#include "LeblancEngine/Render/Resource/Texture/DepthStencilTexture.h"

DepthStencilTexture::DepthStencilTexture()
{

}

DepthStencilTexture::~DepthStencilTexture()
{
	release();
}

bool DepthStencilTexture::initialize(UINT width, UINT height, BOOL is_render_target)
{
	if (m_depth_stencil_texture)
		return true;

	DeviceD3D11& device = g_global_context.m_device_manager.getCurrentDevice();

	m_depth_stencil_texture = static_cast<ID3D11Texture2D*>(device.createTexture(TextureType::Depth_Stencil_Texture, width, height));
	m_depth_stencil_view = static_cast<ID3D11DepthStencilView*>(device.createRenderTargetView(TextureType::Depth_Stencil_Texture, width, height, m_depth_stencil_texture));

	return m_depth_stencil_texture != nullptr;
}

void DepthStencilTexture::release()
{
	if (m_depth_stencil_texture)
	{
		m_depth_stencil_texture->Release();
		m_depth_stencil_texture = nullptr;
	}
}

ID3D11Resource* DepthStencilTexture::getResource()
{
	return m_depth_stencil_texture;
}

ID3D11View* DepthStencilTexture::getRenderTargetView()
{
	return m_depth_stencil_view;
}