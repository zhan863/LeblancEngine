#ifndef LEBLANC_MATERIAL_MANAGER_H
#define LEBLANC_MATERIAL_MANAGER_H

#include <map>

using namespace std;

class Shader;

class MaterialManager
{
public:
	MaterialManager();
	~MaterialManager();

	void initialize();

	void release();

	Shader* getShaderByFilePath(string filename);

private:
	// data
	map<string, Shader*> m_shaders;
};
#endif