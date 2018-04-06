#ifndef LEBLANC_RENDER_STATE_MANAGER_H
#define LEBLANC_RENDER_STATE_MANAGER_H
#include "LeblancEngine/BasicInclude/LeblancPCH.h"
#include "LeblancEngine/Render/Resource/Material/LeblancShaders.h"
#include "LeblancEngine/Render/Basics/LeblancRenderPlatformDefineD3D11.h"
#include "LeblancEngine/Render/Basics/LeblancRenderPlatformIndependentDefine.h"

class RenderStateManager
{
public:
	RenderStateManager();
	~RenderStateManager();

	void initialize();

	void release();

	VertexDeclarationD3D11* getOrCreateVertexDeclaration(const VertexLayoutDeclaration* layout_declaration);
	RasterizerStateD3D11* getOrCreateRasterizerState(RasterizerState rasterizer_mode);
protected:
	std::vector<VertexDeclarationD3D11*> m_vertex_declarations;
	std::map<RasterizerState, RasterizerStateD3D11*> m_rasterizer_states;
};
#endif