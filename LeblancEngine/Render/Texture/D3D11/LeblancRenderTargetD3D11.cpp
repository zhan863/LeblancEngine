#include "LeblancEngine/Render/Texture/D3D11/LeblancRenderTargetD3D11.h"
#include "LeblancEngine/Render/Defines/D3D11/LeblancRenderUtilityD3D11.h"
#include "LeblancEngine/Render/Device/LeblancDevice.h"
#include "LeblancEngine/Render/Texture/LeblancTextureParameters.h"
#include "LeblancEngine/Render/Texture/D3D11/LeblancSurfaceD3D11.h"
#include "LeblancEngine/BasicInclude/LeblancMemoryOperation.h"

namespace Leblanc
{
	RenderTargetTextureD3D11::RenderTargetTextureD3D11(IDevice* device) : TextureD3D11(device)
	{

	}

	RenderTargetTextureD3D11::~RenderTargetTextureD3D11()
	{
		release();
	}

	void RenderTargetTextureD3D11::release()
	{
		safe_delete(m_surface);

		for (int i = 0; i < m_array_slice_views.size(); i++)
		{
			safe_Release(m_array_slice_views[i]);
		}
		m_array_slice_views.clear();

		for (int i = 0; i < m_array_slice_surfaces.size(); i++)
		{
			safe_delete(m_array_slice_surfaces[i]);
		}
		m_array_slice_surfaces.clear();

		for (int i = 0; i < m_mip_slice_views.size(); i++)
		{
			safe_Release(m_mip_slice_views[i]);
		}
		m_mip_slice_views.clear();

		for (int i = 0; i < m_mip_slice_surfaces.size(); i++)
		{
			safe_delete(m_mip_slice_surfaces[i]);
		}
		m_mip_slice_surfaces.clear();

		for each (auto mip_slice_views in m_array_mip_slice_views)
		{
			for (int i = 0; i < mip_slice_views.second.size(); i++)
			{
				safe_Release(mip_slice_views.second[i]);
			}
			mip_slice_views.second.clear();
		}
		m_array_mip_slice_views.clear();

		for each (auto mip_slice_surfaces in m_array_mip_slice_surfaces)
		{
			for (int i = 0; i < mip_slice_surfaces.second.size(); i++)
			{
				safe_delete(mip_slice_surfaces.second[i]);
			}
			mip_slice_surfaces.second.clear();
		}
		m_array_mip_slice_surfaces.clear();
	}

	bool RenderTargetTextureD3D11::bindSurface(IDeviceContext* device_context, int render_target_index) const
	{
		if (m_active_array_index >= 0 && m_active_mip_index == -1)
		{
			return m_array_slice_surfaces[m_active_array_index]->bind(device_context, render_target_index);
		}
		else if (m_active_mip_index >= 0 && m_active_array_index == -1)
		{
			return m_mip_slice_surfaces[m_active_mip_index]->bind(device_context, render_target_index);
		}
		else if (m_active_mip_index >= 0 && m_active_array_index >= 0)
		{
			return m_array_mip_slice_surfaces.at(m_active_array_index)[m_active_mip_index]->bind(device_context, render_target_index);
		}
		else
		{
			return m_surface->bind(device_context, render_target_index);
		}
	}

	bool RenderTargetTextureD3D11::bindClearSurface(IDeviceContext* device_context, uint32_t render_target_index, float* color)
	{
		if (m_active_array_index >= 0 && m_active_mip_index == -1)
		{
			return m_array_slice_surfaces[m_active_array_index]->bindAndClear(device_context, color, render_target_index);
		}
		else if (m_active_mip_index >= 0 && m_active_array_index == -1)
		{
			return m_mip_slice_surfaces[m_active_mip_index]->bindAndClear(device_context, color, render_target_index);
		}
		else if (m_active_mip_index >= 0 && m_active_array_index >= 0)
		{
			return m_array_mip_slice_surfaces.at(m_active_array_index)[m_active_mip_index]->bindAndClear(device_context, color, render_target_index);
		}
		else
		{
			return m_surface->bindAndClear(device_context, color, render_target_index);
		}
	}

