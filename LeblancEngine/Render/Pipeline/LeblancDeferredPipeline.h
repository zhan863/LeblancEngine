#ifndef LEBLANC_Deferred_PIPELINE_H
#define LEBLANC_Deferred_PIPELINE_H

#include "LeblancEngine/Render/Pipeline/LeblancRenderPipeline.h"

namespace Leblanc
{
	class ITexture;
	class IndexMesh;
	class DeferredPipeline : public RenderPipeline
	{
	public:
		DeferredPipeline();

		~DeferredPipeline();

		void initialize();

		void render(ITexture* render_target, Scene* scene);

	protected:
		void generateGBuffer();

		void deferredShading(ITexture* render_target, Scene* scene);

		void postProcessing();

		IndexMesh* m_test_mesh = nullptr;
	};
}

#endif