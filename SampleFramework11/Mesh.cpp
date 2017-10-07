//======================================================================
//
//	MJP's DX11 Sample Framework
//  http://mynameismjp.wordpress.com/
//
//  All code and content licensed under Microsoft Public License (Ms-PL)
//
//======================================================================

#include "PCH.h"

#include "Mesh.h"

#include "Exceptions.h"
#include "Utility.h"

using std::string;
using std::wstring;
using std::vector;
using std::map;
using std::wifstream;

namespace SampleFramework11
{

Mesh::Mesh() :  adjacency(NULL),                 
                vertexStride(0),
                numVertices(0),
                numIndices(0),
                inputElements(NULL),
                numInputElements(0)
{
}

Mesh::~Mesh()
{
    if(adjacency)
        delete[] adjacency;
    if(inputElements)
        delete[] inputElements;

    for(size_t i = 0; i < meshParts.size(); ++i)
        if(meshParts[i].Material.DiffuseMap)
            meshParts[i].Material.DiffuseMap->Release();
}

void Mesh::CreateFromXFile(ID3D11Device* device, LPCWSTR fileName, 
                            const WCHAR* normalMapSuffix, bool generateNormals, 
                            bool generateTangentFrame, IndexType idxType)
{
    indexType = idxType;

    _ASSERT(FileExists(fileName));

    IDirect3DDevice9Ptr d3d9Device = CreateD3D9Device(); 

    // Load the D3DX mesh
    ID3DXMesh* mesh = NULL;
    ID3DXBuffer* adjacencyBuffer = NULL;
    ID3DXBuffer* materialsBuffer = NULL;
    DWORD numMaterials = 0;    

    UINT options = D3DXMESH_MANAGED;
    if (indexType == Index32Bit)
        options |= D3DXMESH_32BIT;
    DXCall(D3DXLoadMeshFromXW(fileName, options, d3d9Device, &adjacencyBuffer, 
                                &materialsBuffer, NULL, &numMaterials, &mesh));

    DWORD* initalAdjacency = reinterpret_cast<DWORD*>(adjacencyBuffer->GetBufferPointer());
    D3DXMATERIAL* materials = reinterpret_cast<D3DXMATERIAL*>(materialsBuffer->GetBufferPointer());

    // Get the directory the mesh was loaded from    
    wstring fileDirectory = GetDirectoryFromFileName(fileName);

    CreateFromD3DXMesh(fileDirectory, device, d3d9Device, mesh, normalMapSuffix, generateNormals,
                        generateTangentFrame, materials, numMaterials, initalAdjacency);

    adjacencyBuffer->Release();
    materialsBuffer->Release();    
}

IDirect3DDevice9Ptr Mesh::CreateD3D9Device()
{
    // Create a D3D9 device (would make it NULL, but PIX doesn't seem to like that)
    IDirect3D9Ptr d3d9;
    d3d9.Attach(Direct3DCreate9(D3D_SDK_VERSION));    

    D3DPRESENT_PARAMETERS pp;
    pp.BackBufferWidth = 1;
    pp.BackBufferHeight = 1;
    pp.BackBufferFormat = D3DFMT_X8R8G8B8;
    pp.BackBufferCount = 1;
    pp.MultiSampleType = D3DMULTISAMPLE_NONE;
    pp.MultiSampleQuality = 0;
    pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    pp.hDeviceWindow = GetDesktopWindow();
    pp.Windowed = true;
    pp.Flags = 0;
    pp.FullScreen_RefreshRateInHz = 0;
    pp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
    pp.EnableAutoDepthStencil = false;

    IDirect3DDevice9Ptr d3d9Device;
    DXCall(d3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, NULL, 
        D3DCREATE_HARDWARE_VERTEXPROCESSING, &pp, &d3d9Device));

    return d3d9Device;
}

