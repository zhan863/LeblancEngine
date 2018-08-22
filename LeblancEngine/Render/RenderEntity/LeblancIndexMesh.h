#ifndef LEBLANC_INDEX_MESH_H
#define LEBLANC_INDEX_MESH_H

#include "LeblancEngine/BasicInclude/LeblancPCH.h"
#include "LeblancEngine/Render/RenderEntity/LeblancStreamMesh.h"

#include <assimp/Include/Importer.hpp>
#include <assimp/Include/scene.h>
#include <assimp/Include/postprocess.h>

namespace Leblanc
{
	class IIndexBuffer;

	class IndexMesh : public StreamMesh
	{
	public:
		IndexMesh();
		~IndexMesh();

		void load(const aiMesh* mesh, const aiScene* scene);

		bool loadFromFile(const char* file_name);

		IIndexBuffer* getIndexBuffer() const { return m_index_buffer; }

		size_t getIndexCount() const { return m_index_count; }

	protected:
		void setIndices(const unsigned int* indices, size_t index_count, size_t face_count);

		void createIndexBuffer(size_t index_count);

		void fillIndexBuffer();

		void release();

		//data
		unsigned int* m_index_buffer_data = nullptr;
		size_t m_index_count = 0;
		IIndexBuffer* m_index_buffer = nullptr;
	};
}
#endif