	const ISurface* RenderTargetTextureD3D11::surface(int32_t array_id, int32_t mip_id) const
	{
		if (array_id >= 0 && mip_id == -1)
		{
			return m_array_slice_surfaces[array_id];
		}
		else if (mip_id >= 0 && array_id == -1)
		{
			return m_mip_slice_surfaces[mip_id];
		}
		else if (mip_id >= 0 && array_id >= 0)
		{
			return m_array_mip_slice_surfaces.at(array_id)[mip_id];
		}
		else
		{
			return m_surface;
		}
	}

	ID3D11ShaderResourceView* RenderTargetTextureD3D11::resourceView(int32_t array_id, int32_t mip_id) const
	{
		if (array_id >= 0 && mip_id == -1)
		{
			return m_array_slice_views[array_id];
		}
		else if (mip_id >= 0 && array_id == -1)
		{
			return m_mip_slice_views[mip_id];
		}
		else if (mip_id >= 0 && array_id >= 0)
		{
			return m_array_mip_slice_views.at(array_id)[mip_id];
		}
		else
		{
			return m_shader_resource_view;
		}
	}

	bool RenderTargetTextureD3D11::create()
	{
		D3D11_TEXTURE2D_DESC desc;
		memset(&desc, 0, sizeof(D3D11_TEXTURE2D_DESC));

		bool is_cube_map = false;
		uint32_t array_size = m_texture_parameters.arraySize();

		if (m_texture_parameters.isCubeMap())
		{
			array_size = 6;
			is_cube_map = true;
		}

		desc.ArraySize = array_size;
		desc.CPUAccessFlags = 0;
		desc.Format = findFormat(m_texture_parameters.format());

		if (m_texture_parameters.width() == MIRROR_BACK_BUFFER)
		{
			m_width = m_device->backBuffer()->width();
		}
		else if (m_texture_parameters.width() == MIRROR_BACK_BUFFER_HALF)
		{
			m_width = m_device->backBuffer()->width() / 2;
		}
		else if (m_texture_parameters.width() == MIRROR_BACK_BUFFER_QUARTER)
		{
			m_width = m_device->backBuffer()->width() / 4;
		}
		else if (m_texture_parameters.width() == MIRROR_BACK_BUFFER_EIGTH)
		{
			m_width = m_device->backBuffer()->width() / 8;
		}

		if (m_texture_parameters.height() == MIRROR_BACK_BUFFER)
		{
			m_height = m_device->backBuffer()->height();
		}
		else if (m_texture_parameters.width() == MIRROR_BACK_BUFFER_HALF)
		{
			m_height = m_device->backBuffer()->height() / 2;
		}
		else if (m_texture_parameters.width() == MIRROR_BACK_BUFFER_QUARTER)
		{
			m_height = m_device->backBuffer()->height() / 4;
		}
		else if (m_texture_parameters.width() == MIRROR_BACK_BUFFER_EIGTH)
		{
			m_height = m_device->backBuffer()->height() / 8;
		}

		desc.Width = m_width;
		desc.Height = m_height;
		desc.MipLevels = (uint32_t)(m_texture_parameters.mipLevels());

		if (is_cube_map)
		{
			desc.MiscFlags |= D3D11_RESOURCE_MISC_TEXTURECUBE;
		}

		desc.SampleDesc.Count = m_texture_parameters.multiSampleCount();
		desc.SampleDesc.Quality = m_texture_parameters.multiSampleQuality();
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		desc.Usage = D3D11_USAGE_DEFAULT;

		ID3D11Device* device = (ID3D11Device*)m_device->device();
		if (desc.SampleDesc.Count > 1)
		{
			uint32_t quality;
			device->CheckMultisampleQualityLevels(findFormat(m_texture_parameters.format()),
				desc.SampleDesc.Count, &quality);
			if (quality <= desc.SampleDesc.Quality)
			{
				// Take the highest we can get
				desc.SampleDesc.Quality = quality - 1;
			}
		}

		m_texture_desc = desc;
		//Create the texture
		ID3D11Texture2D* texture = nullptr;
		if (FAILED(device->CreateTexture2D(&desc, nullptr, &texture)))
		{
			//LOG("TextureD3D11 Error: could not create render target texture");
			return false;
		}

		if (texture)
		{
			m_resource = texture;
			ID3D11ShaderResourceView * resource_view = nullptr;
			D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
			srv_desc.Format = desc.Format;

			if (is_cube_map)
			{
				srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
				srv_desc.TextureCube.MipLevels = desc.MipLevels;
				srv_desc.TextureCube.MostDetailedMip = 0;
			}
			else if (desc.ArraySize > 1)
			{
				srv_desc.ViewDimension = (m_texture_parameters.multiSampleCount() > 1) ? D3D11_SRV_DIMENSION_TEXTURE2DMSARRAY : D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
				srv_desc.Texture2DArray.ArraySize = desc.ArraySize;
				srv_desc.Texture2DArray.FirstArraySlice = 0;
				srv_desc.Texture2DArray.MipLevels = desc.MipLevels;
				srv_desc.Texture2DArray.MostDetailedMip = 0;
			}
			else
			{
				srv_desc.Texture2D.MipLevels = desc.MipLevels;
				srv_desc.Texture2D.MostDetailedMip = 0;
				srv_desc.ViewDimension = (m_texture_parameters.multiSampleCount() > 1) ? D3D11_SRV_DIMENSION_TEXTURE2DMS : D3D11_SRV_DIMENSION_TEXTURE2D;
			}

			if (SUCCEEDED(device->CreateShaderResourceView(texture,
				&srv_desc,
				&resource_view)))
			{
				m_shader_resource_view = resource_view; /* needs index as does texture bind */

				if (!m_surface)
				{
					m_surface = new SurfaceD3D11(m_device);
					if (!m_surface->initialize(0, desc.ArraySize, this))
					{
						//LOG("Failed to create Surface!");
						assert(0);
					}
				}
			}
			else
			{
				//LOG("Failed to create shader resource view for rendertargettexture.");
				return false;
			}

			// Create resource views for each index in the array. All mips for all single images.
			srv_desc.Format = desc.Format;
			if (desc.ArraySize > 1)
			{
				srv_desc.ViewDimension = (m_texture_parameters.multiSampleCount() > 1) ? D3D11_SRV_DIMENSION_TEXTURE2DMSARRAY : D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
				srv_desc.Texture2DArray.ArraySize = desc.ArraySize;
				srv_desc.Texture2DArray.FirstArraySlice = 0;
				srv_desc.Texture2DArray.MipLevels = desc.MipLevels;
				srv_desc.Texture2DArray.MostDetailedMip = 0;

				for (uint32_t i = 0; i < desc.ArraySize; i++)
				{
					ID3D11ShaderResourceView * slice_resource_view = nullptr;
					srv_desc.Texture2DArray.ArraySize = 1;
					srv_desc.Texture2DArray.FirstArraySlice = i;

					HRESULT hr = device->CreateShaderResourceView(texture,
						&srv_desc,
						&slice_resource_view);

					if (SUCCEEDED(hr))
					{
						m_array_slice_views.push_back(slice_resource_view);
					}
					else
					{
						//                    LOG ("Failed because: " << DXGetErrorStringA(hr) << " long reason: " << DXGetErrorDescriptionA(hr));

						//                    LOG ("Failed to allocate slice resource for slice " << i);
						return false;
					}

					SurfaceD3D11* surface = new SurfaceD3D11(m_device);
					if (surface->initialize(i, 1, this))
					{
						m_array_slice_surfaces.push_back(surface);
					}
					else
					{
						// LOG("Failed to allocate slice surface for slice " << i);
						safe_delete(surface);
						return false;
					}
				}
			}

			// Resources and surfaces for all arrayIds with specified mipmap.
			for (uint32_t mipLevel = 0; mipLevel < desc.MipLevels; mipLevel++)
			{
				// Shader resource.
				ID3D11ShaderResourceView* mip_resource = nullptr;
				if (desc.ArraySize > 1)
				{
					srv_desc.ViewDimension = (m_texture_parameters.multiSampleCount() > 1) ? D3D11_SRV_DIMENSION_TEXTURE2DMSARRAY : D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
					srv_desc.Texture2DArray.ArraySize = desc.ArraySize;
					srv_desc.Texture2DArray.FirstArraySlice = 0;
					srv_desc.Texture2DArray.MipLevels = 1;
					srv_desc.Texture2DArray.MostDetailedMip = mipLevel;
				}
				else
				{
					srv_desc.ViewDimension = (m_texture_parameters.multiSampleCount() > 1) ? D3D11_SRV_DIMENSION_TEXTURE2DMS : D3D11_SRV_DIMENSION_TEXTURE2D;
					srv_desc.Texture2D.MipLevels = 1;
					srv_desc.Texture2D.MostDetailedMip = mipLevel;
				}

				if (SUCCEEDED(device->CreateShaderResourceView(texture, &srv_desc, &mip_resource)))
				{
					m_mip_slice_views.push_back(mip_resource);
				}
				else
				{
					// LOG("Failed to allocate slice resource for mip level " << mipLevel);
					return false;
				}

				// Surface / Rendertarget
				SurfaceD3D11* mip_surface = new SurfaceD3D11(m_device);
				mip_surface->initialize(0, desc.ArraySize, this, mipLevel);
				m_mip_slice_surfaces.push_back(mip_surface);
			}

			// Resources for specified arrayId with specified mipmap
			for (uint32_t array_index = 0; array_index < desc.ArraySize; array_index++)
			{
				std::vector<ID3D11ShaderResourceView*> array_mip_slice_views;
				std::vector<SurfaceD3D11*> array_mip_slice_surfaces;

				for (uint32_t mipLevel = 0; mipLevel < desc.MipLevels; mipLevel++)
				{
					// Shader resource
					ID3D11ShaderResourceView* mip_resource = nullptr;
					if (desc.ArraySize > 1)
					{

						srv_desc.ViewDimension = (m_texture_parameters.multiSampleCount() > 1) ? D3D11_SRV_DIMENSION_TEXTURE2DMSARRAY : D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
						srv_desc.Texture2DArray.ArraySize = 1;
						srv_desc.Texture2DArray.FirstArraySlice = 0;
						srv_desc.Texture2DArray.MipLevels = 1;
						srv_desc.Texture2DArray.MostDetailedMip = mipLevel;
					}
					else
					{
						srv_desc.ViewDimension = (m_texture_parameters.multiSampleCount() > 1) ? D3D11_SRV_DIMENSION_TEXTURE2DMS : D3D11_SRV_DIMENSION_TEXTURE2D;
						srv_desc.Texture2D.MipLevels = 1;
						srv_desc.Texture2D.MostDetailedMip = mipLevel;
					}

					if (SUCCEEDED(device->CreateShaderResourceView(texture, &srv_desc, &mip_resource)))
					{
						array_mip_slice_views.push_back(mip_resource);
					}
					else
					{
						// LOG("Failed to allocate slice resource for array level " << arrayId << " mip level " << mipLevel);
						return false;
					}

					// Surface / rendetarget
					SurfaceD3D11* mip_surface = new SurfaceD3D11(m_device);
					mip_surface->initialize(array_index, 1, this, mipLevel);
					array_mip_slice_surfaces.push_back(mip_surface);
				}

				m_array_mip_slice_views.insert(std::make_pair(array_index, array_mip_slice_views));
				m_array_mip_slice_surfaces.insert(std::make_pair(array_index, array_mip_slice_surfaces));
			}
		}

		return true;
	}
}