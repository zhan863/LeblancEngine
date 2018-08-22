#include "LeblancEngine/Render/RenderPass/LeblancRenderPass.h"
#include "LeblancEngine/BasicInclude/LeblancMemoryOperation.h"
#include "LeblancEngine/Render/RenderState/LeblancRenderPipelineState.h"

namespace Leblanc
{
	RenderPass::RenderPass()
	{
		m_pipeline_state_cache = new PipelineState();
	}
	
	RenderPass::~RenderPass()
	{
		clear();
	}

	void RenderPass::clear()
	{
		safe_delete(m_pipeline_state_cache);
	}
}