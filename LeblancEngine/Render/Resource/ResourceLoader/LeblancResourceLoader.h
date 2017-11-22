#ifndef LEBLANC_RESOURCE_LOADER_H
#define LEBLANC_RESOURCE_LOADER_H

#include "LeblancEngine/BasicInclude/LeblancPCH.h"
using namespace std;

enum class MeshFileType
{
	obj,
	x,
	tbx
};

struct VERTEX
{
	D3DXVECTOR3 position;
	D3DXVECTOR3 normal;
	D3DXVECTOR2 texcoord;
};

class ResourceLoader
{
public:
	static ID3DX10Mesh* loadMeshFromFile(char* file_name, MeshFileType type);

protected:
	static void addVertex(UINT index, VERTEX vertex, vector<UINT>& indices, vector<VERTEX>& vertices);
};

#endif