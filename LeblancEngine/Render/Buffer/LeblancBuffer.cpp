#include "LeblancEngine/Render/Buffer/LeblancBuffer.h"
#include "LeblancEngine/BasicInclude/LeblancMemoryOperation.h"
namespace Leblanc
{
	// Declaration data
	VertexLayoutDeclaration::VertexLayoutDeclaration(const std::vector<VertexElement>& vertex_elements)
	{
		for (unsigned int i = 0; i < vertex_elements.size(); i++)
		{
			m_elements.push_back(VertexElement(vertex_elements[i]));
		}
	}

	VertexLayoutDeclaration::VertexLayoutDeclaration(const VertexLayoutDeclaration& in)
	{
		for (unsigned int i = 0; i < in.m_elements.size(); i++)
		{
			m_elements.push_back(VertexElement(in.m_elements[i]));
		}
	};

	VertexLayoutDeclaration::~VertexLayoutDeclaration()
	{
		m_elements.clear();
	}

	const std::vector<VertexElement>& VertexLayoutDeclaration::elements() const
	{
		return m_elements;
	}

	// Declaration
	// Vertex Element
	VertexElement::VertexElement()
	{

	}

	VertexElement::VertexElement(WORD offsetVal,
		BYTE typeVal,
		BYTE usageVal,
		BYTE usageIndexVal,
		BYTE streamIndexVal)
	{
		m_offset = offsetVal;
		m_type = typeVal;
		m_usage = usageVal;
		m_usageIndex = usageIndexVal;
		m_streamIndex = streamIndexVal;
	}

	VertexElement::VertexElement(const VertexElement& element)
	{
		m_offset = element.m_offset;
		m_type = element.m_type;
		m_usageIndex = element.m_usageIndex;
		m_usage = element.m_usage;
		m_streamIndex = element.m_streamIndex;
	}

	WORD VertexElement::offset() const
	{
		return m_offset;
	}

	BYTE VertexElement::type() const
	{
		return m_type;
	}

	BYTE VertexElement::usage() const
	{
		return m_usage;
	}

	BYTE VertexElement::usageIndex() const
	{
		return m_usageIndex;
	}

	BYTE VertexElement::streamIndex() const
	{
		return m_streamIndex;
	}

	bool VertexElement::operator == (const VertexElement &src) const
	{
		if (m_usageIndex != src.usageIndex())
			return false;
		if (m_usage != src.usage())
			return false;
		if (m_type != src.type())
			return false;
		if (m_offset != src.offset())
			return false;

		return true;
	}

	bool VertexElement::operator != (const VertexElement &src) const
	{
		if (m_usageIndex != src.usageIndex())
			return true;
		if (m_usage != src.usage())
			return true;
		if (m_type != src.type())
			return true;
		if (m_offset != src.offset())
			return true;

		return false;
	}

	// VertexStream
	VertexStream::VertexStream(DeclarationUsage usage,
		unsigned int usage_index,
		unsigned int stride_arg,
		unsigned int count_arg,
		const float* stream_src) :
		m_usage(usage),
		m_usage_index(usage_index),
		m_stream(nullptr),
		m_count(count_arg),
		m_stride(stride_arg)
	{
		setStream(m_stride, m_count, stream_src);
	}

	VertexStream::~VertexStream()
	{
		m_stride = 0;
		m_count = 0;

		safe_delete_array(m_stream);
	}

	unsigned int VertexStream::id(const VertexElement& element)
	{
		unsigned int id = (1 << (element.usage() + 16));
		if (element.usageIndex() >= 0)
		{
			id |= (1 << (element.usage() + 1));
		}

		return id;
	}

	unsigned int VertexStream::id(const VertexStream& vertex_stream)
	{
		unsigned int id = (1 << ((BYTE)vertex_stream.usage() + 16));

		if (vertex_stream.usage_index() >= 0)
		{
			id |= (1 << ((vertex_stream.usage_index() + 1)));
		}

		return id;
	}

	void VertexStream::setStream(unsigned int stride_arg,
		unsigned int count_arg,
		const float* stream_src)
	{
		m_stride = stride_arg;
		m_count = count_arg;

		if (size() > 0)
		{
			safe_delete_array(m_stream);
			m_stream = new float[size()];
			setStream(stream_src);
		}
	}

	void VertexStream::setStream(const float* stream_src)
	{
		if (m_stream)
		{
			memcpy(m_stream, stream_src, bufferSizeInBytes());
		}
	}

	unsigned int IVertexDeclaration::elementToSize(const BYTE& type)
	{
		switch (type)
		{
		case (UINT)DeclarationType::FLOAT1:
			return 1 * sizeof(float);
		case (UINT)DeclarationType::FLOAT2:
			return 2 * sizeof(float);
		case (UINT)DeclarationType::FLOAT3:
			return 3 * sizeof(float);
		case (UINT)DeclarationType::FLOAT4:
			return 4 * sizeof(float);
		case (UINT)DeclarationType::UBYTE4:
			return 4 * sizeof(uint8_t);
		}
		return 0;
	}
}