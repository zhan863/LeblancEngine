#include "LeblancEngine/Render/Resource/Texture/Texture2D.h"

Texture2D::Texture2D()
{

}

Texture2D::~Texture2D()
{
	release();
}

bool Texture2D::intialize(ID3D11Device* device, UINT width, UINT height)
{
	if (!device || m_d3d11_texture2d)
		return true;

	D3D11_TEXTURE2D_DESC desc;
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

	HRESULT hr = device->CreateTexture2D(&desc, nullptr, &m_d3d11_texture2d);

	return hr == S_OK;
}

void Texture2D::release()
{
	if (m_d3d11_texture2d)
	{
		m_d3d11_texture2d->Release();
		m_d3d11_texture2d = nullptr;
	}
}

ID3D11Resource* Texture2D::getResource()
{
	return m_d3d11_texture2d;
}