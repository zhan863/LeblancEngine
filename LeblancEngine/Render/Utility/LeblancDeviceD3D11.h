#ifndef LEBLANC_DEVICE_D3D11_H
#define LEBLANC_DEVICE_D3D11_H
#include "LeblancEngine/Render/Include/LeblancDirectInclude.h"
#include "LeblancEngine/Render/Resource/Texture/LeblancTexture.h"
#include "LeblancEngine/Render/Resource/Texture/DepthStencilTexture.h"
#include "LeblancEngine/Render/Resource/Texture/Texture2D.h"
#include "LeblancEngine/Render/Resource/Material/LeblancShaders.h"
#include "LeblancEngine/Render/Basics/LeblancGeometry.h"
#include "LeblancEngine/Render/Basics/LeblancWindow.h"

#include "LeblancEngine/Render/Basics/LeblancRenderPlatformDefineD3D11.h"
#include "LeblancEngine/Render/Basics/LeblancRenderPlatformIndependentDefine.h"

#include <vector>
using namespace std;

class IndexMesh;
class DeviceD3D11
{
public:
	DeviceD3D11();

	~DeviceD3D11();

	void initialize(Window& window);

	void present();

	ID3D11Resource* createTexture(TextureTypes texture_type, UINT width, UINT height);

	ID3D11View* createRenderTargetView(TextureTypes texture_type, UINT width, UINT height, ID3D11Resource* resource);

	Mesh1* createMesh(vector<Vertex>& vertices, vector<UINT>& indices);

	ID3D11RenderTargetView* getBackBufferView();

	bool initialized() { return m_device != nullptr; }

	void setRenderTargets(UINT num_targets, Texture2D** render_targets, DepthStencilTexture* depth_stentil_texture);
	void setRenderTargets(UINT num_targets, ID3D11RenderTargetView** render_targets, DepthStencilTexture* depth_stentil_texture);

	void clearRenderTarget(ID3D11RenderTargetView* render_target);

	void setVertexShader(VertexShader* vertex_shader);
	void setPixelShader(PixelShader* pixel_shader);

	void setInputLayout(ID3D11InputLayout* input_layout);

	void renderIndexMesh(IndexMesh* mesh);

	void setViewPort(FLOAT left_x, FLOAT tp_y, FLOAT width, FLOAT height);

	void setRasterizerState(RasterizerState cull_mode);

	void setDepthStencilState(DepthStencilState depth_stencil_mode);

	void setBlendState(BlendState blend_mode);

	ID3D11InputLayout* createInputLayout(D3D11_INPUT_ELEMENT_DESC* input_layout_desc, UINT layout_desc_count, VertexShader* vertex_shader);

	// new create resource function
	void createBuffer(const D3D11_BUFFER_DESC *desc, D3D11_SUBRESOURCE_DATA *initial_data, ID3D11Buffer **buffer);

	IndexBufferD3D11* createIndexBuffer(const ResourceDeclaration* declaration);

	VertexBufferD3D11* createVertexBuffer(const ResourceDeclaration* declaration);

	ID3D11DeviceContext* getImmediateDeviceContext() { return m_device_context; }

	ID3D11Device* getD3D11Device() { return m_device; }

private:
	void release();

	// data
	ID3D11Device* m_device = nullptr;
	ID3D11DeviceContext* m_device_context = nullptr;
	IDXGISwapChain* m_swap_chain = nullptr;
	ID3D11RenderTargetView* m_back_buffer_view = nullptr;
};


#endif