#include "LeblancEngine/Render/RenderState/LeblancRenderStateManager.h"
#include "LeblancEngine/BasicInclude/LeblancMemoryOperation.h"
#include "LeblancEngine/Render/Buffer/LeblancBuffer.h"
#include "LeblancEngine/Render/RenderState/LeblancRenderState.h"
#include "LeblancEngine/Render/Device/LeblancDevice.h"
#include "LeblancEngine/Render/Device/LeblancDeviceContext.h"
#include "LeblancEngine/Global/LeblancGlobalContext.h"

namespace Leblanc
{
	RenderStateManager::RenderStateManager()
	{

	}

	RenderStateManager::~RenderStateManager()
	{

	}

	void RenderStateManager::initialize()
	{

	}

	void RenderStateManager::release()
	{
		for (int i = 0; i < m_vertex_declarations.size(); i++)
		{
			safe_delete(m_vertex_declarations[i]);
		}
		m_vertex_declarations.clear();

		for each (auto iter in m_rasterizer_states)
		{
			safe_delete(iter.second);
		}
		m_rasterizer_states.clear();
	}

	IVertexDeclaration* RenderStateManager::getOrCreateVertexDeclaration(VertexLayoutDeclaration* layout_declaration)
	{
		if (!layout_declaration)
			return nullptr;

		const std::vector<VertexElement>& vertex_elements = layout_declaration->elements();
		for (int i = 0; i < m_vertex_declarations.size(); i++)
		{
			const std::vector<VertexElement>& ref_vertex_elements = m_vertex_declarations[i]->elements();

			if (vertex_elements.size() != ref_vertex_elements.size())
			{
				continue;
			}

			bool is_same_declaration = true;

			for (int j = 0; j < vertex_elements.size(); j++)
			{
				if (vertex_elements[j] != ref_vertex_elements[j])
				{
					is_same_declaration = false;
					break;
				}
			}

			if (is_same_declaration)
			{
				return m_vertex_declarations[i];
			}
		}

		IDeviceContext* device_context = g_global_context.m_device_manager.getImmediateContext();
		IDevice* device = g_global_context.m_device_manager.getDevice();

		IVertexDeclaration* new_vertex_declaration = device->createVertexDeclaration(layout_declaration);
		if (new_vertex_declaration)
		{
			m_vertex_declarations.push_back(device->createVertexDeclaration(layout_declaration));
		}
		else
		{
			//LOG(MEMORY ERROR);
		}

		return new_vertex_declaration;
	}

	IRasterizerState* RenderStateManager::getOrCreateRasterizerState(RasterizerState rasterizer_state)
	{
		auto iter = m_rasterizer_states.find(rasterizer_state);
		if (iter != m_rasterizer_states.end())
		{
			return iter->second;
		}

		IDevice* device = g_global_context.m_device_manager.getDevice();
		IRasterizerState* new_rasterizer_state = device->createRasterizerState(&rasterizer_state);
		if (new_rasterizer_state)
			m_rasterizer_states.insert(make_pair(rasterizer_state, new_rasterizer_state));
		return new_rasterizer_state;
	}

	IDepthStencilState* RenderStateManager::getOrCreateDepthStencilState(DepthStencilState depth_stencil_mode)
	{
		auto iter = m_depth_stencil_states.find(depth_stencil_mode);
		if (iter != m_depth_stencil_states.end())
		{
			return iter->second;
		}

		IDevice* device = g_global_context.m_device_manager.getDevice();
		IDepthStencilState* new_depth_stencil_state = device->createDepthStencilState(&depth_stencil_mode);
		if (new_depth_stencil_state)
			m_depth_stencil_states.insert(make_pair(depth_stencil_mode, new_depth_stencil_state));
		return new_depth_stencil_state;
	}

	IBlendState* RenderStateManager::getOrCreateBlendState(BlendState blend_mode)
	{
		auto iter = m_blend_states.find(blend_mode);
		if (iter != m_blend_states.end())
		{
			return iter->second;
		}

		IDevice* device = g_global_context.m_device_manager.getDevice();
		IBlendState* new_blend_state = device->createBlendState(&blend_mode);
		if (new_blend_state)
			m_blend_states.insert(make_pair(blend_mode, new_blend_state));

		return new_blend_state;
	}
}