#include "LeblancEngine/Render/Pipeline/LeblancDeferredPipeline.h"
#include "LeblancEngine/Render/Scene/LeblancScene.h"
#include "LeblancEngine/Render/Device/LeblancDeviceManager.h"
#include "LeblancEngine/Render/Device/LeblancDeviceContext.h"
#include "LeblancEngine/Global/LeblancGlobalContext.h"

#include "LeblancEngine/Render/Material/LeblancMaterial.h"
#include "LeblancEngine/Render/Material/LeblancTechnique.h"
#include "LeblancEngine/Render/Material/LeblancShaders.h"
#include "LeblancEngine/Render/RenderEntity/LeblancIndexMesh.h"

#include "LeblancEngine/Render/Texture/LeblancTexture.h"

#include "LeblancEngine/Render/RenderPass/MeshPass/LeblancGBufferPass.h"

namespace Leblanc
{
	DeferredPipeline::DeferredPipeline()
	{

	}

	DeferredPipeline::~DeferredPipeline()
	{

	}

	void DeferredPipeline::initialize()
	{
		m_test_mesh = new IndexMesh();
		m_test_mesh->loadFromFile("Content\\Mesh\\Basic\\quad.fbx");

		g_global_context.m_scene_manager.loadScene("Content\\Mesh\\Basic\\quad.fbx");
	}

	void DeferredPipeline::render(ITexture* render_target, Scene* scene)
	{
		generateGBuffer();

		deferredShading(render_target, scene);

		postProcessing();
	}

	void DeferredPipeline::generateGBuffer()
	{
		GBufferPass pass;
		pass.setScene(g_global_context.m_scene_manager.getSceneByIndex(0));
		pass.render();
	}

	void DeferredPipeline::deferredShading(ITexture* render_target, Scene* scene)
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
		//IndexMesh* mesh = m_test_mesh;// g_global_context.m_scene_manager.getSceneByIndex(0);
		//Material* material = mesh->getMaterial();
		//material->setParameters();
		//IShader* shader = material->getShader();
		//ITechnique* technique = shader->getTechnique(material->getTechniqueName());
		//if (technique)
		//{
		//	technique->bindInputLayout(0, mesh->getVertexDeclaration());
		//	technique->apply(0);
		//	IDeviceContext* device_context = g_global_context.m_device_manager.getImmediateContext();
		//	device_context->setRasterizerState(RasterizerState::NO_CULL);
		//	device_context->setBlendState(BlendState::BLEND_OPAQUE);
		//	device_context->setDepthStencilState(DepthStencilState::DEPTH_LESS_WRITE_STENCIL_PASS);
		//	device_context->setViewPort(0, 0, 1280, 720);
		//	device_context->renderIndexMesh(mesh);
		//}
	}
}