#ifndef LEBLANC_CONSTANT_BUFFER_D3D11_H
#define LEBLANC_CONSTANT_BUFFER_D3D11_H
#include "ThirdParty/Effect/Include/d3dx11effect.h"
#include "LeblancEngine/Render/Material/LeblancConstantBuffer.h"
#include <string>
using namespace std;

namespace Leblanc
{
	class IDevice;
	class IDeviceContext;

	class EffectConstantBufferD3D11 : public EffectConstantBuffer
	{
	public:
		EffectConstantBufferD3D11(IDevice* device, IDeviceContext* device_context);
		~EffectConstantBufferD3D11();

		void initialize(ID3DX11Effect* effect, int index);

		void release();

		string name() { return m_name; }

	private:
		// data
		ID3DX11EffectConstantBuffer * m_buffer_handle = nullptr;
		// reference
		string m_name;
		D3DX11_EFFECT_VARIABLE_DESC m_desc;
	};

	class ConstantBufferD3D11 : public ConstantBuffer
	{
	public:
		ConstantBufferD3D11(IDevice* device, IDeviceContext* device_context, int slot_index, size_t buffer_size) :
			ConstantBuffer(device, device_context, slot_index, buffer_size)
		{
			initialize();
		}

		~ConstantBufferD3D11()
		{
			release();
		}

		virtual void update(void* data);

	private:
		void release();

		void initialize();

		ID3D11Buffer* m_d3d11_buffer = nullptr;
	};
}
#endif