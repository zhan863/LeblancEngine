#ifndef LEBLANC_SHADER_H
#define LEBLANC_SHADER_H
#include "LeblancEngine/BasicInclude/LeblancPCH.h"
#include <map>

using namespace std;

class LeblancShader
{
public:
	LeblancShader(){}
	~LeblancShader(){}

	virtual void initialize() = 0;

	virtual void release() = 0;

	virtual void setShader(void* shader) = 0;

	void setBlob(ID3D10Blob* blob) { m_blob = blob; }

	ID3D10Blob* getBlob() { return m_blob; }

protected:
	ID3D10Blob* m_blob = nullptr;
};

class VertexShader : public LeblancShader
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

class PixelShader : public LeblancShader
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

class ComputeShader : public LeblancShader
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




class Technique;
class GpuVariable;
class ConstantBuffer;
class DeviceD3D11;
class Shader
{
public:
	Shader(DeviceD3D11* device);
	~Shader();

	void release();
	void initialize(string file_name, string include_file_name);

	void setIncludeName(string include_name) { m_include_name = include_name; }
	void setFileName(string file_name) { m_file_name = file_name; }

	std::string getIncludeName() { return m_include_name; }
	std::string getFileName() { return m_file_name; }

	void setMatrix(string name, const float* matrix); 
	void setVector(string name, const float* vector);
	void setScalar(string name, const float* value);	
	
	void setMatrixArray(string name, const float* matrix, int count);
	void setVectorArray(string name, const float* vector, int count);
	void setScalarArray(string name, const float* value, int count);

private:
	void enumerateVariables(D3DX11_EFFECT_DESC& desc);
	void enumerateTechniques(D3DX11_EFFECT_DESC& desc);

	// data
	std::map<string, Technique*> m_techniques;
	std::map<string, GpuVariable*> m_gpu_variables;
	std::map<string, ConstantBuffer*> m_global_constant_buffers;

	ID3DX11Effect* m_effect_handle = nullptr;

	// reference
	std::string m_file_name;
	std::string m_include_name;

	DeviceD3D11* m_device = nullptr;
};
#endif