#include "LeblancEngine/Render/Texture/LeblancFrameBuffer.h"
#include "LeblancEngine/Render/Texture/LeblancSurface.h"
#include "LeblancEngine/Render/Texture/LeblancDepthSurface.h"

namespace Leblanc
{
	static const int k_max_render_target_num = 4;
	FrameBuffer::FrameBuffer()
	{

	}

	FrameBuffer::~FrameBuffer()
	{

	}

	FrameBuffer::FrameBuffer(const ISurface* color_surfaces[], const IDepthSurface* depth_surface, int color_surface_num)
	{
		int target_num = min(k_max_render_target_num, color_surface_num);
		for (int i = 0; i < target_num; i++)
		{
			m_color_surfaces[i] = color_surfaces[i];
		}
		m_render_target_num = color_surface_num;

		m_depth_surface = depth_surface;
	}
}