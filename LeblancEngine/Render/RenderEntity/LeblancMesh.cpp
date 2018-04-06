#include "LeblancEngine/Render/RenderEntity/LeblancMesh.h"
#include "LeblancEngine/BasicInclude/LeblancMemoryOperation.h"
#include "LeblancEngine/Global/LeblancGlobalContext.h"

Mesh::Mesh()
{

}

Mesh::~Mesh()
{
	release();
}

void Mesh::release()
{
	safe_delete(m_vertex_buffer);
	safe_delete(m_material);
}

void Mesh::createVertexBuffer()
{
	safe_delete(m_vertex_buffer);

	void* vertex_data = internalStreamPtr();
	VertexBufferDeclaration declaration = VertexBufferDeclaration(vertexBufferSize(), m_vertex_declaration->vertexStride(), vertex_data);

	DeviceD3D11* device = g_global_context.m_device_manager.getCurrentDevice();

	if(m_vertex_buffer = device->createVertexBuffer(&declaration))
		m_vertex_buffer->setVertexDeclaration(m_vertex_declaration);
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

void Mesh::setVertexDeclaration(const VertexDeclarationD3D11* declaration)
{
	m_vertex_declaration = declaration;
}

uint32_t Mesh::vertexBufferSize()
{
	if (m_vertex_declaration)
	{
		return (uint32_t)(m_vertex_count * (m_vertex_declaration->vertexStride()));
	}

	return 0;
}