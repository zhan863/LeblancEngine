#ifndef LEBLANC_RENDER_ENTITY_H
#define LEBLANC_RENDER_ENTITY_H

#include "LeblancEngine/BasicInclude/LeblancPCH.h"
#include "LeblancEngine/Render/Basics/LeblancGeometry.h"
#include "LeblancEngine/Render/Basics/LeblancRenderBasicDefine.h"

class RenderEntity
{
public:
	RenderEntity();
	~RenderEntity();

	void release();

	void setTransform(XMFLOAT4X4 transform)
	{
		m_transform = transform;
	}

protected:
	// resource
	vector<Mesh*> m_meshes;

	// object data
	XMFLOAT4X4 m_transform;
};
#endif