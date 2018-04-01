#include "LeblancEngine/Render/Resource/Material/LeblancTechnique.h"
#include "LeblancEngine/Render/Utility/LeblancDeviceD3D11.h"

#include "LeblancEngine/BasicInclude/LeblancMemoryOperation.h"

Technique::Technique(DeviceD3D11* device) : m_device(device)
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
	}
}

void Technique::release()
{
	safe_Release(m_technique_handle);
}