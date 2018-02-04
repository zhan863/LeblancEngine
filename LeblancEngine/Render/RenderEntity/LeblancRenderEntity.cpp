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
	release();
}

void RenderEntity::createFromFile(const WCHAR* file_name)
{
	// currently all mesh are loaded from obj files.
	m_mesh = ResourceLoader::loadMeshFromFile(file_name, MeshFileType::obj);
}

void RenderEntity::release()
{
	if (m_mesh)
	{
		m_mesh->release();
		m_mesh = nullptr;
	}
}

void RenderEntity::render(Pass pass)
{
	if (pass == Pass::DeferredShading)
	{
		Material* deferred_material = g_global_context.m_material_manager.getGlobalPassMaterial(pass);
	}
}