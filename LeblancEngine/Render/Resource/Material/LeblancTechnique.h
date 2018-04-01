#ifndef LEBLANC_TECHNIQUE_H
#define LEBLANC_TECHNIQUE_H
#include <string>
#include "ThirdParty/Effect/Include/d3dx11effect.h"
using namespace std;

class DeviceD3D11;
class ID3DX11EffectTechnique;
class ID3DX11Effect;
class Technique
{
public:
	Technique(DeviceD3D11* device);
	~Technique();

	void initialize(ID3DX11Effect* effect, int index);

	void release();

	string name() { return m_name; }

private:
	// data
	ID3DX11EffectTechnique * m_technique_handle = nullptr;
	// reference
	DeviceD3D11 * m_device = nullptr;
	string m_name;
	D3DX11_TECHNIQUE_DESC m_desc;
};
#endif