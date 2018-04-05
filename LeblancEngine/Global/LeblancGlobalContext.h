#ifndef LEBLANC_GLOBAL_CONTEXT_H
#define LEBLANC_GLOBAL_CONTEXT_H

#include "LeblancEngine/Render/Utility/LeblancDeviceManager.h"
#include "LeblancEngine/Render/Pipeline/LeblancPipelineManager.h"
#include "LeblancEngine/Render/Resource/LeblancResourceManager.h"
#include "LeblancEngine/Render/Resource/LeblancRenderStateManager.h"
#include "LeblancEngine/Render/Scene/LeblancSceneManager.h"
#include "LeblancEngine/Render/Resource/Material/LeblancMaterialManager.h"
#include "LeblancEngine/Render/Basics/LeblancWindow.h"
#include "LeblancEngine/Asset/LeblancAssetManager.h"

class GlobalContext
{
public:
	GlobalContext();
	~GlobalContext();

	void initialize(Window& window);

	void release();
	
public:
	DeviceManager m_device_manager;
	PipelineManager m_pipeline_manager;
	ResourceManager m_resource_manager;
	RenderStateManager m_render_state_manager;
	SceneManager m_scene_manager;
	MaterialManager m_material_manager;
	AssetManager m_asset_manager;
};

extern GlobalContext g_global_context;
#endif