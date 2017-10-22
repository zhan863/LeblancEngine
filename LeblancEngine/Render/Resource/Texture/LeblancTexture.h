#ifndef LEBLANC_TEXTURE_H
#define LEBLANC_TEXTURE_H

#include "LeblancEngine/Render/Include/LeblancDirectInclude.h"

class Texture
{
public:
	Texture();
	~Texture();

	virtual bool initialize(ID3D11Device* device, UINT width, UINT height) = 0;

	virtual void release() = 0;

	virtual ID3D11Resource* getResource() = 0;
protected:
};
#endif