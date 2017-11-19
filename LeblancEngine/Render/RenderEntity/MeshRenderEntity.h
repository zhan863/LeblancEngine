#ifndef LEBLANC_MESH_RENDER_ENTITY_H
#define LEBLANC_MESH_RENDER_ENTITY_H

#include "LeblancEngine/Render/RenderEntity/LeblancRenderEntity.h"

class MeshRenderEntity
{
public:
	MeshRenderEntity();
	~MeshRenderEntity();

	virtual void render(Pass pass);

protected:

};
#endif