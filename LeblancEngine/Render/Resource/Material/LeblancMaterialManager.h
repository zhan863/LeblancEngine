#ifndef LEBLANC_MATERIAL_MANAGER_H
#define LEBLANC_MATERIAL_MANAGER_H

#include "LeblancEngine/Render/Resource/Material/LeblancMaterial.h"
#include "LeblancEngine/Render/Basics/LeblancRenderBasicDefine.h"
#include <vector>

using namespace std;

enum class GlobalMaterialType : UINT
{
	ShadowMap,
	GBuffer,
	DeferredShading,
	Postprocessing
};

class MaterialManager
{
public:
	MaterialManager();
	~MaterialManager();

	void initialize();

	void release();

	Material* getGlobalPassMaterial(Pass pass);

protected:
	void loadShadowMapMaterial();

	void loadGBufferMaterial();

	void loadDeferredShadingMaterial();

	void loadPostProcessingMaterial();

	void compileMaterial(LPCWSTR material_file_name, Material& material, LPCSTR vs, LPCSTR ps);

	vector<Material> m_global_materials;
};
#endif