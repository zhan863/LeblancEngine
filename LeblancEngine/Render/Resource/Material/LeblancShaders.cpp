#include "LeblancEngine/Render/Resource/Material/LeblancShaders.h"

Shader::Shader()
{

}

Shader::~Shader()
{

}

void Shader::initialize()
{

}

void Shader::release()
{

}

VertexShader::VertexShader()
{

}

VertexShader::~VertexShader()
{

}

void VertexShader::initialize()
{

}

void VertexShader::release()
{
	if (m_vertex_shader)
	{
		m_vertex_shader->Release();
		m_vertex_shader = nullptr;
	}
}

PixelShader::PixelShader()
{

}

PixelShader::~PixelShader()
{

}

void PixelShader::initialize()
{

}

void PixelShader::release()
{
	if (m_pixel_shader)
	{
		m_pixel_shader->Release();
		m_pixel_shader = nullptr;
	}
}

ComputeShader::ComputeShader()
{

}

ComputeShader::~ComputeShader()
{

}

void ComputeShader::initialize()
{

}

void ComputeShader::release()
{
	if (m_compute_shader)
	{
		m_compute_shader->Release();
		m_compute_shader = nullptr;
	}
}