#include "LeblancEngine/Render/Resource/Material/LeblancTechnique.h"
#include "LeblancEngine/Render/Utility/LeblancDeviceContextD3D11.h"

#include "LeblancEngine/BasicInclude/LeblancMemoryOperation.h"

Technique::Technique(DeviceD3D11* device, DeviceContextD3D11* device_context) : 
	m_device(device), m_device_context(device_context)
{

}

Technique::~Technique()
{
	release();
}

void Technique::initialize(ID3DX11Effect* effect, int index)
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

void Technique::release()
{
	safe_Release(m_technique_handle);
}

void Technique::bindInputLayout(int index, const VertexDeclarationD3D11* vertex_declaration)
{
	if (index >= 0 && index < m_input_layout_caches.size())
	{
		if (m_input_layout_caches[index])
		{
			m_input_layout_caches[index]->bindLayout(vertex_declaration);
		}
	}
}

void Technique::apply(int index)
{
	if (index >= 0)
	{
		ID3DX11EffectPass* pass = m_technique_handle->GetPassByIndex(index);
		if (pass)
		{
			pass->Apply(0, m_device_context->getHandle());
		}
	}
}