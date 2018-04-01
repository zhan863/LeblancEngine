#ifndef LEBLANC_MATERIAL_MANAGER_H
#define LEBLANC_MATERIAL_MANAGER_H

#include <vector>

using namespace std;

class Shader;

class MaterialManager
{
public:
	MaterialManager();
	~MaterialManager();

	void initialize();

	void release();

private:
	// data
	vector<Shader*> m_shaders;
};
#endif