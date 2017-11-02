#ifndef LEBLANC_Render_PIPELINE_H
#define LEBLANC_Render_PIPELINE_H

#include "LeblancEngine/Render/Pipeline/LeblancRenderPipeline.h"
#include "LeblancEngine/Render/Resource/Texture/Texture2D.h"
#include "LeblancEngine/Render/Scene/LeblancScene.h"

class RenderPipeline
{
public:
	RenderPipeline();

	~RenderPipeline();

	virtual void initialize();

	virtual void render(Texture2D* render_target, Scene& scene) = 0;
protected:
};


#endif