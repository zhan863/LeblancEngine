#ifndef LEBLANC_RENDER_PLATFORM_DEFINE_D3D11_H
#define LEBLANC_RENDER_PLATFORM_DEFINE_D3D11_H

#include "LeblancEngine/BasicInclude/LeblancPCH.h"
#include "LeblancEngine/Render/Basics/LeblancRenderBasicDefine.h"
#include "LeblancEngine/Render/Basics/LeblancRenderPlatformIndependentDefine.h"
// Resource delaration
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
	VertexBufferDeclaration(uint32_t sizeInBytesVal,
							uint32_t vertexStride = 0,
							void* vertexPtr = 0) : m_size_in_bytes(sizeInBytesVal), m_vertex_stride(vertexStride), m_vertex_ptr(vertexPtr)
	{}
	  
	virtual	~VertexBufferDeclaration() {}

	uint32_t                    vertexStride() const { return m_vertex_stride; }
	void*                       vertexPtr() const { return m_vertex_ptr; }
	
	unsigned int                sizeInBytes() const { return m_size_in_bytes; }
private:
	// data
	unsigned int                m_size_in_bytes;
	uint32_t                    m_vertex_stride;

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

class DeviceD3D11;
// Resource
class RenderResource
{
public:
	RenderResource(DeviceD3D11* device) : m_device(device) {}
	virtual ~RenderResource() {}

	virtual void release() = 0;
protected:
	// reference
	DeviceD3D11* m_device = nullptr;

};

class IndexBufferD3D11 : public RenderResource
{
public:
	IndexBufferD3D11(DeviceD3D11* device) : RenderResource(device) {}
	virtual ~IndexBufferD3D11() { release(); }

	virtual void release();

	void initialize(const IndexBufferDeclaration* declaration);

	void* lock();

	void unlock();

	void bind() const;

protected:

	// data
	ID3D11Buffer* m_index_buffer = nullptr;
};

class VertexDeclarationD3D11;
class VertexBufferD3D11 : public RenderResource
{
public:
	VertexBufferD3D11(DeviceD3D11* device) : RenderResource(device) {}

	virtual ~VertexBufferD3D11() { release(); }

	virtual void release();

	bool initialize(const VertexBufferDeclaration* data);

	virtual void setVertexDeclaration(const VertexDeclarationD3D11* decl) { m_vertex_declaration = decl; }

	const VertexDeclarationD3D11* vertexDeclaration() const { return m_vertex_declaration; }

	void bind() const;

protected:
	// reference
	const VertexDeclarationD3D11*  m_vertex_declaration = nullptr;

	// data
	ID3D11Buffer* m_vertex_buffer = nullptr;

	uint32_t m_size_in_bytes = 0;
	uint32_t m_stride = 0;
};

class VertexDeclarationD3D11 : public RenderResource
{
public:
	VertexDeclarationD3D11(DeviceD3D11* device) : RenderResource(device) {}
	virtual ~VertexDeclarationD3D11() { release(); }

	virtual void release();

	void initialize(const VertexLayoutDeclaration* declaration);

	const std::vector<VertexElement>& elements() const { return m_input_layout_declaration; }

	static uint32_t elementToSize(const BYTE& type);

	uint32_t vertexStride() const;

	uint32_t getElementCount() const { return m_element_count; }

	const D3D11_INPUT_ELEMENT_DESC* getInputLayout() const { return m_input_layout; }

protected:
	// data
	D3D11_INPUT_ELEMENT_DESC * m_input_layout = nullptr;
	std::vector<VertexElement> m_input_layout_declaration;
	uint32_t m_element_count;
	uint32_t m_vertex_stride;
};

class RasterizerStateD3D11 : public RenderResource
{
public:
	RasterizerStateD3D11(DeviceD3D11* device) : RenderResource(device) {}
	virtual ~RasterizerStateD3D11() { release(); }

	virtual void release();

	void initialize(RasterizerState rasterizer_state);

	ID3D11RasterizerState* getRasterizerState() { return m_rasterizer_state; }
protected:
	// data
	ID3D11RasterizerState * m_rasterizer_state = nullptr;
};

class DepthStencilStateD3D11 : public RenderResource
{
public:
	DepthStencilStateD3D11(DeviceD3D11* device) : RenderResource(device) {}
	virtual ~DepthStencilStateD3D11() { release(); }

	virtual void release();

	void initialize(DepthStencilState depth_stencil_state);

	ID3D11DepthStencilState* getDepthStencilState() { return m_depth_stencil_state; }
protected:
	// data
	ID3D11DepthStencilState * m_depth_stencil_state = nullptr;
};

class BlendStateD3D11 : public RenderResource
{
public:
	BlendStateD3D11(DeviceD3D11* device) : RenderResource(device) {}
	virtual ~BlendStateD3D11() { release(); }

	virtual void release();

	void initialize(BlendState blend_mode);

	ID3D11BlendState* getBlendState() { return m_blend_state; }
protected:
	// data
	ID3D11BlendState * m_blend_state = nullptr;
};

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
		uint32_t usageIndex,
		uint32_t stride,
		uint32_t count,
		const float* stream);

	virtual ~VertexStream();

	const float*					stream() const { return m_stream; }
	float*							stream() { return m_stream; }

	const uint32_t					stride() const { return m_stride; }
	const uint32_t					count() const { return m_count; }
	const uint32_t					size() const { return m_stride * m_count; }
	const uint32_t					bufferSizeInBytes() const { return  size() * sizeof(float); }
	const DeclarationUsage			usage() const { return m_usage; }
	const uint32_t					usage_index() const { return m_usage_index; }

	void						    setStream(uint32_t stride, uint32_t count, const float*);

	static uint32_t					id(const VertexStream&);
	static uint32_t					id(const VertexElement& element);

protected:
	void							setStream(const float*);

private:
	uint32_t						m_stride;
	uint32_t						m_count;
	float*							m_stream;
	DeclarationUsage				m_usage;
	uint32_t						m_usage_index;
};

class InputLayoutCacheD3D11
{
public:
	InputLayoutCacheD3D11(DeviceD3D11* device);
	virtual ~InputLayoutCacheD3D11();

	void							release();
	void							initialize(ID3DX11EffectTechnique* technique, uint32_t passIndex);
	bool							bindLayout(const VertexDeclarationD3D11* vertex_declaration) const;

protected:
	typedef std::map <const VertexDeclarationD3D11*, ID3D11InputLayout*> LayoutMap;
	// data
	mutable LayoutMap               m_layouts;
	uint32_t                        m_pass_index = -1;
	D3DX11_PASS_DESC                m_pass_desc;

	// reference
	ID3DX11EffectTechnique*         m_technique = nullptr;
	DeviceD3D11*					m_device = nullptr;
};
#endif