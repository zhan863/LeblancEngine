#include "LeblancEngine/Render/Material/D3D11/LeblancGpuVariableD3D11.h"

#include "LeblancEngine/BasicInclude/LeblancMemoryOperation.h"

namespace Leblanc
{
	GpuVariableD3D11::GpuVariableD3D11(IDevice* device, IDeviceContext* device_context) :
		IGpuVariable(device, device_context)
	{

	}

	GpuVariableD3D11::~GpuVariableD3D11()
	{
		release();
	}

	void GpuVariableD3D11::initialize(ID3DX11Effect* effect, int index)
	{
		if (effect)
		{
			release();

			m_variable_handle = effect->GetVariableByIndex(index);

			m_variable_handle->GetDesc(&m_desc);
			m_name = m_desc.Name;
		}
	}

	void GpuVariableD3D11::release()
	{
		safe_Release(m_variable_handle);
	}

	void GpuVariableD3D11::setMatrix(const float* matrix)
	{
		m_variable_handle->AsMatrix()->SetMatrix(matrix);
	}

	void GpuVariableD3D11::setVector(const float* vector)
	{
		m_variable_handle->AsVector()->SetFloatVector(vector);
	}

	void GpuVariableD3D11::setScalar(const float* value)
	{
		m_variable_handle->AsScalar()->SetFloat(*value);
	}

	void GpuVariableD3D11::setMatrixArray(const float* matrix, int count)
	{
		m_variable_handle->AsMatrix()->SetMatrixArray(matrix, 0, count);
	}

	void GpuVariableD3D11::setVectorArray(const float* vector, int count)
	{
		m_variable_handle->AsVector()->SetFloatVectorArray(vector, 0, count);
	}

	void GpuVariableD3D11::setScalarArray(const float* value, int count)
	{
		m_variable_handle->AsScalar()->SetFloatArray(value, 0, count);
	}
}