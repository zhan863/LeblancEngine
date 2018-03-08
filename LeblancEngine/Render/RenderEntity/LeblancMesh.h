#ifndef LEBLANC_MESH_H
#define LEBLANC_MESH_H

#include "LeblancEngine/BasicInclude/LeblancPCH.h"
#include "LeblancEngine/Render/Basics/LeblancRenderBasicDefine.h"

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

	void setVertexCount(size_t vertex_count);

	size_t getVertexCount();

	void setPrimitiveType(PrimitiveType primitive_type);

	PrimitiveType getPrimitiveType();

	size_t m_vertex_count = 0;

	PrimitiveType m_primitive_type = PrimitiveType::TriangleList;
};
#endif