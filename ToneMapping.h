//======================================================================
//
//	Tone Mapping Sample
//  by MJP
//  http://mynameismjp.wordpress.com/
//
//  All code and content licensed under Microsoft Public License (Ms-PL)
//
//======================================================================

#pragma once

#include "SampleFramework11/PCH.h"

#include "SampleFramework11/App.h"
#include "SampleFramework11/InterfacePointers.h"
#include "SampleFramework11/Camera.h"
#include "SampleFramework11/Mesh.h"
#include "SampleFramework11/Skybox.h"
#include "SampleFramework11/GraphicsTypes.h"

using namespace SampleFramework11;

class ToneMapping : public App
{

public:

    // Constants
    static const UINT_PTR NumEnvMaps = 6;
    static const UINT_PTR NumMeshes = 6;

protected:

    // Tone mapping curves
    enum ToneMappingType
    {
        None = 0,
        Logarithmic,
        Exponential,
        DragoLogarithmic,
        Reinhard,
        ReinhardModified,
        FilmicALU,
        FilmicUncharted ,

        NumToneMappingTypes
    };

    // Auto exposure types
    enum AutoExposureType
    {
        Manual = 0,
        GeometricMean = 1,
        GeometricMeanAutoKey = 2,

        NumAutoExposureTypes
    };

    FirstPersonCamera camera;

    Skybox skybox;

    RenderTarget2D renderTargetMS;
    DepthStencilBuffer depthBufferMS;
    RenderTarget2D renderTarget;    

    UINT_PTR toneMapSlidersStart;    

    // Meshes
    Mesh meshes[NumMeshes];
    ID3D11InputLayoutPtr meshInputLayouts[NumMeshes];
    UINT_PTR currMesh;
    ID3D11VertexShaderPtr meshVS;
    ID3D11PixelShaderPtr meshPS;
    XMMATRIX meshRotation;

    // Environment maps    
    ID3D11ShaderResourceViewPtr envMaps[NumEnvMaps];
    UINT_PTR currEnvMap;

    // Constant buffers
    struct MeshVSConstants
    {
        XMMATRIX World;
        XMMATRIX WorldViewProjection;
    };

    struct MeshPSConstants
    {
        XMFLOAT4 DiffuseAlbedo;
        XMFLOAT4 SpecularAlbedo;
        XMFLOAT4 EnvMapBias;
        XMFLOAT4 CameraPosWS;
		XMFLOAT4 MainLightDirection;
    };

    ConstantBuffer<MeshVSConstants> meshVSConstants;
    ConstantBuffer<MeshPSConstants> meshPSConstants;
        
    virtual void LoadContent();
    virtual void Render(const Timer& timer);
    virtual void Update(const Timer& timer);
    virtual void BeforeReset();
    virtual void AfterReset();

    void CreateRenderTargets();

    void RenderMesh();

public:

    ToneMapping();    
};

