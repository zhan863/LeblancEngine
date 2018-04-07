#include "LeblancEngine/Render/RenderEntity/LeblancRenderEntity.h"
#include "LeblancEngine/Render/Resource/Material/LeblancMaterial.h"
#include "LeblancEngine/Global/LeblancGlobalContext.h"
#include "LeblancEngine/Render/Resource/Material/LeblancMaterialManager.h"
#include "LeblancEngine/BasicInclude/LeblancMemoryOperation.h"

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
		safe_delete(m_meshes[i]);
	}

	m_meshes.clear();
}

void RenderEntity::addMesh(Mesh* mesh)
{
	if (mesh)
	{
		m_meshes.push_back(mesh);
	}
}