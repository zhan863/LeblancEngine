#ifndef LEBLANC_GLOBAL_CONTEXT_H
#define LEBLANC_GLOBAL_CONTEXT_H

#include "LeblancEngine/Render/Utility/LeblancDeviceManager.h"
#include "LeblancEngine/Render/Pipeline/LeblancPipelineManager.h"
#include "LeblancEngine/Render/Resource/LeblancResourceManager.h"
#include "LeblancEngine/Render/Resource/LeblancRenderStateManager.h"

class GlobalContext
{
public:
	GlobalContext();
	~GlobalContext();

	void initialize(HWND window);

	void release();
	
public:
	DeviceManager m_device_manager;
	PipelineManager m_pipeline_manager;
	ResourceManager m_resource_manager;
	RenderStateManager m_render_state_manager;

};

extern GlobalContext g_global_context;
#endif