#ifndef LEBLANC_MESH_RENDER_PASS_H
#define LEBLANC_MESH_RENDER_PASS_H

#include "LeblancEngine/Render/RenderPass/LeblancRenderPass.h"

namespace Leblanc
{
	class Scene;
	class MeshRenderPass : public RenderPass
	{
	public:
		MeshRenderPass();
		virtual ~MeshRenderPass();

		void setScene(const Scene* scene);

	protected:
		const Scene* m_scene = nullptr;
	};
}
#endif