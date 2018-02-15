#include "LeblancEngine/Render/Resource/LeblancRenderStateManager.h"
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

}

ID3D11InputLayout* RenderStateManager::getOrCreateInputLayout(InputLayoutEnum input_layout_enum, VertexShader* vertex_shader)
{
	if (m_input_layouts[input_layout_enum])
		return m_input_layouts[input_layout_enum];

	if (input_layout_enum == input_layout_pos)
	{
		D3D11_INPUT_ELEMENT_DESC positionLayout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		DeviceD3D11& device = g_global_context.m_device_manager.getCurrentDevice();
	}
	else if (input_layout_enum == input_layout_pos_normal_uv)
	{

	}
	else if (input_layout_enum == input_layout_pos_normal_uv2)
	{

	}

	return nullptr;
}