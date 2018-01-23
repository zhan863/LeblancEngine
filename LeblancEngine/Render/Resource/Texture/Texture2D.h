#ifndef LEBLANC_TEXTURE_2D_H
#define LEBLANC_TEXTURE_2D_H

#include "LeblancEngine/Render/Include/LeblancDirectInclude.h"
#include "LeblancEngine/Render/Resource/Texture/LeblancTexture.h"

class Texture2D : public Texture 
{
public:
	Texture2D();

	~Texture2D();

	virtual bool intialize(UINT width, UINT height);

	virtual void release();

	virtual ID3D11Resource* getResource();

	virtual ID3D11View* getRenderTargetView();

protected:
	ID3D11Texture2D* m_d3d11_texture2d = nullptr;
	ID3D11RenderTargetView* m_d3d11_render_target_view = nullptr;
};

#endif