void Mesh::CreateFromD3DXMesh(const wstring& directory, ID3D11Device* device, IDirect3DDevice9* d3d9Device, 
                                ID3DXMesh* mesh, const WCHAR* normalMapSuffix, bool generateNormals, bool generateTangentFrame,
                                D3DXMATERIAL* materials, DWORD numMaterials, DWORD* initalAdjacency)
{
    // Optimize the mesh
    adjacency = new DWORD[mesh->GetNumFaces() * 3];
    DXCall(mesh->OptimizeInplace(D3DXMESHOPT_VERTEXCACHE|D3DXMESHOPT_ATTRSORT, 
            initalAdjacency, adjacency, NULL, NULL));    

    if (generateTangentFrame)    
        mesh = GenerateTangentFrame(mesh, d3d9Device);    
    else if (generateNormals)    
        mesh = GenerateNormals(mesh, d3d9Device);    

    // Get some of the mesh info
    vertexStride = mesh->GetNumBytesPerVertex();
    numVertices = mesh->GetNumVertices();
    numIndices = mesh->GetNumFaces() * 3;

    // Convert the D3D9 vertex declaration to a D3D11 input element desc    
    CreateInputElements(mesh);

    // Copy over the vertex data
    void* vertices = NULL;
    DXCall(mesh->LockVertexBuffer(0, &vertices));

    D3D11_BUFFER_DESC bufferDesc;
    bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
    bufferDesc.ByteWidth = vertexStride * numVertices;
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.CPUAccessFlags = 0;
    bufferDesc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA initData;
    initData.pSysMem = vertices;
    initData.SysMemPitch = 0;
    initData.SysMemSlicePitch = 0;
    DXCall(device->CreateBuffer(&bufferDesc, &initData, &vertexBuffer));

    // Compute bounding box and sphere
    D3DXVECTOR3* boxMin = reinterpret_cast<D3DXVECTOR3*>(&bBoxMin);
    D3DXVECTOR3* boxMax = reinterpret_cast<D3DXVECTOR3*>(&bBoxMax);
    DXCall(D3DXComputeBoundingBox(reinterpret_cast<D3DXVECTOR3*>(vertices), numVertices, vertexStride, boxMin, boxMax));

    D3DXVECTOR3* sphereCenter = reinterpret_cast<D3DXVECTOR3*>(&bSphereCenter);
    DXCall(D3DXComputeBoundingSphere(reinterpret_cast<D3DXVECTOR3*>(vertices), numVertices, vertexStride, sphereCenter, &bSphereRadius));

    DXCall(mesh->UnlockVertexBuffer());

    // Copy over the index data
    void* indices = NULL;
    DXCall(mesh->LockIndexBuffer(0, &indices));

    UINT indexSize = indexType == Index32Bit ? 4 : 2;
    bufferDesc.ByteWidth = indexSize * numIndices;
    bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

    initData.pSysMem = indices;
    DXCall(device->CreateBuffer(&bufferDesc, &initData, &indexBuffer));

    DXCall(mesh->UnlockIndexBuffer());

    // Copy in the materials
    for (UINT i = 0; i < numMaterials; ++i)
    {
        MeshPart part;        
        GenerateMaterial(part, materials[i], directory, device, normalMapSuffix);
        meshParts.push_back(part);
    }

    // Copy in the subset info
    D3DXATTRIBUTERANGE* attributeTable = new D3DXATTRIBUTERANGE[numMaterials];
    DXCall(mesh->GetAttributeTable(attributeTable, &numMaterials));
    for(UINT i = 0; i < numMaterials; ++i)
    {
        MeshPart& part = meshParts[i];
        part.VertexStart = attributeTable[i].VertexStart;
        part.VertexCount = attributeTable[i].VertexCount;
        part.IndexStart = attributeTable[i].FaceStart * 3;
        part.IndexCount = attributeTable[i].FaceCount * 3;
    }

    delete[] attributeTable;
    mesh->Release();
}

