#include "LeblancEngine/Render/Resource/Material/LeblancGpuVariable.h"
#include "LeblancEngine/Render/Utility/LeblancDeviceD3D11.h"

#include "LeblancEngine/BasicInclude/LeblancMemoryOperation.h"

GpuData::GpuData(GpuDataType type, void* data, size_t count) : m_type(type)
{
	m_size = getGpuDataElementSize(type) * count;
	m_data = new char[m_size];
	m_count = count;
	memcpy(m_data, data, m_size);
}

size_t GpuData::getGpuDataElementSize(GpuDataType type)
{
	static const size_t float_size = 16;
	switch (type)
	{
	case GpuDataType::FLOAT: 
	case GpuDataType::FLOAT_ARRAY: return float_size;
	case GpuDataType::FLOAT4:
	case GpuDataType::FLOAT4_ARRAY: return 4 * float_size;
	case GpuDataType::MATRIX:
	case GpuDataType::MATRIX_ARRAY: return 4 * 4 * float_size;
	}
}

GpuData::~GpuData()
{
	release();
}

void GpuData::release()
{
	char* data = static_cast<char*>(m_data);
	safe_delete_array(data);
	m_data = nullptr;
}

GpuVariable::GpuVariable(DeviceD3D11* device, DeviceContextD3D11* device_context) :
	m_device(device), m_device_context(device_context)
{

}

GpuVariable::~GpuVariable()
{
	release();
}

void GpuVariable::initialize(ID3DX11Effect* effect, int index)
{
	if (effect)
	{
		release();

		m_variable_handle = effect->GetVariableByIndex(index);

		m_variable_handle->GetDesc(&m_desc);
		m_name = m_desc.Name;
	}
}

void GpuVariable::release()
{
	safe_Release(m_variable_handle);
}

void GpuVariable::setMatrix(const float* matrix)
{
	m_variable_handle->AsMatrix()->SetMatrix(matrix);
}

void GpuVariable::setVector(const float* vector)
{
	m_variable_handle->AsVector()->SetFloatVector(vector);
}

void GpuVariable::setScalar(const float* value)
{
	m_variable_handle->AsScalar()->SetFloat(*value);
}

void GpuVariable::setMatrixArray(const float* matrix, int count)
{
	m_variable_handle->AsMatrix()->SetMatrixArray(matrix, 0, count);
}

void GpuVariable::setVectorArray(const float* vector, int count)
{
	m_variable_handle->AsVector()->SetFloatVectorArray(vector, 0, count);
}

void GpuVariable::setScalarArray(const float* value, int count)
{
	m_variable_handle->AsScalar()->SetFloatArray(value, 0, count);
}