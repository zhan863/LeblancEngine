#ifndef LEBLANC_STREAM_MESH_H
#define LEBLANC_STREAM_MESH_H

#include "LeblancEngine/Render/RenderEntity/LeblancMesh.h"
#include "LeblancEngine/Render/Buffer/LeblancBuffer.h"

namespace Leblanc
{
	class StreamMesh : public Mesh
	{
	public:
		StreamMesh();
		virtual ~StreamMesh();

	protected:
		void release();
		void addStream(VertexStream* vertex_stream);

		virtual void* internalStreamPtr();

		bool findStream(VertexStream*&, const VertexElement&);

		// reference
		typedef std::map<unsigned int, VertexStream*> VertexStreamMap;
		VertexStreamMap            m_vertex_streams;

		// data
		void*			   m_vertex_buffer_cpu_memory = nullptr;
		VertexStream*      m_normals_stream = nullptr;
		VertexStream*      m_position_stream = nullptr;
		VertexStream*      m_texcoord_stream = nullptr;
	};
}
#endif