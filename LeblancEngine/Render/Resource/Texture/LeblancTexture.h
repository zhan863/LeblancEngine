#ifndef LEBLANC_TEXTURE_H
#define LEBLANC_TEXTURE_H

#include "LeblancEngine/Render/Include/LeblancDirectInclude.h"

enum TextureType
{
	Texture_2D,
	DepthStencilTexture,
	Texture_3D
};

class Texture
{
public:
	Texture();
	~Texture();

	virtual bool initialize(UINT width, UINT height) = 0;

	virtual void release() = 0;

	virtual ID3D11Resource* getResource() = 0;
protected:
};
#endif