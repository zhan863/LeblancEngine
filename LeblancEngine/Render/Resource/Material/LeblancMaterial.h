#ifndef LEBLANC_MATERIAL_H
#define LEBLANC_MATERIAL_H
#include "LeblancEngine/Render/Resource/Material/LeblancMaterial.h"
#include "LeblancEngine/Render/Resource/Material/LeblancShaders.h"
#include <string>

using namespace std;

class LeblancMaterial
{
public:
	LeblancMaterial();
	~LeblancMaterial();

	void initialize();

	void release();

	VertexShader m_vertex_shader;
	PixelShader m_pixel_shader;
	ComputeShader m_compute_shader;
};

class Shader;
class Technique;
class Material
{
public:
	Material();
	~Material();

	void release();

	void initialize(string file_name);

private:
	// Reference
	Shader * m_shader = nullptr;
	Technique* m_technique = nullptr;

};
#endif