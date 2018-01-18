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
	//loadShadowMapMaterial();
	loadGBufferMaterial();
	loadDeferredShadingMaterial();
	//loadPostProcessingMaterial();
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
	compileMaterial(L"Content/Shader/shadow_map.hlsl", material, "VS", "PS");
	m_global_materials.push_back(material);
}

void MaterialManager::loadGBufferMaterial()
{
	Material material;
	compileMaterial(L"Content/Shader/gbuffer.hlsl", material, "VS", "PS");
	m_global_materials.push_back(material);
}

void MaterialManager::loadDeferredShadingMaterial()
{
	Material material;
	compileMaterial(L"Content/Shader/deferred.hlsl", material, "VS", "PS");
	m_global_materials.push_back(material);
}

void MaterialManager::loadPostProcessingMaterial()
{
	Material material;
	compileMaterial(L"Content/Shader/post_processing.hlsl", material, "VS", "PS");
	m_global_materials.push_back(material);
}

void MaterialManager::compileMaterial(LPCWSTR material_file_name, Material& material, LPCSTR vs, LPCSTR ps)
{
	ShaderCompiler::compileMaterial(material_file_name, vs, ps, material);
}