#include "LeblancEngine/Render/Resource/LeblancResourceManager.h"
#include "LeblancEngine/Render/Resource/TextureOrgainizer/LeblancGBuffer.h"
#include "LeblancEngine/BasicInclude/LeblancMemoryOperation.h"
#include "LeblancEngine/Render/RenderEntity/LeblancIndexMesh.h"

const char* k_quad_file_name = "Content\\Mesh\\Basic\\quad.fbx";

ResourceManager::ResourceManager()
{

}

ResourceManager::~ResourceManager()
{

}

void ResourceManager::initialize(Window& window)
{
	release();

	INT width, height;
	window.GetClientArea(width, height);

	if (width > 0 && height > 0)
		createGBuffer((UINT)width, (UINT)height);

	m_screen_quad = new IndexMesh();
	m_screen_quad->loadFromFile(k_quad_file_name);
}

void ResourceManager::release()
{
	safe_delete(m_gbuffer);
	safe_delete(m_screen_quad);
}

void ResourceManager::createGBuffer(UINT width, UINT height)
{
	safe_delete(m_gbuffer);

	m_gbuffer = new GBuffer();
	m_gbuffer->initialize(width, height);
}

GBuffer* ResourceManager::getGBuffer()
{
	return m_gbuffer;
}

IndexMesh* ResourceManager::getScreenQuad()
{
	return m_screen_quad;
}