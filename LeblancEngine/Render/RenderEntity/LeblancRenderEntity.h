#ifndef LEBLANC_RENDER_ENTITY_H
#define LEBLANC_RENDER_ENTITY_H

enum class MeshFileType
{
	OBJ,
	X
};

enum class Pass
{
	GBuffer,
	DeferredShading,
	SingleForward,
	Transparent,
	DepthOnly
};

class RenderEntity
{
public:
	RenderEntity();
	~RenderEntity();

	virtual void createFromFile(char* file_name, MeshFileType file_type);

	void release();

	virtual void render(Pass pass);

protected:
	
};
#endif