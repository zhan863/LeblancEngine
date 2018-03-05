#ifndef LEBLANC_RESOURCE_LOADER_H
#define LEBLANC_RESOURCE_LOADER_H

#include "LeblancEngine/BasicInclude/LeblancPCH.h"
#include "LeblancEngine/Render/Basics/LeblancGeometry.h"
#include "LeblancEngine/Render/Scene/LeblancScene.h"
using namespace std;

enum class MeshFileType
{
	obj,
	x,
	tbx
};

struct VertexEntry
{
	UINT index;
	VertexEntry* next;
};

class ResourceLoader
{
public:
	static Mesh* loadMeshFromFile(const WCHAR* file_name, MeshFileType type);
	static void loadSceneFromFile(const char* file_name, Scene& scene);

protected:
	static UINT addVertex(UINT index, Vertex vertex, vector<UINT>& indices, vector<Vertex>& vertices, vector<VertexEntry*>& vertex_hashtable);
};

#endif