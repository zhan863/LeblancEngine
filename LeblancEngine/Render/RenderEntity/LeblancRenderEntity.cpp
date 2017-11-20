#include "LeblancEngine/Render/RenderEntity/LeblancRenderEntity.h"
#include "LeblancEngine/Render/Resource/ResourceLoader/LeblancResourceLoader.h"

RenderEntity::RenderEntity()
{

}

RenderEntity::~RenderEntity()
{
	release();
}

void RenderEntity::createFromFile(char* file_name)
{
	// currently all mesh are loaded from obj files.
	m_mesh = ResourceLoader::loadMeshFromFile(file_name, MeshFileType::obj);
}

void RenderEntity::release()
{
	if (m_mesh)
	{
		m_mesh->Release();
		m_mesh = nullptr;
	}
}

void RenderEntity::render(Pass pass)
{

}