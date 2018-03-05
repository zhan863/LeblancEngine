#include "LeblancEngine/Render/Scene/LeblancSceneManager.h"
#include "LeblancEngine/Render/Resource/ResourceLoader/LeblancResourceLoader.h"

SceneManager::SceneManager()
{

}

SceneManager::~SceneManager()
{

}

void SceneManager::initialize()
{

}

void SceneManager::release()
{
	for (int i = 0; i < m_scenes.size(); i++)
	{
		if (m_scenes[i])
			m_scenes[i]->release();
	}
	m_scenes.clear();
}

Scene* SceneManager::getSceneByIndex(int index)
{
	if (index >= 0)
		return m_scenes[index];

	return nullptr;
}

void SceneManager::loadScene(const char* scene_file_name)
{
	Scene* new_scene = new Scene;
	if (new_scene)
	{
		if (new_scene->load(scene_file_name))
			m_scenes.push_back(new_scene);
		else
		{
			new_scene->release();
			delete new_scene;
		}
	}
}