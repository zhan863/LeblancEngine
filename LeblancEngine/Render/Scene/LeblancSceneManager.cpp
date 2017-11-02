#include "LeblancEngine/Render/Scene/LeblancSceneManager.h"

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