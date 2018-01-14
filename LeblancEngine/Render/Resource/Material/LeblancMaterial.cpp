#include "LeblancEngine/Render/Resource/Material/LeblancMaterial.h"

Material::Material()
{

}

Material::~Material()
{

}

void Material::initialize()
{

}

void Material::release()
{
	m_vertex_shader.release();
	m_pixel_shader.release();
	m_compute_shader.release();
}