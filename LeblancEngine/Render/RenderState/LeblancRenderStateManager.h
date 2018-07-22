#ifndef LEBLANC_RENDER_STATE_MANAGER_H
#define LEBLANC_RENDER_STATE_MANAGER_H

#include <map>
#include <vector>
#include "LeblancEngine/Render/Defines/LeblancRenderBasicDefine.h"

namespace Leblanc
{
	class IVertexDeclaration;
	class IRasterizerState;
	class IDepthStencilState;
	class IBlendState;
	class VertexLayoutDeclaration;

	class RenderStateManager
	{
	public:
		RenderStateManager();
		~RenderStateManager();

		void initialize();

		void release();

		IVertexDeclaration* getOrCreateVertexDeclaration(VertexLayoutDeclaration* layout_declaration);
		IRasterizerState* getOrCreateRasterizerState(RasterizerState rasterizer_mode);
		IDepthStencilState* getOrCreateDepthStencilState(DepthStencilState depth_stencil_mode);
		IBlendState* getOrCreateBlendState(BlendState blend_mode);
	protected:
		std::vector<IVertexDeclaration*> m_vertex_declarations;
		std::map<RasterizerState, IRasterizerState*> m_rasterizer_states;
		std::map<DepthStencilState, IDepthStencilState*> m_depth_stencil_states;
		std::map<BlendState, IBlendState*> m_blend_states;
	};
}
#endif