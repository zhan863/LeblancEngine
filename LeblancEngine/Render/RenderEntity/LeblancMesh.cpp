#include "LeblancEngine/Render/RenderEntity/LeblancMesh.h"

Mesh::Mesh()
{

}

Mesh::~Mesh()
{
	release();
}

void Mesh::release()
{
}

void Mesh::buildVertexBuffer()
{

}

void Mesh::setVertexDeclaration()
{

}

void Mesh::setVertexCount(size_t vertex_count)
{
	m_vertex_count = vertex_count;
}

size_t Mesh::getVertexCount()
{
	return m_vertex_count;
}


void Mesh::setPrimitiveType(PrimitiveType primitive_type)
{
	m_primitive_type = primitive_type;
}

PrimitiveType Mesh::getPrimitiveType()
{
	return m_primitive_type;
}