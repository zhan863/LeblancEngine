#include "LeblancEngine/Render/Resource/ResourceLoader/LeblancResourceLoader.h"
#include "LeblancEngine/BasicInclude/LeblancPCH.h"

void ResourceLoader::addVertex(UINT index, VERTEX vertex, vector<UINT>& indices, vector<VERTEX>& vertices)
{
	//bool found = false;
	//UINT index = 0;

	//// Since it's very slow to check every element in the vertex list, a hashtable stores
	//// vertex indices according to the vertex position's index as reported by the OBJ file
	//if ((UINT)m_VertexCache.GetSize() > hash)
	//{
	//	CacheEntry* pEntry = m_VertexCache.GetAt(hash);
	//	while (pEntry != NULL)
	//	{
	//		VERTEX* pCacheVertex = m_Vertices.GetData() + pEntry->index;

	//		// If this vertex is identical to the vertex already in the list, simply
	//		// point the index buffer to the existing vertex
	//		if (0 == memcmp(pVertex, pCacheVertex, sizeof(VERTEX)))
	//		{
	//			found = true;
	//			index = pEntry->index;
	//			break;
	//		}

	//		pEntry = pEntry->pNext;
	//	}
	//}

	//// Vertex was not found in the list. Create a new entry, both within the Vertices list
	//// and also within the hashtable cache
	//if (!found)
	//{
	//	vertices.push_back(vertex);

	//	// Add this to the hashtable
	//	CacheEntry* pNewEntry = new CacheEntry;
	//	if (pNewEntry == NULL)
	//		return (DWORD)E_OUTOFMEMORY;

	//	pNewEntry->index = index;
	//	pNewEntry->pNext = NULL;

	//	// Grow the cache if needed
	//	while ((UINT)m_VertexCache.GetSize() <= hash)
	//	{
	//		m_VertexCache.Add(NULL);
	//	}

	//	// Add to the end of the linked list
	//	CacheEntry* pCurEntry = m_VertexCache.GetAt(hash);
	//	if (pCurEntry == NULL)
	//	{
	//		// This is the head element
	//		m_VertexCache.SetAt(hash, pNewEntry);
	//	}
	//	else
	//	{
	//		// Find the tail
	//		while (pCurEntry->pNext != NULL)
	//		{
	//			pCurEntry = pCurEntry->pNext;
	//		}

	//		pCurEntry->pNext = pNewEntry;
	//	}
	//}

	//return index;
}

ID3DX10Mesh* ResourceLoader::loadMeshFromFile(char* file_name, MeshFileType type)
{
	// File input
	WCHAR str_command[256] = { 0 };
	wifstream in_file(file_name);
	if (!in_file)
		return nullptr;	
	
	vector<D3DXVECTOR3> positions;
	vector<D3DXVECTOR2> texcoords;
	vector<D3DXVECTOR3> normals;

	vector<UINT> indices;
	vector<VERTEX> vertices;

	for (; ;)
	{
		in_file >> str_command;
		if (!in_file)
			break;

		if (0 == wcscmp(str_command, L"#"))
		{
			// Comment
		}
		else if (0 == wcscmp(str_command, L"v"))
		{
			// Vertex Position
			float x, y, z;
			in_file >> x >> y >> z;
			positions.push_back(D3DXVECTOR3(x, y, z));
		}
		else if (0 == wcscmp(str_command, L"vt"))
		{
			// Vertex TexCoord
			float u, v;
			in_file >> u >> v;
			texcoords.push_back(D3DXVECTOR2(u, v));
		}
		else if (0 == wcscmp(str_command, L"vn"))
		{
			// Vertex Normal
			float x, y, z;
			in_file >> x >> y >> z;
			normals.push_back(D3DXVECTOR3(x, y, z));
		}
		else if (0 == wcscmp(str_command, L"f"))
		{
			// Face
			UINT i_position, i_texcoord, i_normal;
			VERTEX vertex;

			for (UINT iFace = 0; iFace < 3; iFace++)
			{
				ZeroMemory(&vertex, sizeof(VERTEX));

				// OBJ format uses 1-based arrays
				in_file >> i_position;
				vertex.position = positions[i_position - 1];

				if ('/' == in_file.peek())
				{
					in_file.ignore();

					if ('/' != in_file.peek())
					{
						// Optional texture coordinate
						in_file >> i_texcoord;
						vertex.texcoord = texcoords[i_texcoord - 1];
					}

					if ('/' == in_file.peek())
					{
						in_file.ignore();

						// Optional vertex normal
						in_file >> i_normal;
						vertex.normal = normals[i_normal - 1];
					}
				}

				// If a duplicate vertex doesn't exist, add this vertex to the Vertices
				// list. Store the index in the Indices array. The Vertices and Indices
				// lists will eventually become the Vertex Buffer and Index Buffer for
				// the mesh.
				addVertex(i_position, vertex, indices, vertices);
			}
		}
		else if (0 == wcscmp(str_command, L"mtllib"))
		{
			// Material library
			WCHAR material_file_name[256];
			in_file >> material_file_name;
		}
		else if (0 == wcscmp(str_command, L"usemtl"))
		{
		}
		else
		{
			// Unimplemented or unrecognized command
		}

		in_file.ignore(1000, '\n');
	}

	// Cleanup
	in_file.close();
	DeleteCache();

	return S_OK;
}