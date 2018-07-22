#include "LeblancEngine/Render/Material/LeblancGpuVariable.h"

#include "LeblancEngine/BasicInclude/LeblancMemoryOperation.h"

namespace Leblanc
{
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

	IGpuVariable::IGpuVariable(IDevice* device, IDeviceContext* device_context) :
		m_device(device), m_device_context(device_context)
	{

	}

	IGpuVariable::~IGpuVariable()
	{
		release();
	}

	void IGpuVariable::release()
	{
	}
}