#include "LeblancEngine/Render/Resource/Texture/DepthStencilTexture.h"

DepthStencilTexture::DepthStencilTexture()
{

}

DepthStencilTexture::~DepthStencilTexture()
{
	release();
}

bool DepthStencilTexture::intialize(ID3D11Device* device, UINT width, UINT height)
{
	if (!device || m_depth_stencil_texture)
		return true;

	D3D11_TEXTURE2D_DESC desc;
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

	HRESULT hr = device->CreateTexture2D(&desc, nullptr, &m_depth_stencil_texture);

	return hr == S_OK;
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