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

		DeviceD3D11& device = g_global_context.m_device_manager.getCurrentDevice();
		device.setVertexShader(&deferred_material->m_vertex_shader);
		device.setPixelShader(&deferred_material->m_pixel_shader);
		device.renderMesh(m_mesh);
	}
}