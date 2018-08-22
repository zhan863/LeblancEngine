#ifndef LEBLANC_RENDER_PIPELINE_STATE_H
#define LEBLANC_RENDER_PIPELINE_STATE_H

#include "LeblancEngine/Render/Defines/LeblancRenderBasicDefine.h"
#include "LeblancEngine/Math/LeblancMath.h"
#include <string>

using namespace std;

namespace Leblanc
{
	class PipelineState
	{
	public:
		PipelineState();
		~PipelineState();

		RasterizerState rasterizer_state = CW_BACK;
		DepthStencilState depth_stencil_state = DEPTH_LESS_WRITE_STENCIL_PASS;
		BlendState blend_state = BLEND_OPAQUE;
		Vector4i view_port = Vector4i(0, 0, 1280, 720);
		string shader_technique_name = "";
	};
}
#endif