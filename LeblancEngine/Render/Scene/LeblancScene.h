#ifndef LEBLANC_SCENE_H
#define LEBLANC_SCENE_H

#include "LeblancEngine/Render/RenderEntity/LeblancRenderEntity.h"
#include <vector>
#include <map>
using namespace std;

class Scene
{
public:
	Scene();
	~Scene();

	void release();

	bool load(const char* file_name);

	RenderEntity* getRenderEntity(int index);

	size_t getRenderEntityCount() { return m_render_entities.size(); }
protected:
	// resource
	vector<RenderEntity*> m_render_entities;

	// reference
	map<Pass, Mesh*> m_pass_mesh_map;
};
#endif