ID3DXMesh* Mesh::GenerateTangentFrame(ID3DXMesh* mesh, IDirect3DDevice9* d3d9Device)
{
    // make sure we have a texture coordinate
    D3DVERTEXELEMENT9 decl[MAX_FVF_DECL_SIZE];        
    DXCall(mesh->GetDeclaration(decl));
    bool foundTexCoord = false;
    for (UINT i = 0; i < MAX_FVF_DECL_SIZE; ++i)
    {
        if (decl[i].Stream == 0xFF)
            break;
        else if(decl[i].Usage == D3DDECLUSAGE_TEXCOORD && decl[i].UsageIndex == 0)
        {
            foundTexCoord = true;
            break;
        }
    }

    _ASSERT(foundTexCoord);

    // Clone the mesh with a new declaration
    D3DVERTEXELEMENT9 newDecl[] = 
    {
        { 0, 0,  D3DDECLTYPE_FLOAT3,  D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
        { 0, 12, D3DDECLTYPE_FLOAT3,  D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0 },
        { 0, 24, D3DDECLTYPE_FLOAT2,  D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
        { 0, 32, D3DDECLTYPE_FLOAT3,  D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT,  0 },
        { 0, 44, D3DDECLTYPE_FLOAT3,  D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL,  0 },
        D3DDECL_END()
    };

    ID3DXMesh* clonedMesh = NULL;

    UINT options = D3DXMESH_MANAGED;
    if (indexType == Index32Bit)
        options |= D3DXMESH_32BIT;
    DXCall(mesh->CloneMesh(options, newDecl, d3d9Device, &clonedMesh));
    mesh->Release();        

    // Calculate the tangent frame
    DXCall(D3DXComputeTangentFrameEx(clonedMesh, 
        D3DDECLUSAGE_TEXCOORD, 0,   
        D3DDECLUSAGE_BINORMAL, 0, 
        D3DDECLUSAGE_TANGENT, 0, 
        D3DDECLUSAGE_NORMAL, 0, 
        D3DXTANGENT_CALCULATE_NORMALS | D3DXTANGENT_GENERATE_IN_PLACE,
        adjacency, 0.01f, 0.25f, 0.01f, NULL, NULL));
        
    return clonedMesh;               
}

ID3DXMesh* Mesh::GenerateNormals(ID3DXMesh* mesh, IDirect3DDevice9* d3d9Device)
{
    // see if we have texture coordinates
    D3DVERTEXELEMENT9 decl[MAX_FVF_DECL_SIZE];        
    DXCall(mesh->GetDeclaration(decl));
    bool foundTexCoord = false;
    for (UINT i = 0; i < MAX_FVF_DECL_SIZE; ++i)
    {
        if (decl[i].Stream == 0xFF)
            break;
        else if(decl[i].Usage == D3DDECLUSAGE_TEXCOORD && decl[i].UsageIndex == 0)
        {
            foundTexCoord = true;
            break;
        }
    }

    // Clone the mesh with a new declaration
    D3DVERTEXELEMENT9 tcDecl[] = 
    {
        { 0, 0,  D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
        { 0, 12, D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0 },
        { 0, 24, D3DDECLTYPE_FLOAT2,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
        D3DDECL_END()
    };

    D3DVERTEXELEMENT9 noTCDecl[] = 
    {
        { 0, 0,  D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
        { 0, 12, D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0 },
        { 0, 24, D3DDECLTYPE_FLOAT2,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
        D3DDECL_END()
    };

    D3DVERTEXELEMENT9* newDecl;
    if (foundTexCoord)
        newDecl = tcDecl;
    else
        newDecl = noTCDecl;

    ID3DXMesh* clonedMesh = NULL;

    UINT options = D3DXMESH_MANAGED;
    if (indexType == Index32Bit)
        options |= D3DXMESH_32BIT;
    DXCall(mesh->CloneMesh(options, newDecl, d3d9Device, &clonedMesh));
    mesh->Release();    

    // Generate the normals
    DXCall(D3DXComputeNormals(clonedMesh, adjacency));

    return clonedMesh;
}

void Mesh::CreateInputElements(ID3DXMesh* mesh)
{
    map<BYTE, LPCSTR> nameMap;
    nameMap[D3DDECLUSAGE_POSITION] = "POSITION";
    nameMap[D3DDECLUSAGE_BLENDWEIGHT] = "BLENDWEIGHT";
    nameMap[D3DDECLUSAGE_BLENDINDICES] = "BLENDINDICES";
    nameMap[D3DDECLUSAGE_NORMAL] = "NORMAL";
    nameMap[D3DDECLUSAGE_TEXCOORD] = "TEXCOORD";
    nameMap[D3DDECLUSAGE_TANGENT] = "TANGENT";
    nameMap[D3DDECLUSAGE_BINORMAL] = "BINORMAL";
    nameMap[D3DDECLUSAGE_COLOR] = "COLOR";

    map<BYTE, DXGI_FORMAT> formatMap;
    formatMap[D3DDECLTYPE_FLOAT1] = DXGI_FORMAT_R32_FLOAT;
    formatMap[D3DDECLTYPE_FLOAT2] = DXGI_FORMAT_R32G32_FLOAT;
    formatMap[D3DDECLTYPE_FLOAT3] = DXGI_FORMAT_R32G32B32_FLOAT;
    formatMap[D3DDECLTYPE_FLOAT4] = DXGI_FORMAT_R32G32B32A32_FLOAT;
    formatMap[D3DDECLTYPE_D3DCOLOR] = DXGI_FORMAT_R8G8B8A8_UNORM;
    formatMap[D3DDECLTYPE_UBYTE4] = DXGI_FORMAT_R8G8B8A8_UINT;
    formatMap[D3DDECLTYPE_SHORT2] = DXGI_FORMAT_R16G16_SINT;
    formatMap[D3DDECLTYPE_SHORT4] = DXGI_FORMAT_R16G16B16A16_SINT;
    formatMap[D3DDECLTYPE_UBYTE4N] = DXGI_FORMAT_R8G8B8A8_UNORM;
    formatMap[D3DDECLTYPE_SHORT2N] = DXGI_FORMAT_R16G16_SNORM;
    formatMap[D3DDECLTYPE_SHORT4N] = DXGI_FORMAT_R16G16B16A16_SNORM;
    formatMap[D3DDECLTYPE_USHORT2N] = DXGI_FORMAT_R16G16_UNORM;
    formatMap[D3DDECLTYPE_USHORT4N] = DXGI_FORMAT_R16G16B16A16_UNORM;
    formatMap[D3DDECLTYPE_UDEC3] = DXGI_FORMAT_R10G10B10A2_UINT;
    formatMap[D3DDECLTYPE_DEC3N] = DXGI_FORMAT_R10G10B10A2_UNORM;
    formatMap[D3DDECLTYPE_FLOAT16_2] = DXGI_FORMAT_R16G16_FLOAT;
    formatMap[D3DDECLTYPE_FLOAT16_4] = DXGI_FORMAT_R16G16B16A16_FLOAT;

    // Figure out the number of elements
    D3DVERTEXELEMENT9 declaration [MAX_FVF_DECL_SIZE];
    DXCall(mesh->GetDeclaration(declaration));
    while(declaration[numInputElements].Stream != 0xFF)
        numInputElements++;

    inputElements = new D3D11_INPUT_ELEMENT_DESC[numInputElements];

    for (UINT64 elemIdx = 0; elemIdx < numInputElements; ++elemIdx)
    {
        D3DVERTEXELEMENT9& element9 = declaration[elemIdx];
        D3D11_INPUT_ELEMENT_DESC& element11 = inputElements[elemIdx];
        element11.InputSlot = 0;
        element11.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
        element11.InstanceDataStepRate = 0;
        element11.SemanticName = nameMap[element9.Usage];
        element11.Format = formatMap[element9.Type];
        element11.AlignedByteOffset = element9.Offset;
        element11.SemanticIndex = element9.UsageIndex;         
    }
}

void Mesh::GenerateMaterial(MeshPart& part, D3DXMATERIAL& material, const std::wstring& directory, 
                                            ID3D11Device* device, const WCHAR* normalMapSuffix)
{
    part.Material.DiffuseAlbedo = XMFLOAT3(reinterpret_cast<float*>(&material.MatD3D.Diffuse));
    part.Material.SpecularAlbedo = XMFLOAT3(reinterpret_cast<float*>(&material.MatD3D.Specular));
    part.Material.SpecularPower = material.MatD3D.Power;
    part.Material.Alpha = material.MatD3D.Diffuse.a;

    if (material.pTextureFilename)
    {
        // Load the diffuse map
        part.Material.TextureName = AnsiToWString(material.pTextureFilename);
        wstring diffuseMapPath = directory + part.Material.TextureName;

        if (part.Material.TextureName.length() > 0 && FileExists(diffuseMapPath.c_str()))        
            DXCall(D3DX11CreateShaderResourceViewFromFileW(device, diffuseMapPath.c_str(), NULL, NULL, &part.Material.DiffuseMap, NULL));

        // Load the normal map
        if (normalMapSuffix)
        {
            static const WCHAR* ImageFileExtensions[] = { L".png", L".jpg", L".dds", L"bmp", L".tif", L".tiff", L".gif" };

            wstring normalMapName = GetFileNameWithoutExtension(diffuseMapPath.c_str()) + normalMapSuffix;
            if (normalMapName.length() > 0)
            {
                for (UINT64 i = 0; i < sizeof(ImageFileExtensions) / sizeof(WCHAR*); ++i)
                {
                    wstring normalMapPath = normalMapName + ImageFileExtensions[i];
                    if (FileExists(normalMapPath.c_str()))
                        DXCall(D3DX11CreateShaderResourceViewFromFileW(device, normalMapPath.c_str(), NULL, NULL, &part.Material.NormalMap, NULL));
                }
            }
        }
    }        
    else
    {
        part.Material.DiffuseMap = NULL;
        part.Material.NormalMap = NULL;
    }
}

// Does a basic draw of all parts
void Mesh::Render(ID3D11DeviceContext* context)
{
    // Set the vertices and indices     
    ID3D11Buffer* vertexBuffers[1] = { vertexBuffer };  
    UINT vertexStrides[1] = { vertexStride };
    UINT offsets[1] = { 0 };
    context->IASetVertexBuffers(0, 1, vertexBuffers, vertexStrides, offsets);    
    context->IASetIndexBuffer(indexBuffer, IndexBufferFormat(), 0);
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // Draw each MeshPart    
    for(size_t i = 0; i < meshParts.size(); ++i)
    {
        MeshPart& meshPart = meshParts[i];        
        context->DrawIndexed(meshPart.IndexCount, meshPart.IndexStart, 0);        
    }
}

}
