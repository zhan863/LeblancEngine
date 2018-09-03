#ifndef LEBLANC_SHADER_COMMON_STRUCT_H
#define LEBLANC_SHADER_COMMON_STRUCT_H

#include <vector>
#include "LeblancEngine/Render/Material/LeblancShaderCommonStruct.h"
#include "LeblancEngine/Render/Material/LeblancShaderCommonStruct.h"

using namespace std;

namespace Leblanc
{
	class IDevice;
	class IDeviceContext;
	class ConstantBuffer;
	class GlobalCBManager
	{
	public:
		GlobalCBManager();
		~GlobalCBManager();

		void initialize();

		void updatePerFrameCB();

		void updatePerViewCB();

		void updatePerRigidObjectCB();

	private:

		void release();

		// data
		PerFrameBuffer m_per_frame_buffer = {};
		PerViewBuffer m_per_view_buffer = {};
		PerRigidObjectBuffer m_per_rigit_object_buffer = {};

		ConstantBuffer* m_per_frame_cb = nullptr;
		ConstantBuffer* m_per_view_cb = nullptr;
		ConstantBuffer* m_per_rigid_object_cb = nullptr;
	};
}
#endif