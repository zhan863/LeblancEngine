#include "LeblancEngine/Render/RenderEntity/LeblancIndexMesh.h"
#include "LeblancEngine/Math/LeblancMath.h"
#include "LeblancEngine/Render/Basics/LeblancRenderPlatformDefineD3D11.h"
#include "LeblancEngine/BasicInclude/LeblancMemoryOperation.h"
#include "LeblancEngine/Global/LeblancGlobalContext.h"

#include "LeblancEngine/Render/Resource/Material/LeblancMaterial.h"
#include "LeblancEngine/Render/Utility/LeblancDeviceD3D11.h"

#include <assimp/Include/Importer.hpp>
#include <assimp/Include/scene.h>
#include <assimp/Include/postprocess.h>

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

bool IndexMesh::loadFromFile(const char* file_name)
{
	// To do: need to load the .scene file instead of the fbx file directly

	Assimp::Importer importer;
	uint32_t flags = aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_PreTransformVertices |
		aiProcess_FlipUVs;

	// the ai scene here equals the entity and the sub mesh of it equals the mesh contained by the entity;
	const aiScene* scene = importer.ReadFile(file_name, flags);

	if (scene == nullptr)
	{
		//LOG("Failed to load scene " << meshFilePathName);
		return false;
	}

	if (scene->mNumMeshes == 0)
	{
		//LOG("Failed to load any meshes " << meshFilePathName);
		return false;
	}

	if (scene->HasMeshes() && scene->mNumMeshes == 1)
	{
		aiMesh* mesh = scene->mMeshes[0];
		if (mesh)
		{
			load(mesh, scene);
			return true;
		}
	}

	return false;
}

void IndexMesh::load(const aiMesh* mesh, const aiScene* scene)
{
	if (mesh)
	{
		release();

		// load mesh
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
			memcpy(normal_ptr, mesh->mNormals, sizeof(float) * 3 * vertex_count);
		}

		if (mesh->HasTextureCoords(0))
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

		// currently in Leblanc Engine, the vertex delcaration is fixed as Position, Normal and Texture coordinate.
		vector<VertexElement> vertex_elements;
		vertex_elements.push_back(VertexElement(0, (BYTE)DeclarationType::FLOAT3, (BYTE)DeclarationUsage::POSITION, 0, 0));
		vertex_elements.push_back(VertexElement(12, (BYTE)DeclarationType::FLOAT3, (BYTE)DeclarationUsage::NORMAL, 0, 0));
		vertex_elements.push_back(VertexElement(24, (BYTE)DeclarationType::FLOAT2, (BYTE)DeclarationUsage::TEXCOORD, 0, 0));

		VertexLayoutDeclaration vertex_layout_declaration(vertex_elements);

		VertexDeclarationD3D11* vertex_declaration = g_global_context.m_render_state_manager.getOrCreateVertexDeclaration(&vertex_layout_declaration);

		setVertexDeclaration(vertex_declaration);

		VertexStream* vertex_stream = new VertexStream(DeclarationUsage::POSITION, 0, 3, vertexCount(), (float*)position_ptr);
		VertexStream* normal_stream = new VertexStream(DeclarationUsage::NORMAL, 0, 3, vertexCount(), (float*)normal_ptr);
		VertexStream* texcoord_stream = new VertexStream(DeclarationUsage::TEXCOORD, 0, 2, vertexCount(), (float*)uv_ptr);

		addStream(vertex_stream);
		addStream(normal_stream); 
		addStream(texcoord_stream);

		createVertexBuffer();

		// release all data created during creation
		safe_delete_array(index_ptr);
		safe_delete_array(position_ptr);
		safe_delete_array(normal_ptr);
		safe_delete_array(uv_ptr);

		// load mesh material
		if (scene->HasMaterials())
		{
			const aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			m_material = new Material;

			if (m_material)
			{
				aiString material_name;
				material->Get("?mat.name", 0, 0, material_name);

				m_material->initialize("Content\\Material\\tone_mapping.material");////material_name.C_Str());
			}
		}
	}
}

void IndexMesh::createIndexBuffer(size_t index_count)
{
	IndexBufferDeclaration declaration(index_count);
	if (m_index_buffer)
	{
		safe_delete(m_index_buffer);
	}

	DeviceD3D11* device = g_global_context.m_device_manager.getDevice();
	m_index_buffer = device->createIndexBuffer(&declaration);

	fillIndexBuffer();
}

void IndexMesh::fillIndexBuffer()
{
	if (m_index_buffer)
	{
		uint32_t* data_segment = static_cast<uint32_t*>(m_index_buffer->lock());
		if (data_segment)
		{
			memcpy(data_segment, m_index_buffer_data, sizeof(uint32_t) * m_index_count);
			m_index_buffer->unlock();
		}
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