#ifndef LEBLANC_RENDER_TARGET_TEXTURE_D3D11_H
#define LEBLANC_RENDER_TARGET_TEXTURE_D3D11_H

#include "LeblancEngine/Render/Texture/D3D11/LeblancTextureD3D11.h"

namespace Leblanc
{
	class ISurface;
	class SurfaceD3D11;
	class IDeviceContext;
	class RenderTargetTextureD3D11 : public TextureD3D11
	{
	public:
		RenderTargetTextureD3D11(IDevice* device);
		virtual ~RenderTargetTextureD3D11();

		virtual bool bindSurface(IDeviceContext* device_context, int render_target_index) const;
		virtual bool bindClearSurface(IDeviceContext* device_context, uint32_t render_target_index, float* color);

		virtual const ISurface* surface(int32_t arrayId = -1, int32_t mipId = -1) const;
		virtual ID3D11ShaderResourceView* resourceView(int32_t arrayId = -1, int32_t mipId = -1) const;

	protected:
		void					   release();
		virtual bool               create();

		// Reference
		D3D11_TEXTURE2D_DESC m_texture_desc;
		int m_active_array_index = -1;
		int m_active_mip_index = -1;

		// data
		
		// surface for all array in mip 0
		SurfaceD3D11* m_surface = nullptr;

		// srv for each array element for all mips;
		vector<ID3D11ShaderResourceView*> m_array_slice_views;
		vector<SurfaceD3D11*> m_array_slice_surfaces;
		
		// srv for each mip element for all array slice
		vector<ID3D11ShaderResourceView*> m_mip_slice_views;
		vector<SurfaceD3D11*> m_mip_slice_surfaces;

		// srv for each array for each mip slice
		map<size_t, std::vector<ID3D11ShaderResourceView*>> m_array_mip_slice_views;
		map<size_t, std::vector<SurfaceD3D11*>> m_array_mip_slice_surfaces;
	};
}

#endif