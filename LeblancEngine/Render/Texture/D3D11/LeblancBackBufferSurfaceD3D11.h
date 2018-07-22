#ifndef LEBLANC_BACKBUFFER_SURFACE_D3D11_H
#define LEBLANC_BACKBUFFER_SURFACE_D3D11_H

#include "LeblancEngine/Render/Texture/D3D11/LeblancSurfaceD3D11.h"
#include "LeblancEngine/Render/Texture/LeblancTextureParameters.h"

struct IDXGISwapChain;
namespace Leblanc
{
	class BackBufferSurfaceD3D11 : public SurfaceD3D11
	{
	public:
		BackBufferSurfaceD3D11(IDevice* device, IDXGISwapChain* swap_chain);
		virtual ~BackBufferSurfaceD3D11();

	protected:
		void     				   release();
		virtual bool               create();

		IDXGISwapChain*			   m_swap_chain = nullptr;
	};
}

#endif