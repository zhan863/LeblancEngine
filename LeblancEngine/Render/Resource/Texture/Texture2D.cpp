#include "LeblancEngine/Render/Resource/Texture/Texture2D.h"

#include "LeblancEngine/Global/LeblancGlobalContext.h"

Texture2D::Texture2D()
{

}

Texture2D::~Texture2D()
{
	release();
}

bool Texture2D::intialize(UINT width, UINT height)
{
	if (m_d3d11_texture2d)
		return true;

	DeviceD3D11* device = g_leblanc_engine_global_context.m_device_manager->getCurrentDevice();

	m_d3d11_texture2d = static_cast<ID3D11Texture2D*>(device->createTexture(TextureType::Texture2D, width, height));

	return m_d3d11_texture2d != nullptr;
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