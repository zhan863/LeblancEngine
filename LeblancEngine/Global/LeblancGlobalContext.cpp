#include "LeblancEngine/Global/LeblancGlobalContext.h"

namespace Leblanc
{
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
		m_material_manager.initialize();
		m_pipeline_manager.initialize();
		m_render_state_manager.initialize();
		m_scene_manager.initialize();
		m_global_cb_manager.initialize();
	}

	void GlobalContext::release()
	{
	}
}
