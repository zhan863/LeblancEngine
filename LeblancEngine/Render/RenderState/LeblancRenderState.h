#ifndef LEBLANC_RENDER_STATE_H
#define LEBLANC_RENDER_STATE_H

namespace Leblanc
{
	class IDevice;
	class IDeviceContext;
	
	class IRasterizerState
	{
	public:
		IRasterizerState(IDevice* device, IDeviceContext* device_context) : m_device(device), m_device_context(device_context) {}
		virtual ~IRasterizerState() { release(); }
	protected:
		virtual void release() {}
		// reference
		IDevice* m_device = nullptr;
		IDeviceContext* m_device_context = nullptr;
	};

	class IBlendState
	{
	public:
		IBlendState(IDevice* device, IDeviceContext* device_context) : m_device(device), m_device_context(device_context) {}
		virtual ~IBlendState() { release(); }
	protected:
		virtual void release() {}
		// reference
		IDevice* m_device = nullptr;
		IDeviceContext* m_device_context = nullptr;
	};

	class IDepthStencilState
	{
	public:
		IDepthStencilState(IDevice* device, IDeviceContext* device_context) : m_device(device), m_device_context(device_context) {}
		virtual ~IDepthStencilState() { release(); }
	protected:
		virtual void release() {}
		// reference
		IDevice* m_device = nullptr;
		IDeviceContext* m_device_context = nullptr;
	};

	class IInputLayoutCache
	{
	public:
		IInputLayoutCache(IDevice* device, IDeviceContext* device_context) : m_device(device), m_device_context(device_context) {}
		virtual ~IInputLayoutCache() { release(); }
	protected:
		virtual void release() {}
		// reference
		IDevice* m_device = nullptr;
		IDeviceContext* m_device_context = nullptr;
	};
}

#endif