#ifndef LEBLANC_GLOBAL_CONTEXT_H
#define LEBLANC_GLOBAL_CONTEXT_H

#include "LeblancEngine/Render/Utility/LeblancDeviceManager.h"
#include "LeblancEngine/Render/Pipeline/LeblancPipelineManager.h"

class GlobalContext
{
public:
	GlobalContext();
	~GlobalContext();

	void initialize();

	void release();
	
public:
	DeviceManager* m_device_manager;
	PipelineManager* m_pipeline_manager;

};

GlobalContext g_leblanc_engine_global_context;
#endif