#ifndef LEBLANC_MESH_RENDER_ENTITY_H
#define LEBLANC_MESH_RENDER_ENTITY_H

#include "LeblancEngine/Render/RenderEntity/LeblancRenderEntity.h"
#include "LeblancEngine/Render/Basics/LeblancRenderBasicDefine.h"

class MeshRenderEntity
{
public:
	MeshRenderEntity();
	~MeshRenderEntity();

	virtual void render(Pass pass);

protected:

};
#endif