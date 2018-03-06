#ifndef LEBLANC_MESH_H
#define LEBLANC_MESH_H

#include "LeblancEngine/BasicInclude/LeblancPCH.h"

#include <assimp/Include/scene.h>

class Mesh
{
public:
	Mesh();
	~Mesh();

	void release();

protected:
	void buildVertexBuffer();

	void setVertexDeclaration();
};
#endif