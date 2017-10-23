#ifndef LEBLANC_DEVICE_D3D11_H
#define LEBLANC_DEVICE_D3D11_H

#include "LeblancEngine/Render/Include/LeblancDirectInclude.h"
class DeviceD3D11
{
public:
	DeviceD3D11();

	~DeviceD3D11();
	
	void initialize();
	
	void release();

	ID3D11Resource* createTexture();

private:
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_device_context;
};


#endif