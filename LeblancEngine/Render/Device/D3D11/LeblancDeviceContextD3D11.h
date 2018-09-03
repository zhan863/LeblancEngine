#ifndef LEBLANC_DEVICE_CONTEXT_D3D11_H
#define LEBLANC_DEVICE_CONTEXT_D3D11_H

#include "LeblancEngine/Render/Device/LeblancDeviceContext.h"
#include "LeblancEngine/Render/Defines/LeblancRenderBasicDefine.h"
#include "LeblancEngine/Render/Include/LeblancDirectInclude.h"
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

		template <ShaderFrequency shader_frequency>
		void setConstantBuffer(int slot_index, ID3D11Buffer*& constant_buffer)
		{
			switch (shader_frequency)
			{
			case SF_Vertex:		m_device_context->VSSetConstantBuffers(slot_index, 1, &constant_buffer); break;
			case SF_Hull:		m_device_context->HSSetConstantBuffers(slot_index, 1, &constant_buffer); break;
			case SF_Domain:		m_device_context->DSSetConstantBuffers(slot_index, 1, &constant_buffer); break;
			case SF_Geometry:	m_device_context->GSSetConstantBuffers(slot_index, 1, &constant_buffer); break;
			case SF_Pixel:		m_device_context->PSSetConstantBuffers(slot_index, 1, &constant_buffer); break;
			case SF_Compute:	m_device_context->CSSetConstantBuffers(slot_index, 1, &constant_buffer); break;
			}
		}

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