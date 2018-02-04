#include "LeblancEngine/Render/Resource/LeblancResourceManager.h"
#include "LeblancEngine/Render/Resource/TextureOrgainizer/LeblancGBuffer.h"

ResourceManager::ResourceManager()
{

}

ResourceManager::~ResourceManager()
{

}

void ResourceManager::initialize()
{
}

void ResourceManager::release()
{
	if (gbuffer)
	{
		gbuffer->release();
		gbuffer = nullptr;
	}
}

void ResourceManager::createGBuffer(UINT width, UINT height)
{
	gbuffer->initialize(width, height);
}

GBuffer* ResourceManager::getGBuffer()
{
	return gbuffer;
}