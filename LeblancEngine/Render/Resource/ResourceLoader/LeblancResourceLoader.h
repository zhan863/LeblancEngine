#ifndef LEBLANC_RESOURCE_LOADER_H
#define LEBLANC_RESOURCE_LOADER_H

#include "LeblancEngine/BasicInclude/LeblancPCH.h"

enum class MeshFileType
{
	obj,
	x,
	tbx
};

class ResourceLoader
{
public:
	static ID3DX10Mesh* loadMeshFromFile(char* file_name, MeshFileType type);
};

#endif