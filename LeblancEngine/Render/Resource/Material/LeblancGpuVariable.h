#ifndef LEBLANC_GPU_VARIABLES_H
#define LEBLANC_GPU_VARIABLES_H

#include "ThirdParty/Effect/Include/d3dx11effect.h"
#include <string>
using namespace std;

class DeviceD3D11;
class ID3DX11Effect;
class ID3DX11EffectVariable;
class GpuVariable
{
public:
	GpuVariable(DeviceD3D11* device);
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
	DeviceD3D11 * m_device = nullptr;
	ID3DX11Effect* m_effect = nullptr;
	string m_name;
	D3DX11_EFFECT_VARIABLE_DESC m_desc;
};
#endif