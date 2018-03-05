#include "LeblancEngine/Render/Scene/LeblancSceneManager.h"

Scene::Scene()
{

}

Scene::~Scene()
{

}

bool Scene::load(const char* file_name)
{
	return false;
}

void Scene::release()
{
	// resource
	for (int i = 0; i < m_render_entities.size(); i++)
	{
		if (m_render_entities[i])
			m_render_entities[i]->release();
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