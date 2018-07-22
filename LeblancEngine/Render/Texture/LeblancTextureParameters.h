#ifndef LEBLANC_TEXTURE_PARAMETERS_H
#define LEBLANC_TEXTURE_PARAMETERS_H

#include <string>
#include <vector>
#include "LeblancEngine/Math/LeblancMath.h"
#include "LeblancEngine/Render/Defines/LeblancRenderBasicDefine.h"
#include "LeblancEngine/Render/Buffer/LeblancBuffer.h"

namespace Leblanc
{
	class TextureParameters : public ResourceDeclaration
	{
	public:
		TextureParameters();

		TextureParameters(const std::string& filename,
			TextureDimension   dimension,
			TextureType        type = FromFile,
			PixelFormat        format = PF_A8R8G8B8,
			const Vector3i&	   bounds = Vector3i(0, 0, 0),
			bool               generate_mip_maps = false,
			int                texture_count = 1,
			int                multi_sample_count = 1,
			int                multi_sample_quality = 0,
			int				   array_size = 1,
			int                mip_levels = 1,
			bool               is_cube_map = false,
			bool               use_uav = false);

		TextureParameters(const std::vector<std::string>& filenames,
			TextureDimension   dimension,
			TextureType        type = FromFile,
			PixelFormat        format = PF_A8R8G8B8,
			const Vector3i&    bounds = Vector3i(0, 0, 0),
			bool               generate_mip_maps = false,
			int                texture_count = 1,
			int                multi_sample_count = 1,
			int                multi_sample_quality = 0,
			int				   array_size = 1,
			int                mip_levels = 1,
			bool               is_cube_map = false,
			bool               use_uav = false);

		TextureParameters(const TextureParameters& data);
		
		virtual ~TextureParameters();

		const std::vector<std::string>&    filenames() const;

		unsigned int                width() const;
		unsigned int                height() const;
		unsigned int                depth() const;
		PixelFormat             format() const;
		void                    setFormat(PixelFormat format);

		int                     type() const;

		TextureDimension        dimension() const;

		bool                    generateMipMaps() const;

		int                     multiSampleCount() const;

		int                     multiSampleQuality() const;

		size_t                  mipLevels() const;
		size_t                  arraySize() const;
		bool                    useUAV() const;
		bool					isCubeMap() const;

		void                    setWidth(unsigned int width);
		void                    setHeight(unsigned int height);
		void                    setDepth(unsigned int depth);
		void                    setNumMipLevels(size_t);

	private:
		std::vector<std::string> m_file_names;
		PixelFormat              m_format = PF_A8R8G8B8;
		TextureDimension         m_dimension = TwoD;
		TextureType              m_type = TextureType::Procedural;
		size_t                   m_mip_levels = 1;
		size_t					 m_array_size = 1;
		Vector3i				 m_resolution = Vector3i(0, 0, 0);

		int                      m_multi_sample_count = 1;
		int                      m_multi_sample_quality = 0;
		bool                     m_generate_mip_maps = false;
		bool                     m_use_uav = false;
		bool					 m_is_cube_map = false;
	};

	class DepthSurfaceParameters
	{
	public:
		DepthSurfaceParameters(PixelFormat format = PF_DEPTH32,
			unsigned int			width = 0,
			unsigned int			height = 0,
			bool				create_from_back_buffer = true,
			int					multi_sample_count = 1,
			int					multi_sample_quality = 0,
			bool				bindable = false);

		DepthSurfaceParameters(const DepthSurfaceParameters& data);

		virtual ~DepthSurfaceParameters();

		unsigned int                width() const;
		unsigned int                height() const;

		PixelFormat             format() const;

		bool                    createFromBackBuffer() const;

		int                     multiSampleQuality() const;

		int                     multiSampleCount() const;

		bool                    bindable() const;

	private:
		unsigned int                m_width;
		unsigned int                m_height;
		PixelFormat             m_format;
		bool                    m_create_from_back_buffer;
		int                     m_multi_sample_quality;
		int                     m_multi_sample_count;
		bool                    m_bindable;
	};
}
#endif