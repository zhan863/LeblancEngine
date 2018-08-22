#ifndef LEBLANC_DEVICE_CONTEXT_H
#define LEBLANC_DEVICE_CONTEXT_H

#include "LeblancEngine/Render/Defines/LeblancRenderBasicDefine.h"

namespace Leblanc
{
	class IDevice;
	class IRasterizerState;
	class IBlendState;
	class IDepthStencilState;
	class IndexMesh;
	class ISurface;
	class IDepthSurface;
	class FrameBuffer;

	class IDeviceContext
	{
	public:
		IDeviceContext(IDevice* device);
		~IDeviceContext();

		virtual void* getHandle() = 0;

		virtual void renderIndexMesh(const IndexMesh* mesh) = 0;

		virtual void setViewPort(int left_x, int top_y, int width, int height) = 0;
		virtual void setRasterizerState(RasterizerState cull_mode) = 0;
		virtual void setDepthStencilState(DepthStencilState depth_stencil_mode) = 0;
		virtual void setBlendState(BlendState blend_mode) = 0;

		virtual void clearRenderTarget(unsigned int index_to_clear_to, unsigned long clearType, float* color) const = 0;
		virtual void clearSurface(ISurface* surface, float* color) const = 0;
		virtual void clearDepthSurface(IDepthSurface* surface, unsigned long clear_type) const = 0;

		virtual void bindRenderTarget(int index, const ISurface* surface) = 0;
		virtual void bindRenderTargets(int target_size, const ISurface* surface) = 0;
		virtual void bindDepthTarget(const IDepthSurface* surface) = 0;

		virtual void bindFrameBuffer(const FrameBuffer* frame_buffer) = 0;
		virtual void bindNullFrameBuffer() = 0;

	private:
		void release();

		// reference
		IDevice* m_device = nullptr;
	};
}
#endif