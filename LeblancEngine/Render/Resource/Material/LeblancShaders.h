#ifndef LEBLANC_SHADER_H
#define LEBLANC_SHADER_H
#include "LeblancEngine/BasicInclude/LeblancPCH.h"
#include <map>

using namespace std;

class GpuData;
class Technique;
class GpuVariable;
class ConstantBuffer;
class DeviceContextD3D11;
class DeviceD3D11;
class Shader
{
public:
	Shader(DeviceD3D11* device, DeviceContextD3D11* device_context);
	~Shader();

	void release();
	void initialize(string file_name, string include_file_name);

	void setIncludeName(string include_name) { m_include_name = include_name; }
	void setFileName(string file_name) { m_file_name = file_name; }

	std::string getIncludeName() { return m_include_name; }
	std::string getFileName() { return m_file_name; }

	void setGpuData(string name, const GpuData* gpu_data);

	Technique* getTechnique(string name);

private:
	void enumerateVariables(D3DX11_EFFECT_DESC& desc);
	void enumerateTechniques(D3DX11_EFFECT_DESC& desc);

	// data
	std::map<string, Technique*> m_techniques;
	std::map<string, GpuVariable*> m_gpu_variables;
	std::map<string, ConstantBuffer*> m_global_constant_buffers;

	ID3DX11Effect* m_effect_handle = nullptr;

	// reference
	std::string m_file_name;
	std::string m_include_name;

	DeviceContextD3D11* m_device_context = nullptr;
	DeviceD3D11* m_device = nullptr;
};
#endif