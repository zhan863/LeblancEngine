#ifndef LEBLANC_RESOURCE_MANAGER_H
#define LEBLANC_RESOURCE_MANAGER_H

#include "LeblancEngine/BasicInclude/LeblancPCH.h"

class GBuffer;

class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

	void initialize();

	void release();

	GBuffer* getGBuffer();

	void createGBuffer(UINT width, UINT height);

protected:

	GBuffer* gbuffer = nullptr;
};
#endif