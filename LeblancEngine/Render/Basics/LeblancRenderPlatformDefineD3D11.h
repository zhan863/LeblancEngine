#ifndef LEBLANC_RENDER_PLATFORM_DEFINE_D3D11_H
#define LEBLANC_RENDER_PLATFORM_DEFINE_D3D11_H

#include "LeblancEngine/BasicInclude/LeblancPCH.h"
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
	IndexBufferDeclaration(size_t index_count) {}
	virtual ~IndexBufferDeclaration() {}

	size_t getIndexCount() const { return m_index_count; }
protected:
	size_t m_index_count = 0;
};

class DeviceD3D11;
// Resource
class RenderResource
{
public:
	RenderResource(DeviceD3D11* device) : m_device(device) {}
	~RenderResource() {}

	virtual void release() = 0;
protected:
	// reference
	DeviceD3D11* m_device = nullptr;

};

class IndexBufferD3D11 : public RenderResource
{
public:
	IndexBufferD3D11(DeviceD3D11* device) : RenderResource(device) {}
	~IndexBufferD3D11() {}

	void release();

	void initialize(const IndexBufferDeclaration* delaration);

	void* lock();

	void unlock();

protected:

	// data
	ID3D11Buffer* m_index_buffer = nullptr;
};
#endif