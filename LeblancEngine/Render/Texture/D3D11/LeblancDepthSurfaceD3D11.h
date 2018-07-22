#ifndef LEBLANC_DEPTH_SURFACE_D3D11_H
#define LEBLANC_DEPTH_SURFACE_D3D11_H

#include "LeblancEngine/Render/Texture/LeblancDepthSurface.h"
#include "LeblancEngine/Render/Texture/LeblancTextureParameters.h"

namespace Leblanc
{
	class DepthSurfaceD3D11 : public IDepthSurface
	{
	public:
		DepthSurfaceD3D11(IDevice* device);
		virtual ~DepthSurfaceD3D11();

		virtual bool               initialize(const DepthSurfaceParameters*);
		virtual bool               bind(IDeviceContext* device_context) const;

		virtual bool               clear(IDeviceContext* device_context);
		virtual bool               clearStencil(IDeviceContext* device_context);

		virtual int                width() const;
		virtual int                height() const;
		virtual int                multiSampleCount() const;
		virtual int                multiSampleQuality() const;

		ID3D11DepthStencilView *   depthStencilView() const;
		ID3D11Texture2D*           depthTexture() const;
		ID3D11ShaderResourceView*  shaderResourceView() const;
		D3D11_TEXTURE2D_DESC       textureDesc() const;

	protected:
		void					   release();
		virtual bool               create();

		DepthSurfaceParameters     m_depth_surface_parameters;
		D3D11_DEPTH_STENCIL_VIEW_DESC m_depth_view_desc;
		D3D11_TEXTURE2D_DESC       m_texture_desc;
		
		// data
		ID3D11DepthStencilView*    m_depth_surface = nullptr;
		ID3D11ShaderResourceView*  m_depth_shader_resource_view = nullptr;
		ID3D11Texture2D*           m_depth_texture = nullptr;
	};
}

#endif