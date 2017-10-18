#ifndef LEBLANC_TEXTURE_H
#define LEBLANC_TEXTURE_H

class Texture
{
public:
	Texture();
	~Texture();

	virtual void initialize(UINT width, UINT height) = 0;

	virtual void release() = 0;

	virtual void bind();

	virtual void unbind();
protected:
};
#endif