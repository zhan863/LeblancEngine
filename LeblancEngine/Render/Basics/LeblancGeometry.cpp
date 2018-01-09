#include "LeblancEngine/Render/Basics/LeblancGeometry.h"

LeblancMesh::LeblancMesh()
{
	
}

LeblancMesh::~LeblancMesh()
{
	release();
}

void LeblancMesh::release()
{
	if (m_index_buffer)
		m_index_buffer->Release();

	if (m_vertex_buffer)
		m_vertex_buffer->Release();
}