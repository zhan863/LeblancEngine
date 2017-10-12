#ifndef LEBLANC_RESOURCE_MANAGER_H
#define LEBLANC_RESOURCE_MANAGER_H
class GBuffer;

class ResourceManager
{
	ResourceManager();
	~ResourceManager();

	void initialize();

	void clear();

	GBuffer* getGBuffer();

protected:
	GBuffer* gbuffer = nullptr;
};
#endif