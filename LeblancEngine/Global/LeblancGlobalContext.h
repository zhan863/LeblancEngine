#ifndef LEBLANC_GLOBAL_CONTEXT_H
#define LEBLANC_GLOBAL_CONTEXT_H

#include "LeblancEngine/Render/Device/LeblancDeviceManager.h"
#include "LeblancEngine/Render/Pipeline/LeblancPipelineManager.h"
#include "LeblancEngine/Render/RenderState/LeblancRenderStateManager.h"
#include "LeblancEngine/Render/Scene/LeblancSceneManager.h"
#include "LeblancEngine/Render/Material/LeblancMaterialManager.h"
#include "LeblancEngine/Render/Window/LeblancWindow.h"
#include "LeblancEngine/Asset/LeblancAssetManager.h"
#include "LeblancEngine/Render/Material/LeblancGlobalCBManager.h"

namespace Leblanc
{
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
		RenderStateManager m_render_state_manager;
		SceneManager m_scene_manager;
		MaterialManager m_material_manager;
		GlobalCBManager m_global_cb_manager;
		AssetManager m_asset_manager;
	};

	extern GlobalContext g_global_context;
}
#endif