#ifndef LEBLANC_RESOURCE_MANAGER_H
#define LEBLANC_RESOURCE_MANAGER_H
class GBuffer;

class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

	void initialize();

	void release();

	GBuffer* getGBuffer();

protected:
	GBuffer* gbuffer = nullptr;
};
#endif