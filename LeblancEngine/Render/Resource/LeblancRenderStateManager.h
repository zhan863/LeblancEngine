#ifndef LEBLANC_RENDER_STATE_MANAGER_H
#define LEBLANC_RENDER_STATE_MANAGER_H
#include "LeblancEngine/BasicInclude/LeblancPCH.h"
#include "LeblancEngine/Render/Resource/Material/LeblancShaders.h"

enum InputLayoutEnum
{
	input_layout_pos_normal_uv,
	input_layout_pos_normal_uv2,
	input_layout_pos,
	k_input_layout_count
};

class RenderStateManager
{
public:
	RenderStateManager();
	~RenderStateManager();

	void initialize();

	void release();

	ID3D11InputLayout* getOrCreateInputLayout(InputLayoutEnum input_layout_enum, VertexShader* vertex_shader);

protected:
	ID3D11InputLayout* m_input_layouts[InputLayoutEnum::k_input_layout_count] = { nullptr, nullptr, nullptr };
};
#endif