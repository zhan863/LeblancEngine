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
	m_current_scene.clear();
}

Scene& SceneManager::getCurrentScene()
{
	return m_current_scene;
}

void SceneManager::buildScene(const char* scene_file_name)
{
	m_current_scene.clear();

	ResourceLoader::loadSceneFromFile(scene_file_name, m_current_scene);
}