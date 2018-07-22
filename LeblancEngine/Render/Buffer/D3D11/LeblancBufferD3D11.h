#ifndef LEBLANC_BUFFER_D3D11_H
#define LEBLANC_BUFFER_D3D11_H

#include "LeblancEngine/Render/Buffer/LeblancBuffer.h"

struct ID3D11Buffer;
namespace Leblanc
{
	class VertexDeclarationD3D11;

	class IndexBufferD3D11 : public IIndexBuffer
	{
	public:
		IndexBufferD3D11(IDevice* device, IDeviceContext* device_context) : IIndexBuffer(device, device_context) {}
		virtual ~IndexBufferD3D11() { release(); }

		virtual void release();

		virtual void initialize(const IndexBufferDeclaration* declaration);

		virtual void* lock();

		virtual void unlock();

		virtual void bind() const;

	protected:

		// data
		ID3D11Buffer * m_index_buffer = nullptr;
	};

	class VertexBufferD3D11 : public IVertexBuffer
	{
	public:
		VertexBufferD3D11(IDevice* device, IDeviceContext* device_context) : IVertexBuffer(device, device_context) {}

		virtual ~VertexBufferD3D11() { release(); }

		virtual void release();

		bool initialize(const VertexBufferDeclaration* data);

		virtual void setVertexDeclaration(const IVertexDeclaration* decl);

		const IVertexDeclaration* vertexDeclaration();

		virtual void bind() const;

	protected:
		// reference
		const VertexDeclarationD3D11*  m_vertex_declaration = nullptr;

		// data
		ID3D11Buffer* m_vertex_buffer = nullptr;

		unsigned int m_size_in_bytes = 0;
		unsigned int m_stride = 0;
	};

	class VertexDeclarationD3D11 : public IVertexDeclaration
	{
	public:
		VertexDeclarationD3D11(IDevice* device, IDeviceContext* device_context) : IVertexDeclaration(device, device_context) {}
		virtual ~VertexDeclarationD3D11() { release(); }

		virtual void release();

		void initialize(const VertexLayoutDeclaration* declaration);

		const std::vector<VertexElement>& elements() const { return m_input_layout_declaration; }

		virtual unsigned int vertexStride() const;

		unsigned int getElementCount() const { return m_element_count; }

		const D3D11_INPUT_ELEMENT_DESC* getInputLayout() const { return m_input_layout; }

	protected:
		// data
		D3D11_INPUT_ELEMENT_DESC * m_input_layout = nullptr;
		std::vector<VertexElement> m_input_layout_declaration;
		unsigned int m_element_count;
		unsigned int m_vertex_stride;
	};
}
#endif