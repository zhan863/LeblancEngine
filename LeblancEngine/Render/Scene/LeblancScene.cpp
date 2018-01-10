#include "LeblancEngine/Render/Scene/LeblancSceneManager.h"

Scene::Scene()
{

}

Scene::~Scene()
{

}

void Scene::initialize()
{

}

void Scene::clear()
{
	for (int i = 0; i < m_render_entities.size(); i++)
	{
		m_render_entities[i].release();
	}

	m_render_entities.clear();
}

RenderEntity& Scene::addRenderEntity()
{
	RenderEntity render_entity;
	m_render_entities.push_back(render_entity);
	return m_render_entities[m_render_entities.size() - 1];
}

RenderEntity* Scene::getRenderEntity(int index)
{
	if (m_render_entities.size() > index)
	{
		return &m_render_entities[index];
	}

	return nullptr;
}