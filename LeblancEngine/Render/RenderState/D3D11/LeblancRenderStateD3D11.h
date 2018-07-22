#ifndef LEBLANC_RENDER_PLATFORM_DEFINE_D3D11_H
#define LEBLANC_RENDER_PLATFORM_DEFINE_D3D11_H

#include "LeblancEngine/Render/RenderState/LeblancRenderState.h"
#include "LeblancEngine/Render/Defines/LeblancRenderBasicDefine.h"
#include "LeblancEngine/Render/Include/LeblancDirectInclude.h"
#include <map>

struct ID3D11RasterizerState;
struct ID3DX11EffectTechnique;
struct ID3D11DepthStencilState;
struct ID3D11BlendState;
struct ID3D11InputLayout;

namespace Leblanc
{
	class RasterizerStateD3D11 : public IRasterizerState
	{
	public:
		RasterizerStateD3D11(IDevice* device, IDeviceContext* device_context) : IRasterizerState(device, device_context) {}
		virtual ~RasterizerStateD3D11() { release(); }

		virtual void release();

		void initialize(const RasterizerState* rasterizer_state);

		ID3D11RasterizerState* getRasterizerState() { return m_rasterizer_state; }
	protected:
		// data
		ID3D11RasterizerState * m_rasterizer_state = nullptr;
	};

	class DepthStencilStateD3D11 : public IDepthStencilState
	{
	public:
		DepthStencilStateD3D11(IDevice* device, IDeviceContext* device_context) : IDepthStencilState(device, device_context) {}
		virtual ~DepthStencilStateD3D11() { release(); }

		virtual void release();

		void initialize(const DepthStencilState* depth_stencil_state);

		ID3D11DepthStencilState* getDepthStencilState() { return m_depth_stencil_state; }
	protected:
		// data
		ID3D11DepthStencilState * m_depth_stencil_state = nullptr;
	};

	class BlendStateD3D11 : public IBlendState
	{
	public:
		BlendStateD3D11(IDevice* device, IDeviceContext* device_context) : IBlendState(device, device_context) {}
		virtual ~BlendStateD3D11() { release(); }

		virtual void release();

		void initialize(const BlendState* blend_mode);

		ID3D11BlendState* getBlendState() { return m_blend_state; }
	protected:
		// data
		ID3D11BlendState * m_blend_state = nullptr;
	};

	class VertexDeclarationD3D11;
	class InputLayoutCacheD3D11 : public IInputLayoutCache
	{
	public:
		InputLayoutCacheD3D11(IDevice* device, IDeviceContext* device_context);
		virtual ~InputLayoutCacheD3D11();

		virtual void release();
		bool bindLayout(const VertexDeclarationD3D11* vertex_declaration) const;
		void initialize(ID3DX11EffectTechnique* technique, unsigned int passIndex);

	protected:

		typedef std::map <const VertexDeclarationD3D11*, ID3D11InputLayout*> LayoutMap;
		// data
		mutable LayoutMap m_layouts;
		unsigned int m_pass_index = -1;
		D3DX11_PASS_DESC m_pass_desc;

		// reference
		ID3DX11EffectTechnique*         m_technique = nullptr;
	};
}
#endif