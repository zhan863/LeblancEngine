#ifndef LEBLANC_ITEXTURE_H
#define LEBLANC_ITEXTURE_H

#include "LeblancEngine/Render/Defines/LeblancRenderBasicDefine.h"
#include "LeblancEngine/Render/Texture/LeblancTextureParameters.h"

namespace Leblanc
{
	class TextureParameters;
	class IDevice;

	class ITexture
	{
	public:
		ITexture(IDevice* device);
		virtual ~ITexture();

		virtual bool initialize(const TextureParameters* texture_parameters) = 0;

		virtual unsigned int width() const;

		virtual unsigned int height() const;

		virtual unsigned int depth() const;
		
		virtual unsigned int arraySize() const;

		virtual unsigned int multiSampleCount() const;

		virtual unsigned int multiSampleQuality() const;

		virtual PixelFormat format() const;

		virtual bool isCubemap() const;

		virtual bool valid() const;

	protected:
		void release();

		// reference
		unsigned int m_width = 0;
		unsigned int m_height = 0;
		unsigned int m_depth = 0;
		PixelFormat m_format = PF_FLOAT16_RGBA;
		TextureParameters m_texture_parameters;

		bool m_valid = false;

		IDevice* m_device = nullptr;
	};
}
#endif