#ifndef LEBLANC_SCENE_H
#define LEBLANC_SCENE_H

#include "LeblancEngine/Render/RenderEntity/LeblancRenderEntity.h"
#include <vector>
using namespace std;

class Scene
{
public:
	Scene();
	~Scene();

	void initialize();

	void clear();

	RenderEntity& addRenderEntity();

	RenderEntity* getRenderEntity(int index);

	size_t getRenderEntityCount() { return m_render_entities.size(); }
protected:
	vector<RenderEntity> m_render_entities;
};
#endif