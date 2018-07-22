#include "LeblancEngine/Render/Texture/D3D11/LeblancTextureD3D11.h"
#include "LeblancEngine/BasicInclude/LeblancMemoryOperation.h"
#include "LeblancEngine/Render/Device/D3D11/LeblancDeviceD3D11.h"
#include "LeblancEngine/Render/Defines/D3D11/LeblancRenderUtilityD3D11.h"

namespace Leblanc
{
	TextureD3D11::TextureD3D11(IDevice* device) : ITexture(device)
	{

	}

	TextureD3D11::~TextureD3D11()
	{
		release();
	}

	void TextureD3D11::release()
	{
		safe_Release(m_shader_resource_view);
		safe_Release(m_resource);
	}

	bool TextureD3D11::initialize(const TextureParameters* texture_parameters)
	{
		if (!m_resource)
		{
			m_texture_parameters = *texture_parameters;
			return create();
		}
		else
		{
			// LOG(already exists!);
		}
	}

	ID3D11Resource* TextureD3D11::getResource()
	{
		return m_resource;
	}

	ID3D11ShaderResourceView* TextureD3D11::getResourceView()
	{
		return m_shader_resource_view;
	}

	bool TextureD3D11::create()
	{
		ID3D11Resource * resource = 0;
		bool staging = m_texture_parameters.type() == StagingFromFile;

		D3D11_TEXTURE2D_DESC desc;
		ID3D11Texture2D* map = nullptr;

		bool is_cube_map = false;
		bool is_texture_array = false;
		const std::vector<std::string>& filenames = m_texture_parameters.filenames();

		if (m_texture_parameters.type() != Procedural)
		{
			is_texture_array = m_texture_parameters.arraySize() > 1;

			size_t mip_map_count = m_texture_parameters.mipLevels();

			is_cube_map = m_texture_parameters.isCubeMap() && m_texture_parameters.arraySize() == 6;			


			PixelFormat format = m_texture_parameters.format();

			desc.Width = static_cast<UINT>(m_texture_parameters.width());
			desc.Height = static_cast<UINT>(m_texture_parameters.height());
			desc.MipLevels = static_cast<UINT>(mip_map_count);
			desc.ArraySize = static_cast<UINT>(m_texture_parameters.arraySize());
			desc.Format = findFormat(format);
			desc.SampleDesc.Count = m_texture_parameters.multiSampleCount();
			desc.SampleDesc.Quality = m_texture_parameters.multiSampleQuality();
			desc.Usage = staging ? D3D11_USAGE_STAGING : D3D11_USAGE_DEFAULT;
			desc.BindFlags = staging ? 0 : D3D11_BIND_SHADER_RESOURCE;
			desc.CPUAccessFlags = staging ? D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ : 0;
			desc.MiscFlags = is_cube_map ? D3D11_RESOURCE_MISC_TEXTURECUBE : 0;
		}
		else
		{
			unsigned int mipLevels = 1;

			desc.ArraySize = 1;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			desc.Format = findFormat(m_texture_parameters.format());
			desc.Width = m_texture_parameters.width();
			desc.Height = m_texture_parameters.height();
			desc.MipLevels = mipLevels;
			desc.MiscFlags = 0;
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
			desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			desc.Usage = D3D11_USAGE_DYNAMIC;
		}

		DeviceD3D11* device_d3d11 = dynamic_cast<DeviceD3D11*>(m_device);
		if (device_d3d11)
		{
			ID3D11Device* device_d3d11_handle = (ID3D11Device*)device_d3d11->device();
			if (device_d3d11_handle)
			{
				HRESULT hr = device_d3d11_handle->CreateTexture2D(&desc, nullptr, &map);

				if (SUCCEEDED(hr))
				{
					if (!staging)
					{
						D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;

						if (is_cube_map)
						{
							srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
							srv_desc.Format = desc.Format;
							srv_desc.TextureCube.MipLevels = desc.MipLevels;
							srv_desc.TextureCube.MostDetailedMip = 0;
						}
						else if (is_texture_array)
						{
							srv_desc.Format = desc.Format;
							srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
							srv_desc.Texture2DArray.MipLevels = desc.MipLevels;
							srv_desc.Texture2DArray.ArraySize = m_texture_parameters.arraySize();
							srv_desc.Texture2DArray.FirstArraySlice = 0;
						}
						else
						{
							srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
							srv_desc.Format = desc.Format;
							srv_desc.Texture2D.MipLevels = desc.MipLevels;
							srv_desc.Texture2D.MostDetailedMip = 0;
						}

						ID3D11ShaderResourceView * resource_view = nullptr;
						if (SUCCEEDED(device_d3d11_handle->CreateShaderResourceView(map,
							&srv_desc,
							&resource_view)))
						{
							m_shader_resource_view = resource_view;
							m_resource = map;
							m_valid = true;
							return true;
						}
					}
					else
					{
						m_valid = false;
						return false;
					}
				}
			}
			
			m_valid = false;
			return false;
		}
		else
		{
			//LOG("Failed to load texture " << this->_filename);
		}

		m_valid = false;
		return false;
	}
}