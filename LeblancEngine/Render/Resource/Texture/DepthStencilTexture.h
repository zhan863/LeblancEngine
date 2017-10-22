#ifndef LEBLANC_DEPTH_STENCIL_TEXTURE_H
#define LEBLANC_DEPTH_STENCIL_TEXTURE_H

#include "LeblancEngine/Render/Include/LeblancDirectInclude.h"
#include "LeblancEngine/Render/Resource/Texture/LeblancTexture.h"

class DepthStencilTexture : public Texture
{
public:
	DepthStencilTexture();

	~DepthStencilTexture();

	virtual bool intialize(ID3D11Device* device, UINT width, UINT height);

	virtual void release();

	virtual ID3D11Resource* getResource();

protected:
	ID3D11Texture2D* m_depth_stencil_texture = nullptr;
};

#endif