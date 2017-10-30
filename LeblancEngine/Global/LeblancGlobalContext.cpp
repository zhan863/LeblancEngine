#include "LeblancEngine/Global/LeblancGlobalContext.h"

GlobalContext::GlobalContext()
{

}

GlobalContext::~GlobalContext()
{

}

void GlobalContext::initialize(HWND window)
{
	m_device_manager.initialize(window);
	m_pipeline_manager.initialize();
	m_resource_manager.initialize();
	m_render_state_manager.initialize();
}

void GlobalContext::release()
{
	m_device_manager.release();
	m_pipeline_manager.release();
	m_resource_manager.release();
	m_render_state_manager.release();
}