#include "LeblancEngine/Render/Material/D3D11/LeblancTechniqueD3D11.h"
#include "LeblancEngine/Render/RenderState/D3D11/LeblancRenderStateD3D11.h"
#include "LeblancEngine/Render/Buffer/D3D11/LeblancBufferD3D11.h"
#include "LeblancEngine/Render/Device/D3D11/LeblancDeviceD3D11.h"
#include "LeblancEngine/Render/Device/D3D11/LeblancDeviceContextD3D11.h"

#include "LeblancEngine/BasicInclude/LeblancMemoryOperation.h"

namespace Leblanc
{
	TechniqueD3D11::TechniqueD3D11(IDevice* device, IDeviceContext* device_context) :
		ITechnique(device, device_context)
	{

	}

	TechniqueD3D11::~TechniqueD3D11()
	{
		release();
	}

	void TechniqueD3D11::release()
	{
		safe_Release(m_technique_handle);
	}

	void TechniqueD3D11::initialize(ID3DX11Effect* effect, int index)
	{
		if (effect)
		{
			release();

			m_technique_handle = effect->GetTechniqueByIndex(index);

			m_technique_handle->GetDesc(&m_desc);
			m_name = m_desc.Name;

			for (int i = 0; i < m_desc.Passes; i++)
			{
				InputLayoutCacheD3D11* layout_cache = new InputLayoutCacheD3D11(m_device, m_device_context);
				layout_cache->initialize(m_technique_handle, i);
				m_input_layout_caches.push_back(layout_cache);
			}
		}
	}

	void TechniqueD3D11::bindInputLayout(int index, const IVertexDeclaration* vertex_declaration)
	{
		if (index >= 0 && index < m_input_layout_caches.size())
		{
			if (m_input_layout_caches[index])
			{
				const VertexDeclarationD3D11* vertex_declaration_d3d11 = dynamic_cast<const VertexDeclarationD3D11 *>(vertex_declaration);
				if (vertex_declaration_d3d11)
				{
					m_input_layout_caches[index]->bindLayout(vertex_declaration_d3d11);
				}
			}
		}
	}

	void TechniqueD3D11::apply(int index)
	{
		if (index >= 0)
		{
			ID3DX11EffectPass* pass = m_technique_handle->GetPassByIndex(index);
			if (pass)
			{
				pass->Apply(0, static_cast<ID3D11DeviceContext*>(m_device_context->getHandle()));
			}
		}
	}
}