#include "LeblancEngine/Render/Resource/Material/LeblancGpuVariable.h"
#include "LeblancEngine/Render/Utility/LeblancDeviceD3D11.h"

#include "LeblancEngine/BasicInclude/LeblancMemoryOperation.h"

GpuVariable::GpuVariable(DeviceD3D11* device) : m_device(device)
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