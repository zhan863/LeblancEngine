#include "LeblancEngine/Render/Material/LeblancGlobalCBManager.h"
#include "LeblancEngine/BasicInclude/LeblancMemoryOperation.h"
#include "LeblancEngine/Render/Device/LeblancDevice.h"
#include "LeblancEngine/Render/Material/LeblancConstantBuffer.h"
#include "LeblancEngine/Global/LeblancGlobalContext.h"

namespace Leblanc
{
	GlobalCBManager::GlobalCBManager()
	{
	}
		
	GlobalCBManager::~GlobalCBManager()
	{
		release();
	}

	void GlobalCBManager::initialize()
	{
		IDevice* device = g_global_context.m_device_manager.getDevice();
		m_per_frame_cb = device->createConstantBuffer(0, sizeof(PerFrameBuffer));
		m_per_view_cb = device->createConstantBuffer(1, sizeof(PerViewBuffer));
		m_per_rigid_object_cb = device->createConstantBuffer(2, sizeof(PerRigidObjectBuffer));
	}

	void GlobalCBManager::release()
	{
		safe_delete(m_per_rigid_object_cb);
		safe_delete(m_per_view_cb);
		safe_delete(m_per_frame_cb);
	}

	void GlobalCBManager::updatePerFrameCB()
	{
		m_per_frame_cb->update(&m_per_frame_buffer);
	}

	void GlobalCBManager::updatePerViewCB()
	{
		m_per_view_cb->update(&m_per_view_buffer);
	}

	void GlobalCBManager::updatePerRigidObjectCB()
	{
		m_per_rigid_object_cb->update(&m_per_rigit_object_buffer);
	}
}