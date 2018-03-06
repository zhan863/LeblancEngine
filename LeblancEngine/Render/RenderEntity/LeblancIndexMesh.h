#ifndef LEBLANC_INDEX_MESH_H
#define LEBLANC_INDEX_MESH_H

#include "LeblancEngine/BasicInclude/LeblancPCH.h"
#include "LeblancEngine/Render/RenderEntity/LeblancStreamMesh.h"

class IndexMesh : public StreamMesh
{
public:
	IndexMesh();
	~IndexMesh();

	void release();

	void load(const aiMesh* mesh);

protected:
};
#endif