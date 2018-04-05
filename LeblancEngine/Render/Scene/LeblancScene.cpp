#include "LeblancEngine/Render/Scene/LeblancSceneManager.h"
#include "LeblancEngine/Render/RenderEntity/LeblancIndexMesh.h"
#include "LeblancEngine/BasicInclude/LeblancMemoryOperation.h"

#include <assimp/Include/Importer.hpp>
#include <assimp/Include/scene.h>
#include <assimp/Include/postprocess.h>

Scene::Scene()
{

}

Scene::~Scene()
{
	release();
}

bool Scene::load(const char* file_name)
{
	// To do: need to load the .scene file instead of the fbx file directly

	Assimp::Importer importer;
	uint32_t flags = aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_PreTransformVertices |
		aiProcess_FlipUVs;

	// the ai scene here equals the entity and the sub mesh of it equals the mesh contained by the entity;
	const aiScene* scene = importer.ReadFile(file_name, flags);   
	
	if (scene == nullptr)
	{
		//LOG("Failed to load scene " << meshFilePathName);
		return false;
	}

	if (scene->mNumMeshes == 0)
	{
		//LOG("Failed to load any meshes " << meshFilePathName);
		return false;
	}
	
	if (scene->HasMeshes())
	{
		RenderEntity* render_entity = new RenderEntity;
		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[i];
			if (mesh)
			{
				IndexMesh* index_mesh = new IndexMesh;
				if (index_mesh)
				{
					index_mesh->load(mesh, scene);
				}

				render_entity->addMesh(index_mesh);
				m_pass_mesh_map[Pass::DeferredShading].push_back(index_mesh);
			}
		}
		m_render_entities.push_back(render_entity);
	}
}

void Scene::release()
{
	// resource
	for (int i = 0; i < m_render_entities.size(); i++)
	{
		safe_delete(m_render_entities[i]);
	}

	m_render_entities.clear();

	// reference
	m_pass_mesh_map.clear();
}

RenderEntity* Scene::getRenderEntity(int index)
{
	if (m_render_entities.size() > index)
	{
		return m_render_entities[index];
	}

	return nullptr;
}