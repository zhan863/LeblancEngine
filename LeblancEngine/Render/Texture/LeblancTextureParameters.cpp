#include "LeblancEngine/Render/Texture/LeblancTextureParameters.h"

namespace Leblanc
{
	TextureParameters::TextureParameters()
	{

	}

	TextureParameters::TextureParameters(const std::vector<std::string>& filenames,
		TextureDimension                dimension,
		TextureType                     type,
		PixelFormat                     format,
		const Vector3i&					bounds,
		bool                            generateMipMaps,
		int                             textureCount,
		int                             multiSampleCount,
		int                             multiSampleQuality,
		int								array_size,
		int                             mipLevels,
		bool                            is_cube_map,
		bool                            use_uav) :
		m_file_names(filenames),
		m_format(format),
		m_dimension(dimension),
		m_resolution(bounds),
		m_type(type),
		m_generate_mip_maps(generateMipMaps),
		m_multi_sample_count(multiSampleCount),
		m_multi_sample_quality(multiSampleQuality),
		m_array_size(array_size),
		m_mip_levels(mipLevels),
		m_is_cube_map(is_cube_map),
		m_use_uav(use_uav)
	{
	};

	TextureParameters::TextureParameters(const std::string& filename,
		TextureDimension       dimension,
		TextureType            type,
		PixelFormat            format,
		const Vector3i&		   bounds,
		bool                   generateMipMaps,
		int                    textureCount,
		int                    multiSampleCount,
		int                    multiSampleQuality,
		int                    array_size,
		int                    mipLevels,
		bool                   is_cube_map,
		bool                   use_uav) :
		m_format(format),
		m_dimension(dimension),
		m_type(type),
		m_resolution(bounds),
		m_generate_mip_maps(generateMipMaps),
		m_multi_sample_count(multiSampleCount),
		m_multi_sample_quality(multiSampleQuality),
		m_array_size(array_size),
		m_mip_levels(mipLevels),
		m_is_cube_map(is_cube_map),
		m_use_uav(use_uav)
	{
		m_file_names.push_back(filename);
	};

	size_t TextureParameters::arraySize() const
	{
		return m_array_size;
	}

	size_t TextureParameters::mipLevels() const
	{
		return m_mip_levels;
	}

	bool TextureParameters::useUAV() const
	{
		return m_use_uav;
	}

	bool TextureParameters::isCubeMap() const 
	{
		return m_is_cube_map;
	}

	void TextureParameters::setNumMipLevels(size_t value)
	{
		m_mip_levels = value;
	}

	TextureParameters::TextureParameters(const TextureParameters& data)
	{
		m_file_names = data.m_file_names;
		m_format = data.m_format;
		m_dimension = data.m_dimension;
		m_type = data.m_type;
		m_generate_mip_maps = data.m_generate_mip_maps;
		m_multi_sample_count = data.m_multi_sample_count;
		m_multi_sample_quality = data.m_multi_sample_quality;
		m_array_size = data.m_array_size;
		m_mip_levels = data.m_mip_levels;
		m_use_uav = data.m_use_uav;
		m_resolution = data.m_resolution;
		m_is_cube_map = data.m_is_cube_map;
	}

	TextureParameters::~TextureParameters()
	{
	};

	const std::vector<std::string>& TextureParameters::filenames() const
	{
		return m_file_names;
	}

	PixelFormat TextureParameters::format() const 
	{
		return m_format;
	}

	void TextureParameters::setFormat(PixelFormat format) 
	{
		m_format = format;
	}

	int TextureParameters::type() const 
	{
		return m_type;
	}

	TextureDimension TextureParameters::dimension() const 
	{
		return m_dimension;
	}

	bool TextureParameters::generateMipMaps() const 
	{
		return m_generate_mip_maps;
	}

	int TextureParameters::multiSampleCount() const 
	{
		return m_multi_sample_count;
	}

	int TextureParameters::multiSampleQuality() const 
	{
		return m_multi_sample_quality;
	}

	void TextureParameters::setWidth(unsigned int width) 
	{
		m_resolution.x = width;
	}

	void TextureParameters::setHeight(unsigned int height)
	{
		m_resolution.y = height;
	}

	void TextureParameters::setDepth(unsigned int depth)
	{
		m_resolution.z = depth;
	}

	unsigned int TextureParameters::width() const
	{
		return m_resolution.x;
	}

	unsigned int TextureParameters::height() const
	{
		return m_resolution.y;
	}

	unsigned int TextureParameters::depth() const
	{
		return m_resolution.z;
	}

	DepthSurfaceParameters::DepthSurfaceParameters(PixelFormat format,
		unsigned int    width,
		unsigned int    height,
		bool        create_from_back_buffer,
		int         multi_sample_count,
		int         multi_sample_quality,
		bool        bindable) :
		m_width(width),
		m_height(height),
		m_format(format),
		m_create_from_back_buffer(create_from_back_buffer),
		m_multi_sample_count(multi_sample_count),
		m_multi_sample_quality(multi_sample_quality),
		m_bindable(bindable)
	{
	};

	DepthSurfaceParameters::DepthSurfaceParameters(const DepthSurfaceParameters & data)
	{
		m_width = data.m_width;
		m_height = data.m_height;
		m_format = data.m_format;
		m_multi_sample_count = data.m_multi_sample_count;
		m_multi_sample_quality = data.m_multi_sample_quality;
		m_create_from_back_buffer = data.m_create_from_back_buffer;
		m_bindable = data.m_bindable;
	}

	DepthSurfaceParameters::~DepthSurfaceParameters() {};

	unsigned int DepthSurfaceParameters::width() const 
	{
		return m_width;
	}

	unsigned int DepthSurfaceParameters::height() const 
	{
		return m_height;
	}

	PixelFormat DepthSurfaceParameters::format() const 
	{
		return m_format;
	}

	bool DepthSurfaceParameters::createFromBackBuffer() const 
	{
		return m_create_from_back_buffer;
	}

	int DepthSurfaceParameters::multiSampleQuality() const 
	{
		return m_multi_sample_quality;
	}

	int DepthSurfaceParameters::multiSampleCount() const 
	{
		return m_multi_sample_count;
	}

	bool DepthSurfaceParameters::bindable() const 
	{
		return m_bindable;
	}
}