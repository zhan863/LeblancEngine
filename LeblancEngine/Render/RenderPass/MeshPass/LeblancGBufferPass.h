#ifndef LEBLANC_GBUFFER_PASS_H
#define LEBLANC_GBUFFER_PASS_H

#include "LeblancEngine/Render/RenderPass/MeshPass/LeblancMeshRenderPass.h"

namespace Leblanc
{
	class GBufferPass : public MeshRenderPass
	{
	public:
		GBufferPass();
		virtual ~GBufferPass();
		virtual void render();
	};
}
#endif