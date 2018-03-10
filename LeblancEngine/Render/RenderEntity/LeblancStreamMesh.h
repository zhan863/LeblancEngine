#ifndef LEBLANC_STREAM_MESH_H
#define LEBLANC_STREAM_MESH_H

#include "LeblancEngine/Render/RenderEntity/LeblancMesh.h"

class StreamMesh : public Mesh
{
public:
	StreamMesh();
	virtual ~StreamMesh();

protected:
	void release();
};
#endif