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

	DeviceD3D11* device = g_global_context.m_device_manager.getCurrentDevice(); 

	VertexDeclarationD3D11* new_vertex_declaration = new VertexDeclarationD3D11(device);
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