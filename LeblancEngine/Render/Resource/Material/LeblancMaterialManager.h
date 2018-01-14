#ifndef LEBLANC_MATERIAL_MANAGER_H
#define LEBLANC_MATERIAL_MANAGER_H

#include "LeblancEngine/Render/Resource/Material/LeblancMaterial.h"
#include <vector>

using namespace std;

class MaterialManager
{
public:
	MaterialManager();
	~MaterialManager();

	void initialize();

	void release();

protected:

	vector<Material> m_global_materials;
};
#endif