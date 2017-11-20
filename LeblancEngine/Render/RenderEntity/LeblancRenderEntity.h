#ifndef LEBLANC_RENDER_ENTITY_H
#define LEBLANC_RENDER_ENTITY_H

#include "LeblancEngine/BasicInclude/LeblancPCH.h"

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

	virtual void createFromFile(char* file_name);

	void release();

	virtual void render(Pass pass);

protected:
	ID3DX10Mesh* m_mesh = nullptr;
};
#endif