#ifndef LEBLANC_FRAME_BUFFER_H
#define LEBLANC_FRAME_BUFFER_H

#include "LeblancEngine/Render/Include/LeblancDirectInclude.h"
#include "LeblancEngine/Render/Defines/LeblancRenderBasicDefine.h"

namespace Leblanc
{
	class ISurface;
	class IDepthSurface;

	class FrameBuffer
	{
	public:
		FrameBuffer();
		~FrameBuffer();

		FrameBuffer(const ISurface* color_surfaces[], const IDepthSurface* depth_surface, int color_surface_num);

		int   getRenderTargetNum() const { return m_render_target_num; }
		const ISurface* getColorSurfaces(int index) const { return m_color_surfaces[index]; }
		const IDepthSurface* getDepthSurface() const { return m_depth_surface; }

	protected:
		const ISurface* m_color_surfaces[4] = { nullptr };
		const IDepthSurface* m_depth_surface = nullptr;
		int   m_render_target_num = 0;
	};
}
#endif