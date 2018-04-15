#ifndef LEBLANC_TECHNIQUE_H
#define LEBLANC_TECHNIQUE_H
#include <string>
#include "ThirdParty/Effect/Include/d3dx11effect.h"
#include "LeblancEngine/Render/Basics/LeblancRenderPlatformDefineD3D11.h"
using namespace std;

class DeviceContextD3D11;
class DeviceD3D11;
class ID3DX11EffectTechnique;
class ID3DX11Effect;
class InputLayoutCacheD3D11;
class VertexDeclarationD3D11;
class Technique
{
public:
	Technique(DeviceD3D11* device, DeviceContextD3D11* device_context);
	~Technique();

	void initialize(ID3DX11Effect* effect, int index);

	void release();

	string name() { return m_name; }

	void bindInputLayout(int index, const VertexDeclarationD3D11* vertex_declaration);

	void apply(int index);

private:
	// data
	ID3DX11EffectTechnique* m_technique_handle = nullptr;
	vector<InputLayoutCacheD3D11*> m_input_layout_caches;

	// reference
	DeviceContextD3D11* m_device_context = nullptr;
	DeviceD3D11* m_device = nullptr;
	string m_name;
	D3DX11_TECHNIQUE_DESC m_desc;
};
#endif