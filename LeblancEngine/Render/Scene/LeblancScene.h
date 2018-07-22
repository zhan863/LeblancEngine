#ifndef LEBLANC_SCENE_H
#define LEBLANC_SCENE_H

#include "LeblancEngine/Render/RenderEntity/LeblancRenderEntity.h"
#include "LeblancEngine/Render/RenderEntity/LeblancMesh.h"
#include <vector>
#include <map>
using namespace std;
using namespace Leblanc;

namespace Leblanc
{
	class Scene
	{
	public:
		Scene();
		~Scene();

		bool load(const char* file_name);

		RenderEntity* getRenderEntity(int index);

		size_t getRenderEntityCount() { return m_render_entities.size(); }
	protected:
		void release();

		// resource
		vector<RenderEntity*> m_render_entities;

		// reference
		map<Pass, vector<Mesh*>> m_pass_mesh_map;
	};
}
#endif