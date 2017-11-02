#ifndef LEBLANC_Deferred_PIPELINE_H
#define LEBLANC_Deferred_PIPELINE_H

#include "LeblancEngine/Render/Pipeline/LeblancRenderPipeline.h"
#include "LeblancEngine/Render/Resource/Texture/Texture2D.h"

class DeferredPipeline : public RenderPipeline
{
public:
	DeferredPipeline();

	~DeferredPipeline();

	void initialize();

	void render(Texture2D* render_target);
protected:
};


#endif