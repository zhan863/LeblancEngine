#ifndef LEBLANC_SCENE_H
#define LEBLANC_SCENE_H

#include "LeblancEngine/Render/RenderEntity/LeblancRenderEntity.h"
#include "LeblancEngine/Render/RenderEntity/LeblancMesh.h"
#include <vector>
#include <map>
#include <string>
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

		const vector<Mesh*>& meshForPass(string pass_name) const;
	protected:
		void release();

		// resource
		vector<RenderEntity*> m_render_entities;

		// reference
		map<string, vector<Mesh*>> m_pass_mesh_map;
	};
}
#endif