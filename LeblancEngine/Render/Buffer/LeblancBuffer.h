#ifndef LEBLANC_BUFFER_H
#define LEBLANC_BUFFER_H

#include <vector>
#include "LeblancEngine/Render/Include/LeblancDirectInclude.h"
#include "LeblancEngine/Render/Defines/LeblancRenderBasicDefine.h"

namespace Leblanc
{
	// Data
	// Declaration data
	class VertexElement
	{
	private:
		VertexElement();

	public:
		VertexElement(WORD offset, BYTE type, BYTE usage, BYTE usageIndex, BYTE streamIndex = 0);
		VertexElement(const VertexElement& element);

		WORD                        offset() const;
		BYTE                        type() const;
		BYTE                        usage() const;
		BYTE                        usageIndex() const;
		BYTE                        streamIndex() const;

		bool                        operator == (const VertexElement &src) const;
		bool                        operator != (const VertexElement &src) const;

	private:
		WORD                        m_offset;
		BYTE                        m_type;
		BYTE                        m_usage;
		BYTE                        m_usageIndex;
		BYTE                        m_streamIndex;
	};

	class VertexStream
	{
	public:
		VertexStream(DeclarationUsage usage,
			unsigned int usageIndex,
			unsigned int stride,
			unsigned int count,
			const float* stream);

		virtual ~VertexStream();

		const float*					stream() const { return m_stream; }
		float*							stream() { return m_stream; }

		const unsigned int					stride() const { return m_stride; }
		const unsigned int					count() const { return m_count; }
		const unsigned int					size() const { return m_stride * m_count; }
		const unsigned int					bufferSizeInBytes() const { return  size() * sizeof(float); }
		const DeclarationUsage			usage() const { return m_usage; }
		const unsigned int					usage_index() const { return m_usage_index; }

		void						    setStream(unsigned int stride, unsigned int count, const float*);

		static unsigned int					id(const VertexStream&);
		static unsigned int					id(const VertexElement& element);

	protected:
		void							setStream(const float*);

	private:
		unsigned int						m_stride;
		unsigned int						m_count;
		float*							m_stream;
		DeclarationUsage				m_usage;
		unsigned int						m_usage_index;
	};

	// Resource Declaration
	class ResourceDeclaration
	{
	public:
		ResourceDeclaration() {}
		virtual ~ResourceDeclaration() {}
	};

	class IndexBufferDeclaration : public ResourceDeclaration
	{
	public:
		IndexBufferDeclaration(size_t index_count) : m_index_count(index_count) {}
		virtual ~IndexBufferDeclaration() {}

		size_t getIndexCount() const { return m_index_count; }
	protected:
		size_t m_index_count = 0;
	};

	class VertexBufferDeclaration : public ResourceDeclaration
	{
	public:
		VertexBufferDeclaration(unsigned int sizeInBytesVal,
			unsigned int vertexStride = 0,
			void* vertexPtr = 0) : m_size_in_bytes(sizeInBytesVal), m_vertex_stride(vertexStride), m_vertex_ptr(vertexPtr)
		{}

		virtual	~VertexBufferDeclaration() {}

		unsigned int                    vertexStride() const { return m_vertex_stride; }
		void*                       vertexPtr() const { return m_vertex_ptr; }

		unsigned int                sizeInBytes() const { return m_size_in_bytes; }
	private:
		// data
		unsigned int                m_size_in_bytes;
		unsigned int                    m_vertex_stride;

		// reference
		void*                       m_vertex_ptr = nullptr;
	};

	class VertexElement;
	class VertexLayoutDeclaration : ResourceDeclaration
	{
	public:
		VertexLayoutDeclaration(const std::vector<VertexElement>& vertexElements);
		VertexLayoutDeclaration(const VertexLayoutDeclaration& in);
		virtual ~VertexLayoutDeclaration();

		const std::vector<VertexElement>& elements() const;

	private:
		std::vector<VertexElement> m_elements;
	};

	// Resource
	class IDevice;
	class IDeviceContext;
	class IVertexDeclaration;
	class IVertexBuffer
	{
	public:
		IVertexBuffer(IDevice* device, IDeviceContext* device_context) : m_device(device), m_device_context(device_context) {}
		virtual ~IVertexBuffer() {}

		virtual void release() = 0;

		virtual void bind() const = 0;

		virtual void setVertexDeclaration(const IVertexDeclaration* decl) = 0;

	protected:
		// reference
		IDevice * m_device = nullptr;
		IDeviceContext* m_device_context = nullptr;
	};

	class IIndexBuffer
	{
	public:
		IIndexBuffer(IDevice* device, IDeviceContext* device_context) : m_device(device), m_device_context(device_context) {}
		virtual ~IIndexBuffer() {}

		virtual void release() = 0;

		virtual void initialize(const IndexBufferDeclaration* declaration) = 0;

		virtual void* lock() = 0;

		virtual void unlock() = 0;

		virtual void bind() const = 0;
	protected:
		// reference
		IDevice * m_device = nullptr;
		IDeviceContext* m_device_context = nullptr;
	};

	class IVertexDeclaration
	{
	public:
		IVertexDeclaration(IDevice* device, IDeviceContext* device_context) : m_device(device), m_device_context(device_context) {}
		virtual ~IVertexDeclaration() {}

		virtual void release() = 0;

		virtual unsigned int vertexStride() const = 0;

		virtual const std::vector<VertexElement>& elements() const = 0;

		static unsigned int elementToSize(const BYTE& type);
	protected:
		// reference
		IDevice * m_device = nullptr;
		IDeviceContext* m_device_context = nullptr;
	};
}
#endif