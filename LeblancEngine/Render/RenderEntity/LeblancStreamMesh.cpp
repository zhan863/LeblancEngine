#include "LeblancEngine/Render/RenderEntity/LeblancStreamMesh.h"
#include "LeblancEngine/BasicInclude/LeblancMemoryOperation.h"

StreamMesh::StreamMesh()
{

}

StreamMesh::~StreamMesh()
{
	release();
}

void StreamMesh::release()
{
	// data
	safe_delete(m_position_stream);
	safe_delete(m_texcoord_stream);
	safe_delete(m_normals_stream);
	safe_delete_array(m_vertex_buffer_cpu_memory); 

	// reference
	m_vertex_streams.clear();
}


bool StreamMesh::findStream(VertexStream*& stream, const VertexElement& element)
{
	stream = nullptr;
	auto it = m_vertex_streams.find(VertexStream::id(element));
	if (it != m_vertex_streams.end())
	{
		stream = it->second;
		return true;
	}
	return false;
}

void StreamMesh::addStream(VertexStream* stream)
{
	m_vertex_streams.insert(std::make_pair(VertexStream::id(*stream), stream));

	if (stream->usage() == DeclarationUsage::POSITION)
	{
		m_position_stream = stream;
	}
	if (stream->usage() == DeclarationUsage::NORMAL)
	{
		m_normals_stream = stream;
	}
	if (stream->usage() == DeclarationUsage::TEXCOORD)
	{
		m_texcoord_stream = stream;
	}
}

void* StreamMesh::internalStreamPtr()
{
	safe_delete_array(m_vertex_buffer_cpu_memory);

	m_vertex_buffer_cpu_memory = new float[vertexBufferSize()];

	// stash the buffer and put it in an array
	std::map <uint32_t, float*> stream_map;

	for (auto it = m_vertex_streams.begin(); it != m_vertex_streams.end(); it++)
	{
		stream_map.insert(std::make_pair(it->first, it->second->stream()));
	}

	const std::vector <VertexElement>& declaration = m_vertex_declaration->elements();

	float* vb = (float*)m_vertex_buffer_cpu_memory;

	for (uint32_t i = 0; i < vertexCount(); i++)
	{
		for (uint32_t j = 0; j < declaration.size() - 1; j++)
		{
			VertexElement element = declaration[j];
			VertexStream* stream = 0;
			if (findStream(stream, element))
			{
				uint32_t stream_id = VertexStream::id(*stream);
				auto stream_it = stream_map.find(stream_id);

				if (stream_it != stream_map.end() &&
					stream_it->second)
				{
					for (uint32_t k = 0; k < stream->stride(); k++)
					{
						*vb++ = stream_it->second[k];
					}
					stream_it->second += stream->stride();
				}
				else
				{
					uint32_t stride = VertexDeclarationD3D11::elementToSize(element.type());
					for (uint32_t k = 0; k < stride; stride++)
					{
						*vb++ = 0;
					}
				}
			}
			else
			{
				// can't find a stream for this element.
				// warn user, and 0 the buffer for the stride size
				uint32_t stride = VertexDeclarationD3D11::elementToSize(element.type());
				for (uint32_t k = 0; k < stride; k++)
				{
					*vb++ = 0;
				}
			}
		}
	}

	return m_vertex_buffer_cpu_memory;
}