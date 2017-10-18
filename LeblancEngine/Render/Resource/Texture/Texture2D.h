#ifndef LEBLANC_TEXTURE_2D_H
#define LEBLANC_TEXTURE_2D_H

class Texture2D : public Texture 
{
public:
	Texture2D();

	~Texture2D();

	virtual void intialize();

	virtual void release();

	virtual void bind();

	virtual void bind();
};

#endif