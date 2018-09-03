#ifndef LEBLANC_SHADER_H
#define LEBLANC_SHADER_H
#include "LeblancEngine/BasicInclude/LeblancPCH.h"
#include <map>

using namespace std;

class IDeviceContext;
class IDevice;

namespace Leblanc
{
	class IDevice;
	class IDeviceContext;
	class ITechnique;
	class GpuData;
	class IGpuVariable;
	class EffectConstantBuffer;

	class IShader
	{
	public:
		IShader(IDevice* device, IDeviceContext* device_context);
		virtual ~IShader();
		
		virtual void initialize(string file_name) = 0;

		void setFileName(string file_name) { m_file_name = file_name; }

		std::string getFileName() { return m_file_name; }

		void setGpuData(string name, const GpuData* gpu_data);

		ITechnique* getTechnique(string name) const;

	protected:
		void release();

		// data
		std::map<string, ITechnique*> m_techniques;
		std::map<string, IGpuVariable*> m_gpu_variables;
		std::map<string, EffectConstantBuffer*> m_global_constant_buffers;

		// reference
		std::string m_file_name;

		IDeviceContext* m_device_context = nullptr;
		IDevice* m_device = nullptr;
	};
}
#endif