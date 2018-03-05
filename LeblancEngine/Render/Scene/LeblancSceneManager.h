#ifndef LEBLANC_SCENE_MANAGER_H
#define LEBLANC_SCENE_MANAGER_H

#include "LeblancEngine/Render/Scene/LeblancScene.h"

class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	void initialize();

	void release();

	void loadScene(const char* scene_file_name);

	Scene* getSceneByIndex(int index);

protected:

	vector<Scene*> m_scenes;
};
#endif