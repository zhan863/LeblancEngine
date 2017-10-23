#include "LeblancEngine/Global/LeblancGlobalContext.h"

GlobalContext::GlobalContext()
{

}

GlobalContext::~GlobalContext()
{

}

void GlobalContext::initialize()
{

}

void GlobalContext::release()
{
	m_device_manager->release();
	m_pipeline_manager->release();
}