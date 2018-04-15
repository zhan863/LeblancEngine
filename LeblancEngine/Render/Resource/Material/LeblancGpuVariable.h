#ifndef LEBLANC_GPU_VARIABLES_H
#define LEBLANC_GPU_VARIABLES_H

#include "ThirdParty/Effect/Include/d3dx11effect.h"
#include "LeblancEngine/Render/Basics/LeblancRenderPlatformIndependentDefine.h"
#include <string>
using namespace std;

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

class DeviceContextD3D11;
class DeviceD3D11;
class ID3DX11Effect;
class ID3DX11EffectVariable;
class GpuVariable
{
public:
	GpuVariable(DeviceD3D11* device, DeviceContextD3D11* device_context);
	~GpuVariable();

	void initialize(ID3DX11Effect* effect, int index);

	void release();

	string name() { return m_name; }

	void setMatrix(const float* matrix);

	void setVector(const float* vector);

	void setScalar(const float* value);

	void setMatrixArray(const float* matrix, int count);

	void setVectorArray(const float* vector, int count);

	void setScalarArray(const float* value, int count);

private:
	// data
	ID3DX11EffectVariable * m_variable_handle = nullptr;
	// reference
	DeviceContextD3D11 * m_device_context = nullptr;
	DeviceD3D11 * m_device = nullptr;
	ID3DX11Effect* m_effect = nullptr;
	string m_name;
	D3DX11_EFFECT_VARIABLE_DESC m_desc;
};
#endif