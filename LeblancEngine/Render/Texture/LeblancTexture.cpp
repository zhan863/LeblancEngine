#include "LeblancEngine/Render/Texture/LeblancTexture.h"

namespace Leblanc
{
	ITexture::ITexture(IDevice* device)
	{
		m_device = device;
	}

	ITexture::~ITexture()
	{

	}

	unsigned int ITexture::width() const
	{
		return m_width;
	}

	unsigned int ITexture::height() const
	{
		return m_height;
	}

	unsigned int ITexture::depth() const
	{
		return m_depth;
	}

	unsigned int ITexture::arraySize() const	
	{
		return m_texture_parameters.arraySize();
	}

	unsigned int ITexture::multiSampleCount() const
	{
		return m_texture_parameters.multiSampleCount();
	}

	unsigned int ITexture::multiSampleQuality() const
	{
		return m_texture_parameters.multiSampleQuality();
	}

	PixelFormat ITexture::format() const
	{
		return m_format;
	}

	bool ITexture::valid() const 
	{
		return m_valid;
	}

	bool ITexture::isCubemap() const
	{
		return m_texture_parameters.isCubeMap();
	}

	void ITexture::release()
	{

	}
}