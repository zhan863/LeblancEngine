#include "LeblancEngine/Render/Scene/LeblancSceneManager.h"
#include "LeblancEngine/BasicInclude/LeblancMemoryOperation.h"

namespace Leblanc
{
	SceneManager::SceneManager()
	{

	}

	SceneManager::~SceneManager()
	{
		release();
	}

	void SceneManager::initialize()
	{

	}

	void SceneManager::release()
	{
		for (int i = 0; i < m_scenes.size(); i++)
		{
			safe_delete(m_scenes[i]);
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
				safe_delete(new_scene);
			}
		}
	}
}