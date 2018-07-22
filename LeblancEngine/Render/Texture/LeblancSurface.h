#ifndef LEBLANC_ISURFACE_H
#define LEBLANC_ISURFACE_H

namespace Leblanc
{
	class ITexture;
	class IDevice;
	class IDeviceContext;
	class ISurface
	{
	public:
		ISurface(IDevice* device);
		virtual ~ISurface();

		virtual bool initialize(int firstLevel = 0, int numberOfLevels = 1, ITexture* texture = 0, int mipLevel = -1) = 0;

		// bind to the specified surface
		virtual bool bind(IDeviceContext* device_context, unsigned int level = 0) const = 0;
		virtual bool bindAndClear(IDeviceContext* device_context, float* color, unsigned int level = 0) const = 0;

		virtual unsigned int width() const = 0;
		virtual unsigned int height() const = 0;
		virtual const ITexture* texture() const = 0;

	protected:
		void release();

		// reference
		IDevice* m_device = nullptr;
		ITexture* m_texture = nullptr;
	};
}
#endif