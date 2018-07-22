#ifndef LEBLANC_RENDER_PIPELINE_H
#define LEBLANC_RENDER_PIPELINE_H

#include "LeblancEngine/Render/Pipeline/LeblancRenderPipeline.h"
#include "LeblancEngine/Render/Texture/LeblancTexture.h"
#include "LeblancEngine/Render/Scene/LeblancScene.h"

namespace Leblanc
{
	class RenderPipeline
	{
	public:
		RenderPipeline();

		~RenderPipeline();

		virtual void initialize();

		virtual void render(ITexture* render_target, Scene* scene) = 0;
	protected:
	};
}

#endif