//======================================================================
//
//	Tone Mapping Sample
//  by MJP
//  http://mynameismjp.wordpress.com/
//
//  All code and content licensed under Microsoft Public License (Ms-PL)
//
//======================================================================

#include "PCH.h"

#include "ToneMapping.h"

#include "resource.h"
#include "SampleFramework11/InterfacePointers.h"
#include "SampleFramework11/Window.h"  
#include "SampleFramework11/DeviceManager.h"
#include "SampleFramework11/Input.h"
#include "SampleFramework11/SpriteRenderer.h"
#include "SampleFramework11/Mesh.h"
#include "SampleFramework11/Utility.h"
#include "SampleFramework11/Camera.h"
#include "SampleFramework11/ShaderCompilation.h"

using namespace SampleFramework11;
using std::wstring;

const UINT WindowWidth = 1280;
const UINT WindowHeight = 720;
const float WindowWidthF = static_cast<float>(WindowWidth);
const float WindowHeightF = static_cast<float>(WindowHeight);

// Mesh filenames
static const wstring MeshFileNames[] = 
{
    L"Happy.x",
    L"Skull.x",
    L"Knot.x",
    L"Teapot.x",
    L"Sphere.x",
    L"Car.x",    
};

// Scale values applied to the mesh
static const float MeshScales[] =
{
    1.0f,
    0.25f,
    2.0f,
    1.0f,
    0.5f,
    0.5f, 
};

// Environment map filenames
static const wstring EnvMapFileNames[] =
{
    L"GraceCathedral.dds",
    L"Glacier.dds",
    L"Ennis.dds",
    L"Doge.dds",            
    L"Pisa.dds",
    L"Uffizi.dds"
};

// Names of the different tone mapping operators
static const wstring ToneMapOperators[] =
{
    L"None",
    L"Logarithmic",
    L"Exponential",
    L"Drago Logarithmic",
    L"Reinhard",
    L"Reinhard (Modified)",
    L"Filmic (ALU)",
    L"Filmic (Uncharted 2)"
};

// Names of the different auto-exposure types
static const wstring AutoExposureTypes[] =
{
    L"Manual",
    L"Geometric Mean (Manual Key Value)",
    L"Geometric Mean (Auto Key Value)"    
};

ToneMapping::ToneMapping() :  App(L"Sample Test", MAKEINTRESOURCEW(IDI_DEFAULT)),
                                camera(WindowWidthF / WindowHeightF,  PiOver4, 0.1f, 100.0f),
                                currMesh(0),
                                currEnvMap(0),
                                toneMapSlidersStart(0)
{
	deviceManager.SetBackBufferWidth(WindowWidth);
	deviceManager.SetBackBufferHeight(WindowHeight);

	window.SetClientArea(WindowWidth, WindowHeight);    
}

void ToneMapping::BeforeReset()
{

}

void ToneMapping::AfterReset()
{
    float aspect = static_cast<float>(deviceManager.BackBufferWidth()) / deviceManager.BackBufferHeight();
    camera.SetAspectRatio(aspect);

    CreateRenderTargets();
}

