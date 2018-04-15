#ifndef LEBLANC_DEVICE_D3D11_H
#define LEBLANC_DEVICE_D3D11_H
#include "LeblancEngine/Render/Include/LeblancDirectInclude.h"
#include "LeblancEngine/Render/Resource/Texture/LeblancTexture.h"
#include "LeblancEngine/Render/Resource/Texture/DepthStencilTexture.h"
#include "LeblancEngine/Render/Resource/Texture/Texture2D.h"
#include "LeblancEngine/Render/Resource/Material/LeblancShaders.h"
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
	ID3D11RenderTargetView* getBackBufferView();
	// new create resource function
	void createBuffer(const D3D11_BUFFER_DESC *desc, D3D11_SUBRESOURCE_DATA *initial_data, ID3D11Buffer **buffer);
	IndexBufferD3D11* createIndexBuffer(const ResourceDeclaration* declaration);
	VertexBufferD3D11* createVertexBuffer(const ResourceDeclaration* declaration);
	ID3D11Device* getHandle() { return m_device; }

private:
	void release();

	// data
	ID3D11Device* m_device = nullptr;
	IDXGISwapChain* m_swap_chain = nullptr;
	ID3D11RenderTargetView* m_back_buffer_view = nullptr;
};


#endif