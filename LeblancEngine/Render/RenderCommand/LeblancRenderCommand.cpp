#include "LeblancEngine/Render/RenderCommand/LeblancRenderCommand.h"
#include "LeblancEngine/Global/LeblancGlobalContext.h"
#include "LeblancEngine/Render/Device/LeblancDeviceContext.h"
#include "LeblancEngine/Render/Material/LeblancMaterial.h"
#include "LeblancEngine/Render/Material/LeblancShaders.h"
#include "LeblancEngine/Render/Material/LeblancTechnique.h"
#include "LeblancEngine/Render/RenderEntity/LeblancIndexMesh.h"

namespace Leblanc
{
	void RenderCommand::execute()
	{
		IDeviceContext* device_context = g_global_context.m_device_manager.getImmediateContext();
		if (device_context && m_mesh && m_shader)
		{
			ITechnique* technique = nullptr;
			if (m_material)
			{
				m_material->setParameters();
				technique = m_shader->getTechnique(m_material->getTechniqueName());
			}

			if (!m_material && !technique)
				technique = m_shader->getTechnique(m_state_cache.shader_technique_name);

			if (technique)
			{
				technique->apply(0);

				technique->bindInputLayout(0, m_mesh->getVertexDeclaration());
				device_context->setRasterizerState(m_state_cache.rasterizer_state);
				device_context->setViewPort(m_state_cache.view_port.x, m_state_cache.view_port.y, m_state_cache.view_port.z, m_state_cache.view_port.w);
				device_context->setDepthStencilState(m_state_cache.depth_stencil_state);
				device_context->setBlendState(m_state_cache.blend_state);

				device_context->renderIndexMesh(dynamic_cast<const IndexMesh*>(m_mesh));
			}
		}
	}
}