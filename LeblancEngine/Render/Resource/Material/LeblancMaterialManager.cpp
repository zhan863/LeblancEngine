#include "LeblancEngine/BasicInclude/LeblancPCH.h"
#include "LeblancEngine/Render/Resource/Material/LeblancMaterialManager.h"
#include "LeblancEngine/Render/Resource/Material/LeblancShaderCompile.h"

MaterialManager::MaterialManager()
{

}

MaterialManager::~MaterialManager()
{

}

void MaterialManager::initialize()
{
	loadShadowMapMaterial();
	loadGBufferMaterial();
	loadPostProcessingMaterial();
	loadDeferredShadingMaterial();
}

void MaterialManager::release()
{
	for (UINT i = 0; i < m_global_materials.size(); i++)
	{
		m_global_materials[i].release();
	}
}

void MaterialManager::loadShadowMapMaterial()
{
	Material material;
	compileMaterial("Content/Shader/shadow_map.hlsl", material, "VS", "PS");
	m_global_materials.push_back(material);
}

void MaterialManager::loadGBufferMaterial()
{
	Material material;
	compileMaterial("Content/Shader/shadow_map.hlsl", material, "VS", "PS");
	m_global_materials.push_back(material);
}

void MaterialManager::loadDeferredShadingMaterial()
{
	Material material;
	compileMaterial("Content/Shader/shadow_map.hlsl", material, "VS", "PS");
	m_global_materials.push_back(material);
}

void MaterialManager::loadPostProcessingMaterial()
{
	Material material;
	compileMaterial("Content/Shader/shadow_map.hlsl", material, "VS", "PS");
	m_global_materials.push_back(material);
}

void MaterialManager::compileMaterial(const char* material_file_name, Material& material, const char* vs, const char* ps)
{
	MaterialCompiler::compileMaterial(material_file_name, vs, ps, material);
}