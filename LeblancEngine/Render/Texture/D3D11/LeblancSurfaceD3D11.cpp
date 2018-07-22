#include "LeblancEngine/Render/Texture/D3D11/LeblancSurfaceD3D11.h"
#include "LeblancEngine/Render/Texture/D3D11/LeblancTextureD3D11.h"
#include "LeblancEngine/Render/Device/D3D11/LeblancDeviceD3D11.h"
#include "LeblancEngine/Render/Device/D3D11/LeblancDeviceContextD3D11.h"
#include "LeblancEngine/BasicInclude/LeblancMemoryOperation.h"

namespace Leblanc
{
	SurfaceD3D11::SurfaceD3D11(IDevice* device) : ISurface(device)
	{

	}

	SurfaceD3D11::~SurfaceD3D11()
	{
		release();
	}

	void SurfaceD3D11::release()
	{
		for (int i = 0; i < m_mip_render_target_views.size(); i++)
			safe_Release(m_mip_render_target_views[i]);
		m_mip_render_target_views.clear();

		m_render_target_view = nullptr;
	}

	bool SurfaceD3D11::initialize(int first_array_index, int array_size, ITexture* texture, int mip_level)
	{
		m_first_array_index = first_array_index;
		m_array_size = array_size;
		m_texture = dynamic_cast<TextureD3D11*>(texture);
		m_mip_level = mip_level;

		return create();
	}

	bool SurfaceD3D11::create()
	{
		// Get the view from the current texture
		if (m_texture)
		{
			if (m_resource = (ID3D11Texture2D*)(m_texture->getResource()))
			{
				m_resource->GetDesc(&m_texture_desc);

				D3D11_RENDER_TARGET_VIEW_DESC desc;
				memset(&desc, 0, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
				desc.Format = m_texture_desc.Format;

				if (m_texture->isCubemap() || m_texture->arraySize() > 1)
				{
					desc.ViewDimension = m_texture->multiSampleCount() > 1 ? D3D11_RTV_DIMENSION_TEXTURE2DMSARRAY : D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
					
					if (m_mip_level == -1)
						desc.Texture2DArray.MipSlice = 0;
					else
						desc.Texture2DArray.MipSlice = m_mip_level;

					desc.Texture2DArray.FirstArraySlice = m_first_array_index;
					desc.Texture2DArray.ArraySize = m_array_size;
				}
				else
				{
					desc.ViewDimension = m_texture->multiSampleCount() > 1 ? D3D11_RTV_DIMENSION_TEXTURE2DMS : D3D11_RTV_DIMENSION_TEXTURE2D;
					
					if (m_mip_level == -1)
						desc.Texture2D.MipSlice = 0;
					else
						desc.Texture2D.MipSlice = m_mip_level;
				}

				DeviceD3D11* device_d3d11 = dynamic_cast<DeviceD3D11*>(m_device);
				if (device_d3d11)
				{
					ID3D11Device* device_ptr = (ID3D11Device*)device_d3d11->device();
					
					device_ptr->CreateRenderTargetView(m_resource, &desc, &m_render_target_view);
					m_render_target_view->GetDesc(&m_render_target_desc);
					m_mip_render_target_views.push_back(m_render_target_view);

					if (m_mip_level == -1)
					{
						for (size_t mip_level = 1; mip_level < m_texture_desc.MipLevels; mip_level++)
						{
							desc.Texture2D.MipSlice = (unsigned int)(mip_level);
							ID3D11RenderTargetView * mip_view;
							device_ptr->CreateRenderTargetView(m_resource, &desc, &mip_view);
							m_mip_render_target_views.push_back(mip_view);
						}
					}
				}
				else
				{
					//LOG("Device is empty! ");
				}
				
			}
			else
			{
				//LOG("Error, no texture to create render target view from ... ");
			}
		}

		return (m_render_target_view != 0);
	}

	unsigned int SurfaceD3D11::width() const
	{
		return m_texture_desc.Width;
	}

	unsigned int SurfaceD3D11::height() const
	{
		return m_texture_desc.Height;
	}

	const ITexture* SurfaceD3D11::texture() const
	{
		return m_texture;
	}

	ID3D11RenderTargetView* SurfaceD3D11::renderTargetView() const
	{
		return m_render_target_view;
	}

	bool SurfaceD3D11::bind(IDeviceContext* device_context, unsigned int level) const
	{
		if (device_context)
		{
			device_context->bindRenderTarget(level, this);
		}
		return true;
	}

	bool SurfaceD3D11::bindAndClear(IDeviceContext* device_context, float* color, unsigned int level) const
	{
		if (device_context)
		{
			ID3D11DeviceContext* context = (ID3D11DeviceContext*)device_context->getHandle();
			if (context)
			{
				context->ClearRenderTargetView(renderTargetView(), color);
				return true;
			}

			device_context->bindRenderTarget(level, this);
		}
		return false;
	}
}