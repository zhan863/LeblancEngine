#include "LeblancEngine/Render/Resource/ResourceLoader/LeblancResourceLoader.h"
#include "LeblancEngine/BasicInclude/LeblancPCH.h"
#include "LeblancEngine/Global/LeblancGlobalContext.h"

DWORD ResourceLoader::addVertex(UINT hash, Vertex vertex, vector<DWORD>& indices, vector<Vertex>& vertices, vector<VertexEntry*>& vertex_hashtable)
{
	bool found = false;
	UINT index = 0;

	// Since it's very slow to check every element in the vertex list, a hashtable stores
	// vertex indices according to the vertex position's index as reported by the OBJ file
	if ((UINT)vertex_hashtable.size() > hash)
	{
		VertexEntry* p_entry = vertex_hashtable[hash];
		while (p_entry != NULL)
		{
			Vertex p_cached_vertex = vertices[p_entry->index];

			// If this vertex is identical to the vertex already in the list, simply
			// point the index buffer to the existing vertex
			if (0 == memcmp(&vertex, &p_cached_vertex, sizeof(Vertex)))
			{
				found = true;
				index = p_entry->index;
				break;
			}

			p_entry = p_entry->next;
		}
	}

	// Vertex was not found in the list. Create a new entry, both within the Vertices list
	// and also within the hash table cache
	if (!found)
	{
		// Add to the Vertices list
		index = vertices.size();
		vertices.push_back(vertex);

		// Add this to the hash table
		VertexEntry* p_new_entry = new VertexEntry;
		if (p_new_entry == NULL)
			return (DWORD)E_OUTOFMEMORY;

		p_new_entry->index = index;
		p_new_entry->next = NULL;

		// Grow the cache if needed
		while ((UINT)vertex_hashtable.size() <= hash)
		{
			vertex_hashtable.push_back(NULL);
		}

		// Add to the end of the linked list
		VertexEntry* p_cur_entry = vertex_hashtable[hash];
		if (p_cur_entry == NULL)
		{
			// This is the head element
			vertex_hashtable[hash] = p_new_entry;
		}
		else
		{
			// Find the tail
			while (p_cur_entry->next != NULL)
			{
				p_cur_entry = p_cur_entry->next;
			}

			p_cur_entry->next = p_new_entry;
		}
	}

	indices.push_back(index);
}

LeblancMesh* ResourceLoader::loadMeshFromFile(const WCHAR* file_name, MeshFileType type)
{
	// File input
	WCHAR str_command[256] = { 0 };
	wifstream in_file(file_name);
	if (!in_file)
		return nullptr;	
	
	vector<XMFLOAT3> positions;
	vector<XMFLOAT2> texcoords;
	vector<XMFLOAT3> normals;
	vector<VertexEntry*> vertex_hashtable;

	vector<DWORD> indices;
	vector<Vertex> vertices;

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
			positions.push_back(XMFLOAT3(x, y, z));
		}
		else if (0 == wcscmp(str_command, L"vt"))
		{
			// Vertex TexCoord
			float u, v;
			in_file >> u >> v;
			texcoords.push_back(XMFLOAT2(u, v));
		}
		else if (0 == wcscmp(str_command, L"vn"))
		{
			// Vertex Normal
			float x, y, z;
			in_file >> x >> y >> z;
			normals.push_back(XMFLOAT3(x, y, z));
		}
		else if (0 == wcscmp(str_command, L"f"))
		{
			// Face
			INT i_position, i_texcoord, i_normal;
			Vertex vertex;

			for (UINT iFace = 0; iFace < 3; iFace++)
			{
				ZeroMemory(&vertex, sizeof(Vertex));

				// OBJ format uses 1-based arrays
				in_file >> i_position;
				i_position = abs(i_position);
				vertex.position = positions[i_position - 1];

				if ('/' == in_file.peek())
				{
					in_file.ignore();

					if ('/' != in_file.peek())
					{
						// Optional texture coordinate
						in_file >> i_texcoord;
						i_texcoord = abs(i_texcoord);
						vertex.texcoord = texcoords[i_texcoord - 1];
					}

					if ('/' == in_file.peek())
					{
						in_file.ignore();

						// Optional vertex normal
						in_file >> i_normal;
						i_normal = abs(i_normal);
						vertex.normal = normals[i_normal - 1];
					}
				}

				// If a duplicate vertex doesn't exist, add this vertex to the Vertices
				// list. Store the index in the Indices array. The Vertices and Indices
				// lists will eventually become the Vertex Buffer and Index Buffer for
				// the mesh.
				addVertex(i_position, vertex, indices, vertices, vertex_hashtable);
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
	
	// Iterate through all the elements in the cache and subsequent linked lists
	for (int i = 0; i < vertex_hashtable.size(); i++)
	{
		VertexEntry* p_entry = vertex_hashtable[i];
		while (p_entry != NULL)
		{
			VertexEntry* next = p_entry->next;
			if (p_entry)
			{
				delete p_entry;
			}

			p_entry = next;
		}
	}
	vertex_hashtable.clear();

	ID3DX10Mesh *p_mesh = nullptr;

	DeviceD3D11& device = g_global_context.m_device_manager.getCurrentDevice();
	return device.createMesh(vertices, indices);
}

void ResourceLoader::loadSceneFromFile(const char* file_name, Scene& scene)
{
	// File input
	WCHAR str_command[256] = { 0 }; 
	WCHAR str_file_name[256] = { 0 };
	wifstream in_file(file_name);
	if (!in_file)
		return;

	for (; ;)
	{
		in_file >> str_command;
		if (!in_file)
			break;

		if (0 == wcscmp(str_command, L"#"))
		{
			// Comment
		}
		else if (0 == wcscmp(str_command, L"s"))
		{
			XMFLOAT4X4 transform;
			in_file >> str_file_name;
			in_file >> transform.m[0][0];
			in_file >> transform.m[0][1];
			in_file >> transform.m[0][2];
			in_file >> transform.m[0][3];
			in_file >> transform.m[1][0];
			in_file >> transform.m[1][1];
			in_file >> transform.m[1][2];
			in_file >> transform.m[1][3];
			in_file >> transform.m[2][0];
			in_file >> transform.m[2][1];
			in_file >> transform.m[2][2];
			in_file >> transform.m[2][3];
			in_file >> transform.m[3][0];
			in_file >> transform.m[3][1];
			in_file >> transform.m[3][2];
			in_file >> transform.m[3][3];

			RenderEntity& render_entity = scene.addRenderEntity();
			render_entity.createFromFile(str_file_name);
			render_entity.setTransform(transform);
		}
		else
		{
			// Unimplemented or unrecognized command
		}

		in_file.ignore(1000, '\n');
	}

	// Cleanup
	in_file.close();
}