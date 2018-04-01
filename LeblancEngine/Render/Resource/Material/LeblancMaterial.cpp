#include "LeblancEngine/Render/Resource/Material/LeblancMaterial.h"

LeblancMaterial::LeblancMaterial()
{

}

LeblancMaterial::~LeblancMaterial()
{

}

void LeblancMaterial::initialize()
{

}

void LeblancMaterial::release()
{
	m_vertex_shader.release();
	m_pixel_shader.release();
	m_compute_shader.release();
}





Material::Material()
{

}

Material::~Material()
{
	release();
}

void Material::release()
{

}

void Material::initialize(string file_name)
{

}