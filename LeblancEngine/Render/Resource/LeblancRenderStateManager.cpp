#include "LeblancEngine/Render/Resource/LeblancRenderStateManager.h"
#include "LeblancEngine/Global/LeblancGlobalContext.h"
#include "LeblancEngine/BasicInclude/LeblancMemoryOperation.h"
#include "LeblancEngine/Global/LeblancGlobalContext.h"

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

VertexDeclarationD3D11* RenderStateManager::getOrCreateVertexDeclaration(const VertexLayoutDeclaration* layout_declaration)
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

	DeviceContextD3D11* device_context = g_global_context.m_device_manager.getImmediateContext();
	DeviceD3D11* device = g_global_context.m_device_manager.getDevice();

	VertexDeclarationD3D11* new_vertex_declaration = new VertexDeclarationD3D11(device, device_context);
	if (new_vertex_declaration)
	{
		new_vertex_declaration->initialize(layout_declaration);
		m_vertex_declarations.push_back(new_vertex_declaration);
	}
	else
	{
		//LOG(MEMORY ERROR);
	}

	return new_vertex_declaration;
}

RasterizerStateD3D11* RenderStateManager::getOrCreateRasterizerState(RasterizerState rasterizer_state)
{
	auto iter = m_rasterizer_states.find(rasterizer_state);
	if (iter != m_rasterizer_states.end())
	{
		return iter->second;
	}

	DeviceD3D11* device = g_global_context.m_device_manager.getDevice();
	DeviceContextD3D11* device_context = g_global_context.m_device_manager.getImmediateContext();
	RasterizerStateD3D11* new_rasterizer_state = new RasterizerStateD3D11(device, device_context);
	new_rasterizer_state->initialize(rasterizer_state);

	m_rasterizer_states.insert(make_pair(rasterizer_state, new_rasterizer_state));
	return new_rasterizer_state;
}

DepthStencilStateD3D11* RenderStateManager::getOrCreateDepthStencilState(DepthStencilState depth_stencil_mode)
{
	auto iter = m_depth_stencil_states.find(depth_stencil_mode);
	if (iter != m_depth_stencil_states.end())
	{
		return iter->second;
	}

	DeviceD3D11* device = g_global_context.m_device_manager.getDevice();
	DeviceContextD3D11* device_context = g_global_context.m_device_manager.getImmediateContext();
	DepthStencilStateD3D11* new_depth_stencil_state = new DepthStencilStateD3D11(device, device_context);
	new_depth_stencil_state->initialize(depth_stencil_mode);

	m_depth_stencil_states.insert(make_pair(depth_stencil_mode, new_depth_stencil_state));
	return new_depth_stencil_state;
}

BlendStateD3D11* RenderStateManager::getOrCreateBlendState(BlendState blend_mode)
{
	auto iter = m_blend_states.find(blend_mode);
	if (iter != m_blend_states.end())
	{
		return iter->second;
	}

	DeviceD3D11* device = g_global_context.m_device_manager.getDevice();
	DeviceContextD3D11* device_context = g_global_context.m_device_manager.getImmediateContext();
	BlendStateD3D11* new_blend_state = new BlendStateD3D11(device, device_context);
	new_blend_state->initialize(blend_mode);

	m_blend_states.insert(make_pair(blend_mode, new_blend_state));
	return new_blend_state;
}