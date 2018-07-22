#ifndef LEBLANC_SURFACE_D3D11_H
#define LEBLANC_SURFACE_D3D11_H

#include "LeblancEngine/Render/Texture/LeblancSurface.h"
#include "LeblancEngine/Render/Include/LeblancDirectInclude.h"

namespace Leblanc
{
	class ITexture;
	class DeviceContextD3D11;
	class TextureD3D11;
	class SurfaceD3D11 : public ISurface
	{
	public:
		SurfaceD3D11(IDevice* device);

		~SurfaceD3D11();

		virtual bool initialize(int firstLevel = 0, int numberOfLevels = 1, ITexture* texture = 0, int mipLevel = -1);

		virtual unsigned int width() const;
		virtual unsigned int height() const;		
		virtual const ITexture* texture() const;
		ID3D11RenderTargetView* renderTargetView() const;

		virtual bool bind(IDeviceContext* device_context, unsigned int level = 0) const;
		virtual bool bindAndClear(IDeviceContext* device_context, float* color, unsigned int level = 0) const;

	protected:
		void release();
		virtual bool create();

		// reference
		unsigned int m_first_array_index = 0;
		unsigned int m_array_size = 1;
		int m_mip_level = -1;
		TextureD3D11* m_texture = nullptr;

		ID3D11Texture2D* m_resource = nullptr;
		D3D11_TEXTURE2D_DESC m_texture_desc;

		// data
		ID3D11RenderTargetView* m_render_target_view = nullptr;
		std::vector<ID3D11RenderTargetView*> m_mip_render_target_views;
		D3D11_RENDER_TARGET_VIEW_DESC m_render_target_desc;
	};
}

#endif