void ToneMapping::LoadContent()
{
    ID3D11DevicePtr device = deviceManager.Device();
    ID3D11DeviceContextPtr deviceContext = deviceManager.ImmediateContext();

    // Create a font + SpriteRenderer
    font.Initialize(L"Arial", 18, SpriteFont::Regular, true, device);
    spriteRenderer.Initialize(device);

    // Camera setup    
    camera.SetPosition(XMLoadFloat3(&XMFLOAT3(0, 0, -10.0f)));

    // Mesh shaders
    ID3D10BlobPtr compiledVS;
    meshVS.Attach(CompileVSFromFile(device, L"Mesh.hlsl", "VS", "vs_4_0", NULL, NULL, &compiledVS));
    meshPS.Attach(CompilePSFromFile(device, L"Mesh.hlsl"));

    // Load the meshes
    for (UINT_PTR i = 0; i < NumMeshes; ++i)
    {
        wstring path(L"Content\\Meshes\\");
        path += MeshFileNames[i];        
        meshes[i].CreateFromXFile(device, path.c_str(), NULL, true);

        // Create the input layouts
        DXCall(device->CreateInputLayout(meshes[i].InputElements(), meshes[i].NumInputElements(), 
                                            compiledVS->GetBufferPointer(), compiledVS->GetBufferSize(), 
                                            &meshInputLayouts[i]));
    }      

    meshRotation = XMMatrixIdentity();

    // Init CB's
    meshVSConstants.Initialize(device);
    meshPSConstants.Initialize(device);

    // Load up the environment maps
    for (UINT_PTR i = 0; i < NumEnvMaps; ++i)
    {
        wstring path(L"Content\\EnvMaps\\");
        path += EnvMapFileNames[i];        
        DXCall(D3DX11CreateShaderResourceViewFromFileW(device, path.c_str(), NULL, NULL, &envMaps[i], NULL));
    }

    // Init the skybox
    skybox.Initialize(device);
}

// Creates all required render targets
void ToneMapping::CreateRenderTargets()
{
    ID3D11Device* device = deviceManager.Device();
    UINT width = deviceManager.BackBufferWidth();
    UINT height = deviceManager.BackBufferHeight();

    renderTarget.Initialize(device, width, height, DXGI_FORMAT_R16G16B16A16_FLOAT);
    renderTargetMS.Initialize(device, width, height, DXGI_FORMAT_R16G16B16A16_FLOAT);
    depthBufferMS.Initialize(device, width, height, DXGI_FORMAT_D24_UNORM_S8_UINT);
}

void ToneMapping::Update(const Timer& timer)
{   
    MouseState mouseState = MouseState::GetMouseState(window);
    KeyboardState kbState = KeyboardState::GetKeyboardState();

    if (kbState.IsKeyDown(Keys::Escape))
        window.Destroy();

    float CamMoveSpeed = 5.0f * timer.DeltaSecondsF();
    const float CamRotSpeed = 0.180f * timer.DeltaSecondsF();
    const float MeshRotSpeed = 0.180f * timer.DeltaSecondsF();

    // Move the camera with keyboard input
    if (kbState.IsKeyDown(LeftShift))
        CamMoveSpeed *= 0.25f;

    XMVECTOR camPos = camera.Position();
    if (kbState.IsKeyDown(Keys::W))
        camPos += camera.Forward() * CamMoveSpeed;
    else if (kbState.IsKeyDown(Keys::S))
        camPos += camera.Back() * CamMoveSpeed;
    if (kbState.IsKeyDown(Keys::A))
        camPos += camera.Left() * CamMoveSpeed;
    else if (kbState.IsKeyDown(Keys::D))
        camPos += camera.Right() * CamMoveSpeed;
    if (kbState.IsKeyDown(Keys::Q))
        camPos += camera.Up() * CamMoveSpeed;
    else if (kbState.IsKeyDown(Keys::E))
        camPos += camera.Down() * CamMoveSpeed;
    camera.SetPosition(camPos);

    // Switch the env map with P key
    if (kbState.GetKeyState(P).RisingEdge)
    {
        INT_PTR change = kbState.GetKeyState(RightShift).Pressed ? -1 : 1;
        INT_PTR map = (currEnvMap + change);
        if (map == NumEnvMaps)
            map = 0;
        else if (map < 0)
            map = NumEnvMaps - 1;
        currEnvMap = map;
    }
        
    // Switch the env map with M key
    if (kbState.GetKeyState(M).RisingEdge)
        currMesh = (currMesh + 1) % NumMeshes; 

    // Rotate the camera with the mouse
    if (mouseState.RButton.Pressed && mouseState.IsOverWindow)
    {
        float xRot = camera.XRotation();
        float yRot = camera.YRotation();
        xRot += mouseState.DY * CamRotSpeed;
        yRot += mouseState.DX * CamRotSpeed;
        camera.SetXRotation(xRot);
        camera.SetYRotation(yRot);
    }

    // Rotate the model with the mouse
    if (mouseState.MButton.Pressed && mouseState.IsOverWindow)
    {
        float xRot = -mouseState.DY * MeshRotSpeed;
        float yRot = -mouseState.DX * MeshRotSpeed;
        meshRotation *= XMMatrixRotationRollPitchYaw(xRot, yRot, 0);
    }         
}

