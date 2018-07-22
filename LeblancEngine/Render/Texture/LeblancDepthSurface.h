#ifndef LEBLANC_IDEPTH_SURFACE_H
#define LEBLANC_IDEPTH_SURFACE_H

namespace Leblanc
{
	class IDevice;
	class IDeviceContext;
	class DepthSurfaceParameters;
	class IDepthSurface
	{
	public:
		IDepthSurface(IDevice* device);

		virtual ~IDepthSurface();
		virtual bool initialize(const DepthSurfaceParameters*) = 0;
		virtual bool bind(IDeviceContext* device_context) const = 0;
		virtual bool clear(IDeviceContext* device_context) = 0;
		virtual bool clearStencil(IDeviceContext* device_context) = 0;
		virtual int  multiSampleCount() const = 0;
		virtual int  multiSampleQuality() const = 0;
		virtual int  width() const = 0;
		virtual int  height() const = 0;

	protected:
		void release();
		//reference
		IDevice* m_device = nullptr;
	};
}
#endif