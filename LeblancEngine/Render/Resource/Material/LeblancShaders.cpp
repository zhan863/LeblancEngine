#include "LeblancEngine/Render/Resource/Material/LeblancShaders.h"
#include "LeblancEngine/Render/Resource/Material/LeblancTechnique.h"
#include "LeblancEngine/Render/Resource/Material/LeblancConstantBuffer.h"
#include "LeblancEngine/Render/Resource/Material/LeblancGpuVariable.h"

#include "LeblancEngine/Render/Utility/LeblancDeviceContextD3D11.h"
#include "LeblancEngine/Render/Utility/LeblancDeviceD3D11.h"

#include "LeblancEngine/BasicInclude/LeblancMemoryOperation.h"
#include "ThirdParty/Effect/Include/d3dx11effect.h"

Shader::Shader(DeviceD3D11* device, DeviceContextD3D11* device_context) :
	m_device(device), m_device_context(device_context)
{

}

Shader::~Shader()
{
	release();
}

void Shader::initialize(string file_name, string include_file_name)
{
	setFileName(file_name);
	setIncludeName(include_file_name);

	release();

	std::wstring w_file_name = std::wstring(file_name.begin(), file_name.end());
	std::wstring w_include_file_name = std::wstring(include_file_name.begin(), include_file_name.end());
	ID3DBlob* error_message = nullptr;
	if (SUCCEEDED(D3DX11CompileEffectFromFile(w_file_name.c_str(), nullptr, nullptr, D3D10_SHADER_ENABLE_BACKWARDS_COMPATIBILITY, 0, m_device->getHandle(), &m_effect_handle, &error_message)))
	{
		D3DX11_EFFECT_DESC desc;
		if (SUCCEEDED(m_effect_handle->GetDesc(&desc)))
		{
			enumerateVariables(desc);
			enumerateTechniques(desc);
		}
	}
}

void Shader::enumerateTechniques(D3DX11_EFFECT_DESC& desc)
{
	for (int i = 0; i < desc.Techniques; i++)
	{
		Technique* technique = new Technique(m_device, m_device_context);
		technique->initialize(m_effect_handle, i);

		m_techniques.insert(make_pair(technique->name(), technique));
	}
}

void Shader::enumerateVariables(D3DX11_EFFECT_DESC& desc)
{
	for (int i = 0; i < desc.GlobalVariables; i++)
	{
		GpuVariable* gpu_variable = new GpuVariable(m_device, m_device_context);
		gpu_variable->initialize(m_effect_handle, i);

		m_gpu_variables.insert(make_pair(gpu_variable->name(), gpu_variable));
	}

	for (int i = 0; i < desc.ConstantBuffers; i++)
	{
		ConstantBuffer* constant_buffer = new ConstantBuffer(m_device, m_device_context);
		constant_buffer->initialize(m_effect_handle, i);

		// The hack here, remove the globals from the constant buffer pool since it is used to store the instance data of shader.
		if (constant_buffer->name() != "$Globals")
			m_global_constant_buffers.insert(make_pair(constant_buffer->name(), constant_buffer));
	}
}

void Shader::release()
{
	for each (auto iter in m_techniques)
	{
		safe_delete(iter.second);
	}
	m_techniques.clear();

	for each (auto iter in m_gpu_variables)
	{
		safe_delete(iter.second);
	}
	m_gpu_variables.clear();

	for each (auto iter in m_global_constant_buffers)
	{
		safe_delete(iter.second);
	}
	m_global_constant_buffers.clear();

	safe_Release(m_effect_handle);
}

Technique* Shader::getTechnique(string name)
{
	map<string, Technique*>::iterator technique = m_techniques.find(name);
	if (technique != m_techniques.end())
	{
		return technique->second;
	}

	return nullptr;
}

void Shader::setGpuData(string name, const GpuData* gpu_data)
{
	if (!gpu_data)
		return;

	map<string, GpuVariable*>::iterator gpu_variable = m_gpu_variables.find(name);
	if (gpu_variable != m_gpu_variables.end())
	{
		switch(gpu_data->m_type)
		{
		case GpuDataType::FLOAT: gpu_variable->second->setScalar(static_cast<const float*>(gpu_data->m_data)); break;
		case GpuDataType::FLOAT4: gpu_variable->second->setVector(static_cast<const float*>(gpu_data->m_data)); break;
		case GpuDataType::MATRIX: gpu_variable->second->setMatrix(static_cast<const float*>(gpu_data->m_data)); break;
		case GpuDataType::FLOAT_ARRAY: gpu_variable->second->setScalarArray(static_cast<const float*>(gpu_data->m_data), gpu_data->m_count); break;
		case GpuDataType::FLOAT4_ARRAY: gpu_variable->second->setVectorArray(static_cast<const float*>(gpu_data->m_data), gpu_data->m_count); break;
		case GpuDataType::MATRIX_ARRAY: gpu_variable->second->setMatrixArray(static_cast<const float*>(gpu_data->m_data), gpu_data->m_count); break;
		}
	}
}