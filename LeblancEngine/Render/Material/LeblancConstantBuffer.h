#ifndef LEBLANC_CONSTANT_BUFFER_H
#define LEBLANC_CONSTANT_BUFFER_H
#include <string>
using namespace std;

namespace Leblanc
{
	class IDeviceContext;
	class IDevice;

	class EffectConstantBuffer
	{
	public:
		EffectConstantBuffer(IDevice* device, IDeviceContext* device_context);
		virtual ~EffectConstantBuffer();

		string name() { return m_name; }

	protected:
		void release();

		//reference
		IDevice* m_device = nullptr;
		IDeviceContext* m_device_context = nullptr;

		string m_name;
	};

	class ConstantBuffer
	{
	public:
		ConstantBuffer(IDevice* device, IDeviceContext* device_context, int slot_index, size_t buffer_size)
			: m_device(device), m_device_context(device_context), m_slot_index(slot_index), m_buffer_size(buffer_size)
		{
		}

		virtual ~ConstantBuffer() {}

		virtual void update(void* data) = 0;

	protected:
		//Reference
		IDevice* m_device = nullptr;
		IDeviceContext* m_device_context = nullptr;

		int m_slot_index = -1;
		size_t m_buffer_size = 0;
	};
}
#endif