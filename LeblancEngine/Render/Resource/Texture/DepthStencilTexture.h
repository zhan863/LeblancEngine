#ifndef LEBLANC_DEPTH_STENCIL_TEXTURE_H
#define LEBLANC_DEPTH_STENCIL_TEXTURE_H

#include "LeblancEngine/Render/Include/LeblancDirectInclude.h"
#include "LeblancEngine/Render/Resource/Texture/LeblancTexture.h"

class DepthStencilTexture : public Texture
{
public:
	DepthStencilTexture();

	~DepthStencilTexture();

	virtual bool intialize(UINT width, UINT height, BOOL is_render_target);

	virtual void release();

	virtual ID3D11Resource* getResource();

	virtual ID3D11View* getRenderTargetView();

protected:
	ID3D11Texture2D* m_depth_stencil_texture = nullptr;
	ID3D11DepthStencilView* m_depth_stencil_view = nullptr;
};

#endif