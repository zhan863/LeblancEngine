#ifndef LEBLANC_SHADER_D3D11_H
#define LEBLANC_SHADER_D3D11_H
#include "LeblancEngine/BasicInclude/LeblancPCH.h"
#include "LeblancEngine/Render/Material/LeblancShaders.h"
#include <map>

using namespace std;

struct ID3DX11Effect;
namespace Leblanc
{
	class ShaderD3D11 : public IShader
	{
	public:
		ShaderD3D11(IDevice* device, IDeviceContext* device_context);
		virtual ~ShaderD3D11();

		void release();
		void initialize(string file_name, string include_file_name);

	protected:
		void enumerateVariables(D3DX11_EFFECT_DESC& desc);
		void enumerateTechniques(D3DX11_EFFECT_DESC& desc);

		ID3DX11Effect* m_effect_handle = nullptr;
	};
}
#endif