#ifndef LEBLANC_DEVICE_D3D11_H
#define LEBLANC_DEVICE_D3D11_H

#include "LeblancEngine/Render/Include/LeblancDirectInclude.h"
#include "LeblancEngine/Render/Resource/Texture/LeblancTexture.h"
#include "LeblancEngine/Render/Basics/LeblancGeometry.h"

class DeviceD3D11
{
public:
	DeviceD3D11();

	~DeviceD3D11();
	
	void initialize(HWND window);
	
	void release();

	void present();

	ID3D11Resource* createTexture(TextureType texture_type, UINT width, UINT height);
	
	ID3DX10Mesh* createMesh(vector<Vertex>& vertices, vector<DWORD>& indices);

	bool initialized() { return m_device != nullptr; }

private:
	ID3D11Device* m_device = nullptr;
	ID3D11DeviceContext* m_device_context = nullptr;
	IDXGISwapChain* m_swap_chain = nullptr;
};


#endif