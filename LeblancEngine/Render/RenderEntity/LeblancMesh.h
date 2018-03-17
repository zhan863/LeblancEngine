#ifndef LEBLANC_MESH_H
#define LEBLANC_MESH_H

#include "LeblancEngine/BasicInclude/LeblancPCH.h"
#include "LeblancEngine/Render/Basics/LeblancRenderBasicDefine.h"
#include "LeblancEngine/Render/Basics/LeblancRenderPlatformDefineD3D11.h"

#include <assimp/Include/scene.h>

class Mesh
{
public:
	Mesh();
	virtual ~Mesh();

protected:
	void release();

	void createVertexBuffer();

	virtual void* internalStreamPtr() = 0;

	void setVertexDeclaration(const VertexDeclarationD3D11* declaration);

	void setVertexCount(size_t vertex_count);

	size_t getVertexCount();

	void setPrimitiveType(PrimitiveType primitive_type);

	PrimitiveType getPrimitiveType();

	void setVertexCount(uint32_t vertex_count) { m_vertex_count = vertex_count; }

	uint32_t vertexCount() { return m_vertex_count; }

	uint32_t vertexBufferSize();

	// data
	size_t m_vertex_count = 0;

	PrimitiveType m_primitive_type = PrimitiveType::TriangleList;

	VertexBufferD3D11* m_vertex_buffer = nullptr;

	// reference
	const VertexDeclarationD3D11* m_vertex_declaration = nullptr;
};
#endif