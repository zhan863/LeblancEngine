#ifndef LEBLANC_CONSTANT_BUFFER_H
#define LEBLANC_CONSTANT_BUFFER_H
#include "ThirdParty/Effect/Include/d3dx11effect.h"
#include <string>
using namespace std;

class DeviceContextD3D11;
class DeviceD3D11;
class ID3DX11Effect;
class ID3DX11EffectConstantBuffer;
class ConstantBuffer
{
public:
	ConstantBuffer(DeviceD3D11* device, DeviceContextD3D11* device_context);
	~ConstantBuffer();

	void initialize(ID3DX11Effect* effect, int index);

	void release();

	string name() { return m_name; }

private:
	// data
	ID3DX11EffectConstantBuffer * m_buffer_handle = nullptr;
	// reference
	DeviceContextD3D11 * m_device_context = nullptr;
	DeviceD3D11 * m_device = nullptr;
	string m_name;
	D3DX11_EFFECT_VARIABLE_DESC m_desc;
};
#endif