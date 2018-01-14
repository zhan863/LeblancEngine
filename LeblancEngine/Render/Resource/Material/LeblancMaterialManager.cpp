#include "LeblancEngine/BasicInclude/LeblancPCH.h"
#include "LeblancEngine/Render/Resource/Material/LeblancMaterialManager.h"

MaterialManager::MaterialManager()
{

}

MaterialManager::~MaterialManager()
{

}

void MaterialManager::initialize()
{

}

void MaterialManager::release()
{
	for (UINT i = 0; i < m_global_materials.size(); i++)
	{
		m_global_materials[i].release();
	}
}