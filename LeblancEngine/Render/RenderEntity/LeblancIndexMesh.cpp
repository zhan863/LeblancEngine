#include "LeblancEngine/Render/RenderEntity/LeblancIndexMesh.h"
#include "LeblancEngine/Math/LeblancMath.h"
#include "LeblancEngine/Render/Basics/LeblancRenderPlatformDefineD3D11.h"
#include "LeblancEngine/BasicInclude/LeblancMemoryOperation.h"
#include "LeblancEngine/Global/LeblancGlobalContext.h"

IndexMesh::IndexMesh()
{

}

IndexMesh::~IndexMesh()
{
	release();
}

void IndexMesh::release()
{
	safe_delete_array(m_index_buffer_data);
	safe_delete(m_index_buffer);

	m_index_count = 0;
}

void IndexMesh::load(const aiMesh* mesh)
{
	if (mesh)
	{
		size_t vertex_count = mesh->mNumVertices;
		size_t index_count = mesh->mNumFaces * 3;

		Vector3f* position_ptr = new Vector3f[vertex_count];
		Vector3f* normal_ptr = new Vector3f[vertex_count];
		Vector2f* uv_ptr = new Vector2f[vertex_count];
		uint32_t* index_ptr = new uint32_t[index_count];

		memset(position_ptr, 0, sizeof(float) * 3 * vertex_count);
		memset(normal_ptr, 0, sizeof(float) * 3 * vertex_count);
		memset(uv_ptr, 0, sizeof(float) * 2 * vertex_count);
		memset(index_ptr, 0, sizeof(uint32_t) * index_count);

		if (mesh->HasPositions())
		{
			memcpy(position_ptr, mesh->mVertices, sizeof(float) * 3 * vertex_count);
		}

		if (mesh->HasNormals())
		{
			memcpy(position_ptr, mesh->mNormals, sizeof(float) * 3 * vertex_count);
		}

		if (mesh->HasTextureCoords())
		{
			for (int i = 0; i < vertex_count; i++)
			{
				uv_ptr[i].x = mesh->mTextureCoords[0][i].x;
				uv_ptr[i].y = mesh->mTextureCoords[0][i].y;
			}
		}

		size_t triangle_count = mesh->mNumFaces;
		for (int i = 0; i < triangle_count; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				index_ptr[i * 3 + j] = mesh->mFaces[i].mIndices[j];
			}
		}

		setIndices(index_ptr, index_count, triangle_count);

		setVertexCount(vertex_count);

		setPrimitiveType(PrimitiveType::TriangleList);

		// release all data created during creation
		safe_delete_array(index_ptr);
	}
}

void IndexMesh::createIndexBuffer(size_t index_count)
{
	IndexBufferDeclaration declaration(index_count);
	if (m_index_buffer)
	{
		safe_delete(m_index_buffer);
	}

	DeviceD3D11* device = g_global_context.m_device_manager.getCurrentDevice();
	m_index_buffer = device->createIndexBuffer(&declaration);

	fillIndexBuffer();
}

void IndexMesh::fillIndexBuffer()
{
	if (m_index_buffer)
	{

	}
}

void IndexMesh::setIndices(const uint32_t* indices, size_t index_count, size_t face_count)
{
	if (index_count > m_index_count)
	{
		safe_delete_array(m_index_buffer_data);
		m_index_buffer_data = new uint32_t[index_count];
		createIndexBuffer(index_count);
	}
	m_index_count = index_count;

	if (m_index_buffer_data)
		memcpy(m_index_buffer_data, indices, index_count * sizeof(uint32_t));
	else
		return;// log the memory error

	fillIndexBuffer();
}