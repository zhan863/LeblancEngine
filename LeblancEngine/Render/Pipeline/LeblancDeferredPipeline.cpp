#include "LeblancEngine/Render/Pipeline/LeblancDeferredPipeline.h"
#include "LeblancEngine/Render/Scene/LeblancScene.h"
#include "LeblancEngine/Render/Utility/LeblancDeviceManager.h"
#include "LeblancEngine/Global/LeblancGlobalContext.h"

#include "LeblancEngine/Render/Resource/Material/LeblancMaterial.h"
#include "LeblancEngine/Render/Resource/Material/LeblancTechnique.h"
#include "LeblancEngine/Render/Resource/Material/LeblancShaders.h"
#include "LeblancEngine/Render/RenderEntity/LeblancIndexMesh.h"
#include "LeblancEngine/Render/Utility/LeblancDeviceContextD3D11.h"

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

	deferredShading(render_target, scene);

	postProcessing();
}

void DeferredPipeline::generateGBuffer()
{

}

void DeferredPipeline::deferredShading(Texture2D* render_target, Scene& scene)
{
	//DeviceD3D11& device = g_global_context.m_device_manager.getCurrentDevice();

	////device.setRenderTargets(1, &render_target, nullptr);
	//device.setViewPort(0, 0, render_target->getSize().x, render_target->getSize().y);

	//for (UINT i = 0; i < scene.getRenderEntityCount(); i++)
	//{
	//	RenderEntity* render_entity = scene.getRenderEntity(i);
	//	if (render_entity)
	//	{
	//		render_entity->render(Pass::DeferredShading);
	//	}
	//}
}

void DeferredPipeline::postProcessing()
{
	IndexMesh* mesh = g_global_context.m_resource_manager.getScreenQuad();
	Material* material = mesh->getMaterial();
	material->setParameters();
	Shader* shader = material->getShader();
	Technique* technique = shader->getTechnique(material->getTechniqueName());
	if (technique)
	{
		technique->bindInputLayout(0, mesh->getVertexDeclaration());
		technique->apply(0);
		DeviceContextD3D11* device_context = g_global_context.m_device_manager.getImmediateContext();
		device_context->setRasterizerState(RasterizerState::NONE);
		device_context->setBlendState(BlendState::BLEND_OPAQUE);
		device_context->setDepthStencilState(DepthStencilState::ALL_PASS);
		device_context->setViewPort(0, 0, 1280, 720);
		device_context->renderIndexMesh(mesh);
	}
}