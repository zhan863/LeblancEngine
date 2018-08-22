#ifndef LEBLANC_DEVICE_CONTEXT_D3D11_H
#define LEBLANC_DEVICE_CONTEXT_D3D11_H

#include "LeblancEngine/Render/Device/LeblancDeviceContext.h"
#include "LeblancEngine/Render/Defines/LeblancRenderBasicDefine.h"
#define MAX_RENDER_TARGETS 8

struct ID3D11DeviceContext;
namespace Leblanc
{
	class IDevice;
	class IDepthSurface;
	class ISurface;
	class DeviceContextD3D11 : public IDeviceContext
	{
	public:
		DeviceContextD3D11(IDevice* device);
		virtual ~DeviceContextD3D11();

		virtual void* getHandle() { return m_device_context; }

		void setHandle(ID3D11DeviceContext* handle) { m_device_context = handle; }

		virtual void renderIndexMesh(const IndexMesh* mesh);

		virtual void setViewPort(int left_x, int top_y, int width, int height);
		virtual void setRasterizerState(RasterizerState cull_mode);
		virtual void setDepthStencilState(DepthStencilState depth_stencil_mode);
		virtual void setBlendState(BlendState blend_mode);
		
		virtual void clearRenderTarget(unsigned int index_to_clear_to, unsigned long clear_type, float* color) const;
		virtual void clearSurface(ISurface* surface, float* color) const;
		virtual void clearDepthSurface(IDepthSurface* surface, unsigned long clear_type) const;

		virtual void bindRenderTarget(int index, const ISurface* surface);
		virtual void bindRenderTargets(int target_size, const ISurface* surface);
		virtual void bindDepthTarget(const IDepthSurface* surface);

		virtual void bindFrameBuffer(const FrameBuffer* frame_buffer);
		virtual void bindNullFrameBuffer();

	private:
		void release();
		void bindTargets();

		// reference
		const ISurface* m_current_render_targets[MAX_RENDER_TARGETS];
		const IDepthSurface* m_current_depth_target = nullptr;
		unsigned int m_current_target_count = 0;

		// data
		ID3D11DeviceContext* m_device_context = nullptr;
	};
}

#endif