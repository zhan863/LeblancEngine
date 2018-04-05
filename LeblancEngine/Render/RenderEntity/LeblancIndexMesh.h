#ifndef LEBLANC_INDEX_MESH_H
#define LEBLANC_INDEX_MESH_H

#include "LeblancEngine/BasicInclude/LeblancPCH.h"
#include "LeblancEngine/Render/RenderEntity/LeblancStreamMesh.h"
#include "LeblancEngine/Render/Basics/LeblancRenderPlatformDefineD3D11.h"

class IndexMesh : public StreamMesh
{
public:
	IndexMesh();
	~IndexMesh();

	void load(const aiMesh* mesh, const aiScene* scene);

	bool loadFromFile(const char* file_name);

protected:
	void setIndices(const uint32_t* indices, size_t index_count, size_t face_count);

	void createIndexBuffer(size_t index_count);

	void fillIndexBuffer();

	void release();

	//data
	uint32_t* m_index_buffer_data = nullptr;
	size_t m_index_count = 0;
	IndexBufferD3D11* m_index_buffer = nullptr;
};
#endif