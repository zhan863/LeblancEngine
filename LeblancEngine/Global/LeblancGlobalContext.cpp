#include "LeblancEngine/Global/LeblancGlobalContext.h"

GlobalContext g_global_context;

GlobalContext::GlobalContext()
{

}

GlobalContext::~GlobalContext()
{

}

void GlobalContext::initialize(Window& window)
{
	m_device_manager.initialize(window);
	m_pipeline_manager.initialize();
	m_material_manager.initialize();
	m_resource_manager.initialize(window);
	m_render_state_manager.initialize();
	m_scene_manager.initialize();
}

void GlobalContext::release()
{
	m_pipeline_manager.release();
	m_resource_manager.release();
	m_render_state_manager.release();
	m_material_manager.release();
}