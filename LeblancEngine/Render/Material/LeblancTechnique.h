#ifndef LEBLANC_TECHNIQUE_H
#define LEBLANC_TECHNIQUE_H
#include <string>
using namespace std;

namespace Leblanc
{
	class IVertexDeclaration;
	class IDevice;
	class IDeviceContext;
	class ITechnique
	{
	public:
		ITechnique(IDevice* device, IDeviceContext* device_context);
		virtual ~ITechnique();

		string name() { return m_name; }

		virtual void bindInputLayout(int index, const IVertexDeclaration* vertex_declaration) = 0;

		virtual void apply(int index) = 0;

	protected:
		void release();

		// reference
		IDeviceContext* m_device_context = nullptr;
		IDevice* m_device = nullptr;
		string m_name;
	};
}
#endif