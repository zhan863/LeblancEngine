#ifndef LEBLANC_RENDER_STATE_MANAGER_H
#define LEBLANC_RENDER_STATE_MANAGER_H
#include "LeblancEngine/BasicInclude/LeblancPCH.h"
#include "LeblancEngine/Render/Resource/Material/LeblancShaders.h"
#include "LeblancEngine/Render/Basics/LeblancRenderPlatformDefineD3D11.h"

class RenderStateManager
{
public:
	RenderStateManager();
	~RenderStateManager();

	void initialize();

	void release();

	VertexDeclarationD3D11* getOrCreateVertexDeclaration(const VertexLayoutDeclaration* layout_declaration);

protected:
	std::vector<VertexDeclarationD3D11*> m_vertex_declarations;
};
#endif