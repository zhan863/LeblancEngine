#ifndef LEBLANC_MATERIAL_H
#define LEBLANC_MATERIAL_H
#include "LeblancEngine/Render/Resource/Material/LeblancMaterial.h"
#include "LeblancEngine/Render/Resource/Material/LeblancShaders.h"

class Material
{
public:
	Material();
	~Material();

	void initialize();

	void release();

	VertexShader m_vertex_shader;
	PixelShader m_pixel_shader;
	ComputeShader m_compute_shader;
};
#endif