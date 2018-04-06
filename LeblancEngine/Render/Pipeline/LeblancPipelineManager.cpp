#include "LeblancEngine/Render/Pipeline/LeblancPipelineManager.h"
#include "LeblancEngine/Render/Scene/LeblancSceneManager.h"
#include "LeblancEngine/Global/LeblancGlobalContext.h"
#include "LeblancEngine/Render/Resource/TextureOrgainizer/LeblancGBuffer.h"

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
		GBuffer* gbuffer = g_global_context.m_resource_manager.getGBuffer();
		m_deferred_pipeline.render(gbuffer->m_albedo_buffer, *g_global_context.m_scene_manager.getSceneByIndex(0));
	}
}