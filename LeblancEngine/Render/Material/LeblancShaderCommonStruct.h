#ifndef LEBLANC_GLOBAL_CB_MANAGER_H
#define LEBLANC_GLOBAL_CB_MANAGER_H

#include "LeblancEngine/Math/LeblancMath.h"

namespace Leblanc 
{
	struct PerFrameBuffer
	{
		XMVECTOR per_frame_time_info;
	};

	struct PerViewBuffer
	{
		XMMATRIX per_view_world_view_matrix;
		XMMATRIX per_view_view_proj_matrix;
		XMMATRIX per_view_world_proj_matrix;
	};

	struct PerRigidObjectBuffer
	{
		XMMATRIX per_object_object_world_matrix;
	};
}
#endif