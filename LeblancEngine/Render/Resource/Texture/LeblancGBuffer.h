#ifndef LEBLANC_GBUFFER_H
#define LEBLANC_GBUFFER_H
class Texture2D;
class DepthStencilTexture;

class GBuffer
{
public:
	GBuffer();
	~GBuffer();

	void initialize(UINT width, UINT height);

	void release();

	void bind();

	void unbind();
protected:
	Texture2D* buffer1;
	Texture2D* buffer2;
	Texture2D* buffer3;

	DepthStencilTexture* depth_stencil_texture;
};
#endif