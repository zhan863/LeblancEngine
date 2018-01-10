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

	void buildScene(const char* scene_file_name);

	Scene& getCurrentScene();

protected:
	Scene m_current_scene;
};
#endif