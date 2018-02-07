#ifndef LEBLANC_DEVICE_D3D11_H
#define LEBLANC_DEVICE_D3D11_H

#include "LeblancEngine/Render/Include/LeblancDirectInclude.h"
#include "LeblancEngine/Render/Resource/Texture/LeblancTexture.h"
#include "LeblancEngine/Render/Resource/Texture/DepthStencilTexture.h"
#include "LeblancEngine/Render/Resource/Texture/Texture2D.h"
#include "LeblancEngine/Render/Basics/LeblancGeometry.h"
#include "LeblancEngine/Render/Basics/LeblancWindow.h"

#include <vector>
using namespace std;

class DeviceD3D11
{
public:
	DeviceD3D11();

	~DeviceD3D11();
	
	void initialize(Window& window);
	
	void release();

	void present();

	ID3D11Resource* createTexture(TextureType texture_type, UINT width, UINT height);

	ID3D11View* createRenderTargetView(TextureType texture_type, UINT width, UINT height, ID3D11Resource* resource);

	ID3D11Device* getD3D11Device() { return m_device; }
	
	LeblancMesh* createMesh(vector<Vertex>& vertices, vector<DWORD>& indices);

	ID3D11RenderTargetView* getBackBufferView();

	bool initialized() { return m_device != nullptr; }

	void setRenderTargets(UINT num_targets, Texture2D** render_targets, DepthStencilTexture* depth_stentil_texture);
	void setRenderTargets(UINT num_targets, ID3D11RenderTargetView** render_targets, DepthStencilTexture* depth_stentil_texture);

	void clearRenderTarget(ID3D11RenderTargetView* render_target);

private:
	ID3D11Device* m_device = nullptr;
	ID3D11DeviceContext* m_device_context = nullptr;
	IDXGISwapChain* m_swap_chain = nullptr;
	ID3D11RenderTargetView* m_back_buffer_view = nullptr;
};


#endif