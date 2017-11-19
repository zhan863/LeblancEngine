#include "LeblancEngine/Render/Pipeline/LeblancDeferredPipeline.h"
#include "LeblancEngine/Render/Scene/LeblancScene.h"

DeferredPipeline::DeferredPipeline()
{

}

DeferredPipeline::~DeferredPipeline()
{

}

void DeferredPipeline::initialize()
{

}

void DeferredPipeline::render(Texture2D* render_target, Scene& scene)
{
	generateGBuffer();

	deferredShading();

	postProcessing();
}

void DeferredPipeline::generateGBuffer()
{

}

void DeferredPipeline::deferredShading()
{

}

void DeferredPipeline::postProcessing()
{

}