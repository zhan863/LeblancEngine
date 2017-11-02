#include "LeblancEngine/Render/Pipeline/LeblancPipelineManager.h"
#include "LeblancEngine/Render/Scene/LeblancSceneManager.h"
#include "LeblancEngine/Global/LeblancGlobalContext.h"

PipelineManager::PipelineManager()
{

}

PipelineManager::~PipelineManager()
{

}

void PipelineManager::initialize()
{
	m_deferred_pipeline.initialize();
}

void PipelineManager::release()
{

}

void PipelineManager::render(RenderType render_type)
{
	if (render_type == RenderType::Deferred)
	{
		m_deferred_pipeline.render(nullptr, g_global_context.m_scene_manager.getCurrentScene());
	}
}