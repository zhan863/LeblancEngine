#ifndef LEBLANC_SHADER_H
#define LEBLANC_SHADER_H
#include "LeblancEngine/BasicInclude/LeblancPCH.h"

class Shader
{
public:
	Shader();
	~Shader();

	virtual void initialize() = 0;

	virtual void release() = 0;

	virtual void setShader(void* shader) = 0;

protected:
};

class Shader;
class VertexShader : public Shader
{
public:
	VertexShader();
	~VertexShader();

	virtual void initialize();
	virtual void release();

	virtual void setShader(void* shader);

	ID3D11VertexShader* getVertexShader() { return m_vertex_shader; }

protected:
	ID3D11VertexShader* m_vertex_shader = nullptr;
};

class PixelShader : public Shader
{
public:
	PixelShader();
	~PixelShader();

	virtual void initialize();
	virtual void release();

	virtual void setShader(void* shader);

	ID3D11PixelShader* getPixelShader() { return m_pixel_shader; }

protected:
	ID3D11PixelShader* m_pixel_shader = nullptr;
};

class ComputeShader : public Shader
{
public:
	ComputeShader();
	~ComputeShader();

	virtual void initialize();
	virtual void release();

	virtual void setShader(void* shader);

	ID3D11ComputeShader* getComputeShader() { return m_compute_shader; }

protected:
	ID3D11ComputeShader* m_compute_shader = nullptr;
};
#endif