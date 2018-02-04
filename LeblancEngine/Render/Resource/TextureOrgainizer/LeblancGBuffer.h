#ifndef LEBLANC_GBUFFER_H
#define LEBLANC_GBUFFER_H

#include "LeblancEngine/Render/Include/LeblancDirectInclude.h"

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

	Texture2D* m_normal_smoothness_buffer = nullptr;
	Texture2D* m_albedo_buffer = nullptr;
	Texture2D* m_metal_reflectance_ao_buffer = nullptr;
	Texture2D* m_radiosity_buffer = nullptr;

	DepthStencilTexture* m_depth_stencil_texture_buffer = nullptr;
};
#endif