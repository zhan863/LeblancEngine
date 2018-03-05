#include "LeblancEngine/Render/RenderEntity/LeblancRenderEntity.h"
#include "LeblancEngine/Render/Resource/ResourceLoader/LeblancResourceLoader.h"
#include "LeblancEngine/Render/Basics/LeblancGeometry.h"
#include "LeblancEngine/Render/Resource/Material/LeblancMaterial.h"
#include "LeblancEngine/Global/LeblancGlobalContext.h"
#include "LeblancEngine/Render/Resource/Material/LeblancMaterialManager.h"

RenderEntity::RenderEntity()
{

}

RenderEntity::~RenderEntity()
{
}

void RenderEntity::release()
{
	for (UINT i = 0; i < m_meshes.size(); i++)
	{
		if (m_meshes[i])
		{
			m_meshes[i]->release();
		}
	}

	m_meshes.clear();
}