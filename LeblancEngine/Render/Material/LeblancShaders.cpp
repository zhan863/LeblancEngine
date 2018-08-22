#include "LeblancEngine/Render/Material/LeblancShaders.h"
#include "LeblancEngine/Render/Material/LeblancTechnique.h"
#include "LeblancEngine/Render/Material/LeblancConstantBuffer.h"
#include "LeblancEngine/Render/Material/LeblancGpuVariable.h"

#include "LeblancEngine/Render/Device/LeblancDevice.h"
#include "LeblancEngine/Render/Device/LeblancDeviceContext.h"

#include "LeblancEngine/BasicInclude/LeblancMemoryOperation.h"

namespace Leblanc
{
	IShader::IShader(IDevice* device, IDeviceContext* device_context) :
		m_device(device), m_device_context(device_context)
	{

	}

	IShader::~IShader()
	{
		release();
	}

	void IShader::release()
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
	}

	ITechnique* IShader::getTechnique(string name) const
	{
		map<string, ITechnique*>::const_iterator technique = m_techniques.find(name);
		if (technique != m_techniques.end())
		{
			return technique->second;
		}

		return nullptr;
	}

	void IShader::setGpuData(string name, const GpuData* gpu_data)
	{
		if (!gpu_data)
			return;

		map<string, IGpuVariable*>::iterator gpu_variable = m_gpu_variables.find(name);
		if (gpu_variable != m_gpu_variables.end())
		{
			switch (gpu_data->m_type)
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
}