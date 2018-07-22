#ifndef LEBLANC_GPU_VARIABLES_D3D11_H
#define LEBLANC_GPU_VARIABLES_D3D11_H

#include "LeblancEngine/Render/Material/LeblancGpuVariable.h"
#include "ThirdParty/Effect/Include/d3dx11effect.h"

#include <string>
using namespace std;

namespace Leblanc
{
	class GpuVariableD3D11 : public IGpuVariable
	{
	public:
		GpuVariableD3D11(IDevice* device, IDeviceContext* device_context);
		virtual ~GpuVariableD3D11();

		void initialize(ID3DX11Effect* effect, int index);

		virtual void setMatrix(const float* matrix);

		virtual void setVector(const float* vector);

		virtual void setScalar(const float* value);

		virtual void setMatrixArray(const float* matrix, int count);

		virtual void setVectorArray(const float* vector, int count);

		virtual void setScalarArray(const float* value, int count);

	protected:
		void release();

		// data
		ID3DX11EffectVariable * m_variable_handle = nullptr;
		// reference
		ID3DX11Effect* m_effect = nullptr;
		D3DX11_EFFECT_VARIABLE_DESC m_desc;
	};
}
#endif