void ToneMapping::Render(const Timer& timer)
{
    ID3D11DeviceContextPtr context = deviceManager.ImmediateContext();

	ID3D11RenderTargetView* renderTargets[1] = { deviceManager.BackBuffer() };// { renderTargetMS.RTView };
	context->OMSetRenderTargets(1, renderTargets, depthBufferMS.DSView);

	context->ClearRenderTargetView(renderTargets[0], reinterpret_cast<float*>(&XMFLOAT4(0, 0, 0, 1)));
	context->ClearDepthStencilView(depthBufferMS.DSView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	RenderMesh();
    
    XMFLOAT3 bias = XMFLOAT3(1, 1, 1);
    skybox.Render(context, envMaps[currEnvMap], camera.ViewMatrix(), camera.ProjectionMatrix(), bias);
}

void ToneMapping::RenderMesh()
{
    PIXEvent event(L"Mesh Pass");

    ID3D11DeviceContextPtr context = deviceManager.ImmediateContext();

    // Set states
    float blendFactor[4] = {1, 1, 1, 1};    
    context->RSSetState(rasterizerStates.BackFaceCull());
    context->OMSetBlendState(blendStates.BlendDisabled(), blendFactor, 0xFFFFFFFF);
    context->OMSetDepthStencilState(depthStencilStates.DepthWriteEnabled(), 0);
    ID3D11SamplerState* sampStates[1] = { samplerStates.Anisotropic() };
    context->PSSetSamplers(0, 1, sampStates);

    // Set constant buffers
    float meshScale = MeshScales[currMesh];
    XMMATRIX meshWorld = XMMatrixScaling(meshScale, meshScale, meshScale) * meshRotation;
    meshVSConstants.Data.World = XMMatrixTranspose(meshWorld);
    meshVSConstants.Data.WorldViewProjection = XMMatrixTranspose(meshWorld * camera.ViewProjectionMatrix());
    meshVSConstants.ApplyChanges(context);
    meshVSConstants.SetVS(context, 0);

    meshPSConstants.Data.DiffuseAlbedo = XMFLOAT4(1, 1, 1, 1);
    meshPSConstants.Data.SpecularAlbedo = XMFLOAT4(1, 1, 1, 1);

	meshPSConstants.Data.MainLightDirection = XMFLOAT4(0, 0, -1, 0);
    meshPSConstants.Data.EnvMapBias = XMFLOAT4(1, 1, 1, 1);
    XMStoreFloat4(&meshPSConstants.Data.CameraPosWS, camera.Position());
    meshPSConstants.ApplyChanges(context);
    meshPSConstants.SetPS(context, 0);

    // Set shaders
    context->VSSetShader(meshVS , NULL, 0);
    context->PSSetShader(meshPS, NULL, 0);
    context->GSSetShader(NULL, NULL, 0);
    context->DSSetShader(NULL, NULL, 0);
    context->HSSetShader(NULL, NULL, 0);

    // Set the texture
    ID3D11ShaderResourceView* textures[1] = { envMaps[currEnvMap].GetInterfacePtr() };
    context->PSSetShaderResources(0, 1, textures);

    // Set the input layout
    context->IASetInputLayout(meshInputLayouts[currMesh]);

    // Draw the mesh
    meshes[currMesh].Render(context);
}

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{	    			                                     
	ToneMapping app;
    app.Run();
}


