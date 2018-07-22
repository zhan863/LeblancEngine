#ifndef LEBLANC_DEVICE_D3D11_H
#define LEBLANC_DEVICE_D3D11_H
#include "LeblancEngine/Render/Device/LeblancDevice.h"

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct D3D11_BUFFER_DESC;
struct D3D11_SUBRESOURCE_DATA;
struct ID3D11Buffer;
struct D3D11_RASTERIZER_DESC;
struct ID3D11RasterizerState;
struct D3D11_BLEND_DESC;
struct ID3D11BlendState;
struct D3D11_DEPTH_STENCIL_DESC;
struct ID3D11DepthStencilState;

namespace Leblanc
{
	class DeviceContextD3D11;
	class DeviceD3D11 : public IDevice
	{
	public:
		DeviceD3D11();
		virtual ~DeviceD3D11();

		virtual void initialize(Window* window);

		virtual IIndexBuffer* createIndexBuffer(const ResourceDeclaration* declaration);
		virtual IVertexBuffer* createVertexBuffer(const ResourceDeclaration* declaration);
		virtual ITexture* createTexture(const ResourceDeclaration* resource_declaration);

		virtual IVertexDeclaration* createVertexDeclaration(const VertexLayoutDeclaration* layout_declaration);
		virtual IRasterizerState* createRasterizerState(const RasterizerState* rasterizer_state_declaration);
		virtual IDepthStencilState* createDepthStencilState(const DepthStencilState* depth_stencil_declaration);
		virtual IBlendState* createBlendState(const BlendState* blend_declaration);
		virtual IShader* createShader(string file_name);

		virtual void present();

		virtual void* device() { return m_device; }

		virtual ISurface* backBuffer() { return m_back_buffer; }
		virtual IDepthSurface* depthSurface() { return m_depth_surface; }
		virtual FrameBuffer* getFrameBuffer() { return m_frame_buffer; }

		virtual void* getHandle() { return m_device; }

		// playform dependent
		void createRasterizerState(D3D11_RASTERIZER_DESC* desc, ID3D11RasterizerState** state);
		void createBlendState(D3D11_BLEND_DESC* desc, ID3D11BlendState** state);
		void createDepthStencilState(D3D11_DEPTH_STENCIL_DESC* desc, ID3D11DepthStencilState** state);
		void createBuffer(const D3D11_BUFFER_DESC *desc, D3D11_SUBRESOURCE_DATA *initial_data, ID3D11Buffer **buffer);

	protected:
		void release();

		// data
		ID3D11Device* m_device = nullptr;
		IDXGISwapChain* m_swap_chain = nullptr;
		DeviceContextD3D11* m_immediate_context = nullptr;
		ISurface* m_back_buffer = nullptr;
		IDepthSurface* m_depth_surface = nullptr;

		FrameBuffer* m_frame_buffer = nullptr;
	};
}
#endif