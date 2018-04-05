#ifndef LEBLANC_MATERIAL_H
#define LEBLANC_MATERIAL_H
#include "LeblancEngine/Render/Resource/Material/LeblancMaterial.h"
#include "LeblancEngine/Render/Resource/Material/LeblancShaders.h"
#include <string>
#include <map>

using namespace std;

class Shader;
class Technique;
class GpuData;
class Material
{
public:
	Material();
	~Material();

	void release();

	void initialize(string file_name);

private:
	GpuData * getGpuDataFromString(string type_name, string value);

	// Reference
	Shader * m_shader = nullptr;
	string m_technique_name;
	string m_pass_name;

	// Data
	map<string, GpuData*> m_parameters;
	map<string, string> m_textures;

};
#endif