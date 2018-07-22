#ifndef LEBLANC_TECHNIQUE_D3D11_H
#define LEBLANC_TECHNIQUE_D3D11_H

#include "LeblancEngine/Render/Material/LeblancTechnique.h"
#include "ThirdParty/Effect/Include/d3dx11effect.h"
#include <vector>
using namespace std;

class ID3DX11Effect;
class ID3DX11EffectTechnique;

namespace Leblanc
{
	class IVertexDeclaration;
	class IDevice;
	class IDeviceContext;
	class InputLayoutCacheD3D11;
	class TechniqueD3D11 : public ITechnique
	{
	public:
		TechniqueD3D11(IDevice* device, IDeviceContext* device_context);
		virtual ~TechniqueD3D11();

		virtual void bindInputLayout(int index, const IVertexDeclaration* vertex_declaration);

		virtual void apply(int index);

		void initialize(ID3DX11Effect* effect, int index);

	protected:
		void release();

		// data
		ID3DX11EffectTechnique * m_technique_handle = nullptr;
		vector<InputLayoutCacheD3D11*> m_input_layout_caches;

		// reference
		D3DX11_TECHNIQUE_DESC m_desc;
	};
}
#endif