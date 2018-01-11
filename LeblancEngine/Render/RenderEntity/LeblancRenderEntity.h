#ifndef LEBLANC_RENDER_ENTITY_H
#define LEBLANC_RENDER_ENTITY_H

#include "LeblancEngine/BasicInclude/LeblancPCH.h"
#include "LeblancEngine/Render/Basics/LeblancGeometry.h"

enum class Pass
{
	GBuffer,
	DeferredShading,
	SingleForward,
	Transparent,
	DepthOnly
};

class RenderEntity
{
public:
	RenderEntity();
	~RenderEntity();

	virtual void createFromFile(const WCHAR* file_name);

	void release();

	virtual void render(Pass pass);

	void setTransform(XMFLOAT4X4 transform)
	{
		m_transform = transform;
	}

protected:
	LeblancMesh* m_mesh = nullptr;
	XMFLOAT4X4 m_transform;
};
#endif