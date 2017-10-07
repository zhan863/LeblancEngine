//======================================================================
//
//	MJP's DX11 Sample Framework
//  http://mynameismjp.wordpress.com/
//
//  All code and content licensed under Microsoft Public License (Ms-PL)
//
//======================================================================

#pragma once

#include "PCH.h"

#include "InterfacePointers.h"

namespace SampleFramework11
{

struct MeshMaterial
{
    XMFLOAT3 DiffuseAlbedo;
    XMFLOAT3 SpecularAlbedo;
    float SpecularPower;
    float Alpha; 
    std::wstring TextureName;
    ID3D11ShaderResourceView* DiffuseMap;
    ID3D11ShaderResourceView* NormalMap;
};

struct MeshPart
{    
    UINT VertexStart;
    UINT VertexCount;
    UINT IndexStart;
    UINT IndexCount;

    MeshMaterial Material;
};

class Mesh
{
public: 

    enum IndexType
    {
        Index16Bit = 0,
        Index32Bit = 1
    };          

    // Lifetime
    Mesh();
    ~Mesh();

    // Loading from file formats
    void CreateFromXFile(ID3D11Device* device, 
                         LPCWSTR fileName, 
                         const WCHAR* normalMapSuffix = NULL,
                         bool generateNormals = false,
                         bool generateTangentFrame = false,
                         IndexType idxType = Index16Bit);

    // Rendering
    void Render(ID3D11DeviceContext* context);

    // Accessors
    ID3D11Buffer* VertexBuffer() { return vertexBuffer; };
    const ID3D11Buffer* VertexBuffer() const { return vertexBuffer; };
    ID3D11Buffer* IndexBuffer() { return indexBuffer; };
    const ID3D11Buffer* IndexBuffer() const { return indexBuffer; };

    std::vector<MeshPart>& MeshParts() { return meshParts; };
    const std::vector<MeshPart>& MeshParts() const { return meshParts; };

    const D3D11_INPUT_ELEMENT_DESC* InputElements() const { return inputElements; };
    UINT NumInputElements() const { return numInputElements; };

    DWORD VertexStride() const { return vertexStride; };
    DWORD NumVertices() const { return numVertices; };
    DWORD NumIndices() const { return numIndices; };

    IndexType IndexBufferType() const { return indexType; };
    DXGI_FORMAT IndexBufferFormat() const { return indexType == Index32Bit ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT; };

protected:

    IDirect3DDevice9Ptr CreateD3D9Device();

    void CreateFromD3DXMesh(const std::wstring& directory, ID3D11Device* device, IDirect3DDevice9* d3d9Device, 
                            ID3DXMesh* mesh, const WCHAR* normalMapSuffix, bool generateNormals, bool GenerateTangentFrame,
                            D3DXMATERIAL* materials, DWORD numMaterials, DWORD* initalAdjacency);    

    ID3DXMesh* GenerateTangentFrame(ID3DXMesh* mesh, IDirect3DDevice9* d3d9Device);
    ID3DXMesh* GenerateNormals(ID3DXMesh* mesh, IDirect3DDevice9* d3d9Device);
    void CreateInputElements(ID3DXMesh* mesh);    
    void GenerateMaterial(MeshPart& part, D3DXMATERIAL& material, const std::wstring& directory, 
                            ID3D11Device* device, const WCHAR* normalMapSuffix);

    ID3D11BufferPtr vertexBuffer;
    ID3D11BufferPtr indexBuffer;

    std::vector<MeshPart> meshParts;
    D3D11_INPUT_ELEMENT_DESC* inputElements;
    UINT numInputElements;

    XMFLOAT3 bBoxMin;
    XMFLOAT3 bBoxMax;

    XMFLOAT3 bSphereCenter;
    float bSphereRadius;

    DWORD* adjacency;
    DWORD vertexStride;
    DWORD numVertices;
    DWORD numIndices;

    IndexType indexType;
};

}