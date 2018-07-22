#ifndef LEBLANC_IDEVICE_H
#define LEBLANC_IDEVICE_H

#include "LeblancEngine/Render/Defines/LeblancRenderBasicDefine.h"
#include <string>

using namespace std;

namespace Leblanc
{
	class ResourceDeclaration;
	class VertexLayoutDeclaration;
	class ITexture;
	class IIndexBuffer;
	class IVertexBuffer;
	class Window;
	class ISurface;
	class IDepthSurface;
	class FrameBuffer;
	class IVertexDeclaration;
	class IRasterizerState;
	class IDepthStencilState;
	class IBlendState;
	class IShader;

	class IDevice
	{
	public:
		IDevice();
		virtual ~IDevice();

		virtual void initialize(Window* window) = 0;

		virtual IIndexBuffer* createIndexBuffer(const ResourceDeclaration* declaration) = 0;
		virtual IVertexBuffer* createVertexBuffer(const ResourceDeclaration* declaration) = 0;
		virtual ITexture* createTexture(const ResourceDeclaration* resource_declaration) = 0;

		virtual IVertexDeclaration* createVertexDeclaration(const VertexLayoutDeclaration* layout_declaration) = 0;
		virtual IRasterizerState* createRasterizerState(const RasterizerState* rasterizer_state_declaration) = 0;
		virtual IDepthStencilState* createDepthStencilState(const DepthStencilState* depth_stencil_declaration) = 0;
		virtual IBlendState* createBlendState(const BlendState* blend_declaration) = 0;

		virtual IShader* createShader(string file_name) = 0;

		virtual ISurface* backBuffer() = 0;
		virtual IDepthSurface* depthSurface() = 0;
		virtual FrameBuffer* getFrameBuffer() = 0;

		virtual void* getHandle() = 0;

		virtual void present() = 0;

		virtual void* device() = 0;

	private:
		void release();
	};
}
#endif