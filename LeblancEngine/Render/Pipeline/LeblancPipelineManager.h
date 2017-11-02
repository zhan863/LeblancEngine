#ifndef LEBLANC_PIPELINE_MANAGER_H
#define LEBLANC_PIPELINE_MANAGER_H

#include "LeblancEngine/Render/Pipeline/LeblancDeferredPipeline.h"

class PipelineManager
{
public:
	PipelineManager();
	~PipelineManager();

	void initialize();

	void release();

protected:
	DeferredPipeline deferred_pipeline;
};
#endif