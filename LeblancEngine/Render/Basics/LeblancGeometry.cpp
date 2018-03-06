#include "LeblancEngine/Render/Basics/LeblancGeometry.h"

Mesh1::Mesh1()
{
	
}

Mesh1::~Mesh1()
{
	release();
}

void Mesh1::release()
{
	if (m_index_buffer)
		m_index_buffer->Release();

	if (m_vertex_buffer)
		m_vertex_buffer->Release();
}