#include "LeblancEngine/Render/Resource/Material/LeblancShaders.h"
#include "LeblancEngine/Render/Resource/Material/LeblancTechnique.h"
#include "LeblancEngine/Render/Resource/Material/LeblancConstantBuffer.h"
#include "LeblancEngine/Render/Resource/Material/LeblancGpuVariable.h"

#include "LeblancEngine/Render/Utility/LeblancDeviceD3D11.h"

#include "LeblancEngine/BasicInclude/LeblancMemoryOperation.h"
#include "ThirdParty/Effect/Include/d3dx11effect.h"

Shader::Shader(DeviceD3D11* device) : m_device(device)
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
	if (SUCCEEDED(D3DX11CompileEffectFromFile(w_file_name.c_str(), nullptr, nullptr, D3D10_SHADER_ENABLE_BACKWARDS_COMPATIBILITY, 0, m_device->getD3D11Device(), &m_effect_handle, &error_message)))
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
		Technique* technique = new Technique(m_device);
		technique->initialize(m_effect_handle, i);

		m_techniques.insert(make_pair(technique->name(), technique));
	}
}

void Shader::enumerateVariables(D3DX11_EFFECT_DESC& desc)
{
	for (int i = 0; i < desc.GlobalVariables; i++)
	{
		GpuVariable* gpu_variable = new GpuVariable(m_device);
		gpu_variable->initialize(m_effect_handle, i);

		m_gpu_variables.insert(make_pair(gpu_variable->name(), gpu_variable));
	}

	for (int i = 0; i < desc.ConstantBuffers; i++)
	{
		ConstantBuffer* constant_buffer = new ConstantBuffer(m_device);
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

void Shader::setMatrix(string name, const float* matrix)
{
	map<string, GpuVariable*>::iterator gpu_variable = m_gpu_variables.find(name);
	if (gpu_variable != m_gpu_variables.end());
	{
		gpu_variable->second->setMatrix(matrix);
	}
}

void Shader::setVector(string name, const float* vector)
{
	map<string, GpuVariable*>::iterator gpu_variable = m_gpu_variables.find(name);
	if (gpu_variable != m_gpu_variables.end());
	{
		gpu_variable->second->setVector(vector);
	}
}

void Shader::setScalar(string name, const float* value)
{
	map<string, GpuVariable*>::iterator gpu_variable = m_gpu_variables.find(name);
	if (gpu_variable != m_gpu_variables.end());
	{
		gpu_variable->second->setScalar(value);
	}
}

void Shader::setMatrixArray(string name, const float* matrix, int count)
{
	map<string, GpuVariable*>::iterator gpu_variable = m_gpu_variables.find(name);
	if (gpu_variable != m_gpu_variables.end());
	{
		gpu_variable->second->setMatrixArray(matrix, count);
	}
}

void Shader::setVectorArray(string name, const float* vector, int count)
{
	map<string, GpuVariable*>::iterator gpu_variable = m_gpu_variables.find(name);
	if (gpu_variable != m_gpu_variables.end());
	{
		gpu_variable->second->setVectorArray(vector, count);
	}
}

void Shader::setScalarArray(string name, const float* value, int count)
{
	map<string, GpuVariable*>::iterator gpu_variable = m_gpu_variables.find(name);
	if (gpu_variable != m_gpu_variables.end());
	{
		gpu_variable->second->setScalarArray(value, count);
	}
}

































VertexShader::VertexShader()
{

}

VertexShader::~VertexShader()
{

}

void VertexShader::initialize()
{

}

void VertexShader::release()
{
	if (m_vertex_shader)
	{
		m_vertex_shader->Release();
		m_vertex_shader = nullptr;
	}

	if (m_blob)
	{
		m_blob->Release();
		m_blob = nullptr;
	}
}

void VertexShader::setShader(void* shader)
{
	m_vertex_shader = (ID3D11VertexShader*)shader;
}

PixelShader::PixelShader()
{

}

PixelShader::~PixelShader()
{

}

void PixelShader::initialize()
{

}

void PixelShader::release()
{
	if (m_pixel_shader)
	{
		m_pixel_shader->Release();
		m_pixel_shader = nullptr;
	}

	if (m_blob)
	{
		m_blob->Release();
		m_blob = nullptr;
	}
}

void PixelShader::setShader(void* shader)
{
	m_pixel_shader = (ID3D11PixelShader*)shader;
}

ComputeShader::ComputeShader()
{

}

ComputeShader::~ComputeShader()
{

}

void ComputeShader::initialize()
{

}

void ComputeShader::release()
{
	if (m_compute_shader)
	{
		m_compute_shader->Release();
		m_compute_shader = nullptr;
	}

	if (m_blob)
	{
		m_blob->Release();
		m_blob = nullptr;
	}
}

void ComputeShader::setShader(void* shader)
{
	m_compute_shader = (ID3D11ComputeShader*)shader;
}