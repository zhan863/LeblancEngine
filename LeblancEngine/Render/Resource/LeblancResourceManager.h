#ifndef LEBLANC_RESOURCE_MANAGER_H
#define LEBLANC_RESOURCE_MANAGER_H

#include "LeblancEngine/BasicInclude/LeblancPCH.h"
#include "LeblancEngine/Render/Basics/LeblancWindow.h"

class IndexMesh;
class GBuffer;

class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

	void initialize(Window& window);

	void release();

	GBuffer* getGBuffer();

	IndexMesh* getScreenQuad();

	void createGBuffer(UINT width, UINT height);

protected:

	// Data
	GBuffer* m_gbuffer = nullptr;

	IndexMesh* m_screen_quad = nullptr;
};
#endif