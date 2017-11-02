#ifndef LEBLANC_PIPELINE_MANAGER_H
#define LEBLANC_PIPELINE_MANAGER_H

#include "LeblancEngine/Render/Pipeline/LeblancDeferredPipeline.h"

enum class RenderType
{
	Deferred,
	Forward
};

class PipelineManager
{
public:
	PipelineManager();
	~PipelineManager();

	void initialize();

	void release();

	void render(RenderType render_type);

protected:
	DeferredPipeline m_deferred_pipeline;
};
#endif