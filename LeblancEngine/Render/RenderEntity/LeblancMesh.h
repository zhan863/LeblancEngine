#ifndef LEBLANC_MESH_H
#define LEBLANC_MESH_H

#include "LeblancEngine/BasicInclude/LeblancPCH.h"
#include "LeblancEngine/Render/Defines/LeblancRenderBasicDefine.h"

namespace Leblanc
{
	class Material;
	class IVertexBuffer;
	class IVertexDeclaration;

	class Mesh
	{
	public:
		Mesh();
		virtual ~Mesh();

		Material* getMaterial() { return m_material; }

		const IVertexBuffer* getVertexBuffer() { return m_vertex_buffer; }

		const IVertexDeclaration* getVertexDeclaration() { return m_vertex_declaration; }

		PrimitiveType getPrimitiveType() { return m_primitive_type; }

	protected:
		void release();

		void createVertexBuffer();

		virtual void* internalStreamPtr() = 0;

		void setVertexDeclaration(IVertexDeclaration* declaration);

		void setVertexCount(size_t vertex_count);

		size_t getVertexCount();

		void setPrimitiveType(PrimitiveType primitive_type);

		void setVertexCount(unsigned int vertex_count) { m_vertex_count = vertex_count; }

		unsigned int vertexCount() { return m_vertex_count; }

		unsigned int vertexBufferSize();

		// data
		size_t m_vertex_count = 0;

		PrimitiveType m_primitive_type = PrimitiveType::TriangleList;

		IVertexBuffer* m_vertex_buffer = nullptr;

		// reference
		IVertexDeclaration* m_vertex_declaration = nullptr;

		Material* m_material = nullptr;
	};
}
#endif