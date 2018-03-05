#include "LeblancEngine/Render/Basics/LeblancGeometry.h"

Mesh::Mesh()
{
	
}

Mesh::~Mesh()
{
	release();
}

void Mesh::release()
{
	if (m_index_buffer)
		m_index_buffer->Release();

	if (m_vertex_buffer)
		m_vertex_buffer->Release();
}