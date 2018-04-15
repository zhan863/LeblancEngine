#ifndef LEBLANC_DEVICE_CONTEXT_D3D11_H
#define LEBLANC_DEVICE_CONTEXT_D3D11_H

#include "LeblancEngine/Render/Include/LeblancDirectInclude.h"
#include "LeblancEngine/Render/Basics/LeblancRenderPlatformIndependentDefine.h"

class IndexMesh;
class DeviceD3D11;
class Texture2D;
class DepthStencilTexture;

class DeviceContextD3D11
{
public:
	DeviceContextD3D11(DeviceD3D11* device);

	~DeviceContextD3D11();

	void setHandle(ID3D11DeviceContext* handle) { m_device_context = handle; }
	ID3D11DeviceContext* getHandle() { return m_device_context; }

	void setRenderTargets(UINT num_targets, Texture2D** render_targets, DepthStencilTexture* depth_stentil_texture);
	void setRenderTargets(UINT num_targets, ID3D11RenderTargetView** render_targets, DepthStencilTexture* depth_stentil_texture);
	void clearRenderTarget(ID3D11RenderTargetView* render_target);
	void setInputLayout(ID3D11InputLayout* input_layout);
	void renderIndexMesh(IndexMesh* mesh);
	void setViewPort(FLOAT left_x, FLOAT tp_y, FLOAT width, FLOAT height);
	void setRasterizerState(RasterizerState cull_mode);
	void setDepthStencilState(DepthStencilState depth_stencil_mode);
	void setBlendState(BlendState blend_mode);

private:
	void release();

	// reference
	DeviceD3D11* m_device = nullptr;

	// data
	ID3D11DeviceContext* m_device_context = nullptr;
};


#endif