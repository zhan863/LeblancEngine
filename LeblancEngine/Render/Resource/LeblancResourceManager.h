#ifndef LEBLANC_RESOURCE_MANAGER_H
#define LEBLANC_RESOURCE_MANAGER_H

#include "LeblancEngine/BasicInclude/LeblancPCH.h"
#include "LeblancEngine/Render/Basics/LeblancWindow.h"

class GBuffer;

class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

	void initialize(Window& window);

	void release();

	GBuffer* getGBuffer();

	void createGBuffer(UINT width, UINT height);

protected:

	GBuffer* gbuffer = nullptr;
};
#endif