#ifndef LEBLANC_GPU_VARIABLES_H
#define LEBLANC_GPU_VARIABLES_H
#include <string>
using namespace std;

namespace Leblanc
{
	enum class GpuDataType
	{
		FLOAT = 0,
		FLOAT4 = 1,
		MATRIX = 2,
		FLOAT_ARRAY = 3,
		FLOAT4_ARRAY = 4,
		MATRIX_ARRAY = 5
	};

	class GpuData
	{
	public:
		GpuData(GpuDataType type, void* data, size_t count = 1);
		~GpuData();

		static size_t getGpuDataElementSize(GpuDataType type);

		void release();

		size_t m_size = 0;
		size_t m_count = 1;
		void* m_data = nullptr;
		GpuDataType m_type = GpuDataType::FLOAT4;
	};

	class IDevice;
	class IDeviceContext;
	class IGpuVariable
	{
	public:
		IGpuVariable(IDevice* device, IDeviceContext* device_context);
		virtual ~IGpuVariable();

		string name() { return m_name; }

		virtual void setMatrix(const float* matrix) = 0;

		virtual void setVector(const float* vector) = 0;

		virtual void setScalar(const float* value) = 0;

		virtual void setMatrixArray(const float* matrix, int count) = 0;

		virtual void setVectorArray(const float* vector, int count) = 0;

		virtual void setScalarArray(const float* value, int count) = 0;

	protected:
		void release();

		// reference
		IDeviceContext * m_device_context = nullptr;
		IDevice * m_device = nullptr;
		string m_name;
	};
}
#endif