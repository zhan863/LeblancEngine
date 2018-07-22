#include "LeblancEngine/Render/Material/D3D11/LeblancShadersD3D11.h"
#include "LeblancEngine/Render/Material/D3D11/LeblancTechniqueD3D11.h"
#include "LeblancEngine/Render/Material/D3D11/LeblancGpuVariableD3D11.h"
#include "LeblancEngine/Render/Material/D3D11/LeblancConstantBufferD3D11.h"
#include "LeblancEngine/Render/Device/LeblancDevice.h"

#include "LeblancEngine/BasicInclude/LeblancMemoryOperation.h"
#include "ThirdParty/Effect/Include/d3dx11effect.h"

namespace Leblanc
{
	ShaderD3D11::ShaderD3D11(IDevice* device, IDeviceContext* device_context) :
		IShader(device, device_context)
	{

	}

	ShaderD3D11::~ShaderD3D11()
	{
		release();
	}

	void ShaderD3D11::initialize(string file_name, string include_file_name)
	{
		setFileName(file_name);
		setIncludeName(include_file_name);

		release();

		std::wstring w_file_name = std::wstring(file_name.begin(), file_name.end());
		std::wstring w_include_file_name = std::wstring(include_file_name.begin(), include_file_name.end());
		ID3DBlob* error_message = nullptr;
		if (SUCCEEDED(D3DX11CompileEffectFromFile(w_file_name.c_str(), nullptr, nullptr, D3D10_SHADER_ENABLE_BACKWARDS_COMPATIBILITY, 0, (ID3D11Device*)m_device->getHandle(), &m_effect_handle, &error_message)))
		{
			D3DX11_EFFECT_DESC desc;
			if (SUCCEEDED(m_effect_handle->GetDesc(&desc)))
			{
				enumerateVariables(desc);
				enumerateTechniques(desc);
			}
		}
	}

	void ShaderD3D11::enumerateTechniques(D3DX11_EFFECT_DESC& desc)
	{
		for (int i = 0; i < desc.Techniques; i++)
		{
			TechniqueD3D11* technique = new TechniqueD3D11(m_device, m_device_context);
			technique->initialize(m_effect_handle, i);

			m_techniques.insert(make_pair(technique->name(), technique));
		}
	}

	void ShaderD3D11::enumerateVariables(D3DX11_EFFECT_DESC& desc)
	{
		for (int i = 0; i < desc.GlobalVariables; i++)
		{
			GpuVariableD3D11* gpu_variable = new GpuVariableD3D11(m_device, m_device_context);
			gpu_variable->initialize(m_effect_handle, i);

			m_gpu_variables.insert(make_pair(gpu_variable->name(), gpu_variable));
		}

		for (int i = 0; i < desc.ConstantBuffers; i++)
		{
			ConstantBufferD3D11* constant_buffer = new ConstantBufferD3D11(m_device, m_device_context);
			constant_buffer->initialize(m_effect_handle, i);

			// The hack here, remove the globals from the constant buffer pool since it is used to store the instance data of shader.
			if (constant_buffer->name() != "$Globals")
				m_global_constant_buffers.insert(make_pair(constant_buffer->name(), constant_buffer));
		}
	}

	void ShaderD3D11::release()
	{
		safe_Release(m_effect_handle);
	}
}