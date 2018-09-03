#ifndef LEBLANC_PIPELINE_MANAGER_H
#define LEBLANC_PIPELINE_MANAGER_H

#include "LeblancEngine/Render/Pipeline/LeblancDeferredPipeline.h"
namespace Leblanc
{
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

		void render(RenderType render_type);

	protected:
		void release();

		DeferredPipeline m_deferred_pipeline;
	};
}
#endif