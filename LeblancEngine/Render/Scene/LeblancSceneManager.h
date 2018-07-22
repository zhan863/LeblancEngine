#ifndef LEBLANC_SCENE_MANAGER_H
#define LEBLANC_SCENE_MANAGER_H

#include "LeblancEngine/Render/Scene/LeblancScene.h"

namespace Leblanc
{
	class SceneManager
	{
	public:
		SceneManager();
		~SceneManager();

		void initialize();

		void loadScene(const char* scene_file_name);

		Scene* getSceneByIndex(int index);

	protected:
		void release();

		vector<Scene*> m_scenes;
	};
}
#endif