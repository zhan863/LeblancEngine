#include "LeblancEngine/Render/Resource/LeblancResourceManager.h"
#include "LeblancEngine/Render/Resource/TextureOrgainizer/LeblancGBuffer.h"

ResourceManager::ResourceManager()
{

}

ResourceManager::~ResourceManager()
{

}

void ResourceManager::initialize(Window& window)
{
	INT width, height;
	window.GetClientArea(width, height);

	if (width > 0 && height > 0)
		createGBuffer((UINT)width, (UINT)height);
}

void ResourceManager::release()
{
	if (gbuffer)
	{
		delete gbuffer;
		gbuffer = nullptr;
	}
}

void ResourceManager::createGBuffer(UINT width, UINT height)
{
	if (gbuffer)
	{
		delete gbuffer;
	}

	gbuffer = new GBuffer();
	gbuffer->initialize(width, height);
}

GBuffer* ResourceManager::getGBuffer()
{
	return gbuffer;
}