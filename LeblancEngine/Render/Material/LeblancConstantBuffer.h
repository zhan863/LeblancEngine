#ifndef LEBLANC_CONSTANT_BUFFER_H
#define LEBLANC_CONSTANT_BUFFER_H
#include <string>
using namespace std;

namespace Leblanc
{
	class IDeviceContext;
	class IDevice;

	class ConstantBuffer
	{
	public:
		ConstantBuffer(IDevice* device, IDeviceContext* device_context);
		virtual ~ConstantBuffer();

		string name() { return m_name; }

	protected:
		void release();

		//reference
		IDevice* m_device = nullptr;
		IDeviceContext* m_device_context = nullptr;

		string m_name;
	};
}
#endif