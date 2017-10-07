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
                                tmType(None),
                                autoExposureType(Manual),
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

    postProcessor.AfterReset(deviceManager.BackBufferWidth(), deviceManager.BackBufferHeight());

    AdjustGUI();
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

    // Init the post processor
    postProcessor.Initialize(device);

    InitializeGUI();

    toneGraph.Initialize(device);
}

// Creates the sliders
void ToneMapping::InitializeGUI()
{
    ID3D11DevicePtr device = deviceManager.Device();

    // Sliders
    diffuseRSlider.Initalize(device);    
    diffuseRSlider.MinVal() = 0.0f;
    diffuseRSlider.MaxVal() = 1.0f;
    diffuseRSlider.NumSteps() = 100;
    diffuseRSlider.Value() = 0.25f;
    diffuseRSlider.Name() = L"Diffuse R";
    guiObjects.push_back(&diffuseRSlider);

    diffuseGSlider.Initalize(device);    
    diffuseGSlider.MinVal() = 0.0f;
    diffuseGSlider.MaxVal() = 1.0f;
    diffuseGSlider.NumSteps() = 100;
    diffuseGSlider.Value() = 0.25f;
    diffuseGSlider.Name() = L"Diffuse G";
    guiObjects.push_back(&diffuseGSlider);    

    diffuseBSlider.Initalize(device);    
    diffuseBSlider.MinVal() = 0.0f;
    diffuseBSlider.MaxVal() = 1.0f;
    diffuseBSlider.NumSteps() = 100;
    diffuseBSlider.Value() = 0.25f;
    diffuseBSlider.Name() = L"Diffuse B";
    guiObjects.push_back(&diffuseBSlider);    

    specularRSlider.Initalize(device);    
    specularRSlider.MinVal() = 0.0f;
    specularRSlider.MaxVal() = 1.0f;
    specularRSlider.NumSteps() = 100;
    specularRSlider.Value() = 0.75f;
    specularRSlider.Name() = L"Specular R";
    guiObjects.push_back(&specularRSlider);    

    specularGSlider.Initalize(device);    
    specularGSlider.MinVal() = 0.0f;
    specularGSlider.MaxVal() = 1.0f;
    specularGSlider.NumSteps() = 100;
    specularGSlider.Value() = 0.75f;
    specularGSlider.Name() = L"Specular G";
    guiObjects.push_back(&specularGSlider);    

    specularBSlider.Initalize(device);    
    specularBSlider.MinVal() = 0.0f;
    specularBSlider.MaxVal() = 1.0f;
    specularBSlider.NumSteps() = 100;
    specularBSlider.Value() = 0.75f;
    specularBSlider.Name() = L"Specular B";
    guiObjects.push_back(&specularBSlider);    

    bloomThresholdSlider.Initalize(device);    
    bloomThresholdSlider.MinVal() = 0.0f;
    bloomThresholdSlider.MaxVal() = 10.0f;
    bloomThresholdSlider.NumSteps() = 100;
    bloomThresholdSlider.Value() = 2.0f;
    bloomThresholdSlider.Name() = L"Bloom Threshold";
    guiObjects.push_back(&bloomThresholdSlider);    

    bloomMagSlider.Initalize(device);    
    bloomMagSlider.MinVal() = 0.0f;
    bloomMagSlider.MaxVal() = 2.0f;
    bloomMagSlider.NumSteps() = 100;
    bloomMagSlider.Value() = 0.0f;
    bloomMagSlider.Name() = L"Bloom Magnitude";
    guiObjects.push_back(&bloomMagSlider);    

    bloomBlurSigma.Initalize(device);    
    bloomBlurSigma.MinVal() = 0.5f;
    bloomBlurSigma.MaxVal() = 1.5f;
    bloomBlurSigma.NumSteps() = 100;
    bloomBlurSigma.Value() = 0.8f;
    bloomBlurSigma.Name() = L"Bloom Blur Sigma";
    guiObjects.push_back(&bloomBlurSigma);        

    exposureSlider.Initalize(device);    
    exposureSlider.MinVal() = -10.0f;
    exposureSlider.MaxVal() = 10.0f;
    exposureSlider.NumSteps() = 100;
    exposureSlider.Value() = 0.0f;
    exposureSlider.Name() = L"Exposure";
    guiObjects.push_back(&exposureSlider);    

    keyValueSlider.Initalize(device);
    keyValueSlider.MinVal() = 0.0f;
    keyValueSlider.MaxVal() = 1.0;
    keyValueSlider.NumSteps() = 100;
    keyValueSlider.Value() = 0.18f;
    keyValueSlider.Name() = L"Auto-Exposure Key Value";
    guiObjects.push_back(&keyValueSlider);

    lumMapMipSlider.Initalize(device);
    lumMapMipSlider.MinVal() = 0.0f;
    lumMapMipSlider.MaxVal() = 10.0f;
    lumMapMipSlider.NumSteps() = 100;
    lumMapMipSlider.Value() = 10.0f;
    lumMapMipSlider.Name() = L"Luminance Map Mip Level";
    guiObjects.push_back(&lumMapMipSlider);

    adaptationRateSlider.Initalize(device);
    adaptationRateSlider.MinVal() = 0.0f;
    adaptationRateSlider.MaxVal() = 4.0f;
    adaptationRateSlider.NumSteps() = 100;
    adaptationRateSlider.Value() = 1.25f;
    adaptationRateSlider.Name() = L"Adaptation Rate";
    guiObjects.push_back(&adaptationRateSlider);

    toneMapSlidersStart = guiObjects.size() - 1;

    luminanceSaturationSlider.Initalize(device);    
    luminanceSaturationSlider.MinVal() = 0.0f;
    luminanceSaturationSlider.MaxVal() = 4.0f;
    luminanceSaturationSlider.NumSteps() = 100;
    luminanceSaturationSlider.Value() = 1.0f;
    luminanceSaturationSlider.Name() = L"Luminance Saturation";
    guiObjects.push_back(&luminanceSaturationSlider);                

    whiteSlider.Initalize(device);    
    whiteSlider.MinVal() = 0.0f;
    whiteSlider.MaxVal() = 25.0f;
    whiteSlider.NumSteps() = 100;
    whiteSlider.Value() = 5.0f;
    whiteSlider.Name() = L"White Luminance";
    guiObjects.push_back(&whiteSlider);
    
    biasSlider.Initalize(device);    
    biasSlider.MinVal() = 0.0f;
    biasSlider.MaxVal() = 1.0f;
    biasSlider.NumSteps() = 100;
    biasSlider.Value() = 0.5f;
    biasSlider.Name() = L"Bias";
    guiObjects.push_back(&biasSlider);

    shoulderStrengthSlider.Initalize(device);    
    shoulderStrengthSlider.MinVal() = 0.0f;
    shoulderStrengthSlider.MaxVal() = 2.0f;
    shoulderStrengthSlider.NumSteps() = 100;
    shoulderStrengthSlider.Value() = 0.22f;
    shoulderStrengthSlider.Name() = L"Shoulder Strength";
    guiObjects.push_back(&shoulderStrengthSlider);    
    
    linearStrengthSlider.Initalize(device);    
    linearStrengthSlider.MinVal() = 0.0f;
    linearStrengthSlider.MaxVal() = 5.0f;
    linearStrengthSlider.NumSteps() = 100;
    linearStrengthSlider.Value() = 0.3f;
    linearStrengthSlider.Name() = L"Linear Strength";
    guiObjects.push_back(&linearStrengthSlider);    
    
    linearAngleSlider.Initalize(device);    
    linearAngleSlider.MinVal() = 0.0f;
    linearAngleSlider.MaxVal() = 1.0f;
    linearAngleSlider.NumSteps() = 100;
    linearAngleSlider.Value() = 0.1f;
    linearAngleSlider.Name() = L"Linear Angle";
    guiObjects.push_back(&linearAngleSlider);    
    
    toeStrengthSlider.Initalize(device);    
    toeStrengthSlider.MinVal() = 0.0f;
    toeStrengthSlider.MaxVal() = 2.0f;
    toeStrengthSlider.NumSteps() = 100;
    toeStrengthSlider.Value() = 0.2f;
    toeStrengthSlider.Name() = L"Toe Strength";
    guiObjects.push_back(&toeStrengthSlider); 

    toeNumeratorSlider.Initalize(device);    
    toeNumeratorSlider.MinVal() = 0.0f;
    toeNumeratorSlider.MaxVal() = 0.5f;
    toeNumeratorSlider.NumSteps() = 100;
    toeNumeratorSlider.Value() = 0.01f;
    toeNumeratorSlider.Name() = L"Toe Numerator";
    guiObjects.push_back(&toeNumeratorSlider);  
    
    toeDenominatorSlider.Initalize(device);    
    toeDenominatorSlider.MinVal() = 0.0f;
    toeDenominatorSlider.MaxVal() = 2.0f;
    toeDenominatorSlider.NumSteps() = 100;
    toeDenominatorSlider.Value() = 0.3f;
    toeDenominatorSlider.Name() = L"Toe Denominator";
    guiObjects.push_back(&toeDenominatorSlider);    
    
    linearWhiteSlider.Initalize(device);    
    linearWhiteSlider.MinVal() = 0.0f;
    linearWhiteSlider.MaxVal() = 20.0f;
    linearWhiteSlider.NumSteps() = 1000;
    linearWhiteSlider.Value() = 11.2f;
    linearWhiteSlider.Name() = L"Linear White";
    guiObjects.push_back(&linearWhiteSlider);

    // Keep track of which sliders should be on for each tone mapping method
    toneMappingSliders[Logarithmic].push_back(&luminanceSaturationSlider);
    toneMappingSliders[Logarithmic].push_back(&whiteSlider);

    toneMappingSliders[Exponential].push_back(&luminanceSaturationSlider);
    toneMappingSliders[Exponential].push_back(&whiteSlider);

    toneMappingSliders[DragoLogarithmic].push_back(&luminanceSaturationSlider);
    toneMappingSliders[DragoLogarithmic].push_back(&whiteSlider);
    toneMappingSliders[DragoLogarithmic].push_back(&biasSlider);
    
    toneMappingSliders[Reinhard].push_back(&luminanceSaturationSlider);
    
    toneMappingSliders[ReinhardModified].push_back(&luminanceSaturationSlider);
    toneMappingSliders[ReinhardModified].push_back(&whiteSlider);
    
    toneMappingSliders[FilmicUncharted].push_back(&shoulderStrengthSlider);
    toneMappingSliders[FilmicUncharted].push_back(&linearStrengthSlider);
    toneMappingSliders[FilmicUncharted].push_back(&linearAngleSlider);
    toneMappingSliders[FilmicUncharted].push_back(&toeStrengthSlider);
    toneMappingSliders[FilmicUncharted].push_back(&toeNumeratorSlider);
    toneMappingSliders[FilmicUncharted].push_back(&toeDenominatorSlider);
    toneMappingSliders[FilmicUncharted].push_back(&linearWhiteSlider);

    AdjustGUI();
}

// Adjusts the position of the sliders
void ToneMapping::AdjustGUI()
{
    float width = static_cast<float>(deviceManager.BackBufferWidth());
    float x = width - 300;
    float y = 10.0f;
    const float Spacing = 40.0f;

    for (UINT_PTR i = 0; i < guiObjects.size(); ++i)
    {
        guiObjects[i]->Position() = XMFLOAT2(x, y);
        y += Spacing;

        if (i == toneMapSlidersStart)
        {
            x += 150;
            y = 10;
        }
    }
}

// Creates all required render targets
void ToneMapping::CreateRenderTargets()
{
    ID3D11Device* device = deviceManager.Device();
    UINT width = deviceManager.BackBufferWidth();
    UINT height = deviceManager.BackBufferHeight();

    renderTarget.Initialize(device, width, height, DXGI_FORMAT_R16G16B16A16_FLOAT);
    renderTargetMS.Initialize(device, width, height, DXGI_FORMAT_R16G16B16A16_FLOAT, 1, 4);
    depthBufferMS.Initialize(device, width, height, DXGI_FORMAT_D24_UNORM_S8_UINT, false, 4);
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

    // Switch tone mapping mode with L key
    if (kbState.GetKeyState(L).RisingEdge)
    {
        INT_PTR change = kbState.GetKeyState(RightShift).Pressed ? -1 : 1;
        INT_PTR type = (tmType + change);
        if (type == NumToneMappingTypes)
            type = 0;
        else if (type < 0)
            type = NumToneMappingTypes - 1;
        tmType = static_cast<ToneMappingType>(type);
    }

    // Enable/Disable auto-exposure with K key
    if (kbState.GetKeyState(K).RisingEdge)
    {
        INT_PTR change = kbState.GetKeyState(RightShift).Pressed ? -1 : 1;
        INT_PTR type = (autoExposureType + change);
        if (type == NumAutoExposureTypes)
            type = 0;
        else if (type < 0)
            type = NumAutoExposureTypes - 1;
        autoExposureType = static_cast<AutoExposureType>(type);
    }

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

    // Enable or disable sliders based on current tone mapping mode
    for (UINT_PTR i = toneMapSlidersStart; i < guiObjects.size(); ++i)
            guiObjects[i]->Enabled() = false;

    for(UINT_PTR i = 0; i < toneMappingSliders[tmType].size(); ++i)
        toneMappingSliders[tmType][i]->Enabled() = true;

    keyValueSlider.Enabled() = autoExposureType == GeometricMean;
    exposureSlider.Enabled() = autoExposureType == Manual;
    lumMapMipSlider.Enabled() = (autoExposureType == GeometricMean || autoExposureType == GeometricMeanAutoKey);
    adaptationRateSlider.Enabled() = lumMapMipSlider.Enabled();  

    for (UINT_PTR i = 0; i < guiObjects.size(); ++i)
        guiObjects[i]->Update(mouseState.X, mouseState.Y, mouseState.LButton.Pressed);          
}

void ToneMapping::Render(const Timer& timer)
{
    ID3D11DeviceContextPtr context = deviceManager.ImmediateContext();

    ID3D11RenderTargetView* renderTargets[1] = { renderTargetMS.RTView };
    context->OMSetRenderTargets(1, renderTargets, depthBufferMS.DSView);

    context->ClearRenderTargetView(renderTarget.RTView, reinterpret_cast<float*>(&XMFLOAT4(0, 0, 0, 1)));
    context->ClearDepthStencilView(depthBufferMS.DSView, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);

    RenderMesh();
    
    XMFLOAT3 bias = XMFLOAT3(1, 1, 1);
    skybox.Render(context, envMaps[currEnvMap], camera.ViewMatrix(), camera.ProjectionMatrix(), bias);

    // Resolve the multisampled render target
    context->ResolveSubresource(renderTarget.Texture, 0, renderTargetMS.Texture, 0, renderTarget.Format);

    D3DPERF_BeginEvent(0xFFFFFFFF, L"Post Processing");
    TMPostProcessor::Constants constants;
    constants.BloomThreshold = bloomThresholdSlider.Value();
    constants.BloomMagnitude = bloomMagSlider.Value();
    constants.BloomBlurSigma = bloomBlurSigma.Value();    
    constants.WhiteLevel = whiteSlider.Value();
    constants.Tau = adaptationRateSlider.Value();
    constants.Exposure = exposureSlider.Value();
    constants.AutoExposure = static_cast<float>(autoExposureType);
    constants.KeyValue = keyValueSlider.Value();
    constants.TimeDelta = timer.DeltaSecondsF();
    constants.ToneMapTechnique = static_cast<float>(tmType);
    constants.ShoulderStrength = shoulderStrengthSlider.Value();
    constants.LinearStrength = linearStrengthSlider.Value();
    constants.LinearAngle = linearAngleSlider.Value();
    constants.ToeStrength = toeStrengthSlider.Value();
    constants.ToeNumerator = toeNumeratorSlider.Value();
    constants.ToeDenominator = toeDenominatorSlider.Value();
    constants.LinearWhite = linearWhiteSlider.Value();
    constants.LuminanceSaturation = luminanceSaturationSlider.Value();
    constants.LumMapMipLevel = lumMapMipSlider.Value();
    constants.Bias = biasSlider.Value();
    
    postProcessor.SetConstants(constants);
    postProcessor.Render(context, renderTarget.SRView, deviceManager.BackBuffer());
    D3DPERF_EndEvent();

    renderTargets[0] = deviceManager.BackBuffer();
    context->OMSetRenderTargets(1, renderTargets, NULL);

    D3D11_VIEWPORT vp;
    vp.Width = static_cast<float>(deviceManager.BackBufferWidth());
    vp.Height = static_cast<float>(deviceManager.BackBufferHeight());
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    vp.MinDepth = 0;
    vp.MaxDepth = 1;
    context->RSSetViewports(1, &vp);

    RenderHUD();
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

    meshPSConstants.Data.DiffuseAlbedo = XMFLOAT4(diffuseRSlider.Value(), 
                                                    diffuseGSlider.Value(), 
                                                    diffuseBSlider.Value(), 1);
    meshPSConstants.Data.SpecularAlbedo = XMFLOAT4(specularRSlider.Value(), 
                                                    specularGSlider.Value(), 
                                                    specularBSlider.Value(), 1);

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

void ToneMapping::RenderHUD()
{
    PIXEvent event(L"HUD Pass");

    spriteRenderer.Begin(deviceManager.ImmediateContext(), SpriteRenderer::Point);

    toneGraph.Render(postProcessor.GetConstants(), spriteRenderer, XMFLOAT2(50.0f, 25.0f));

    XMMATRIX transform = XMMatrixScaling(0.2f, 0.2f, 1.0f);
    transform *= XMMatrixTranslation(50.0f, 225.0f, 0.0f);
    spriteRenderer.Render(postProcessor.GetExposureMap(), transform);    

    // Mesh name
    transform = XMMatrixTranslation(25.0f, deviceManager.BackBufferHeight() - 100.0f, 0);
    wstring meshText(L"Mesh(Press M to change): " + MeshFileNames[currMesh]);
    spriteRenderer.RenderText(font, meshText.c_str(), transform, XMFLOAT4(1, 1, 0, 1));

    // Env map name
    transform._42 += 25.0f;
    wstring envMapText(L"Environment Map(Press P to change): " + EnvMapFileNames[currEnvMap]);
    spriteRenderer.RenderText(font, envMapText.c_str(), transform, XMFLOAT4(1, 1, 0, 1));

    // Tone mapping
    transform._42 += 25.0f;
    wstring toneMapText(L"Tone Mapping Curve(Press L to change): ");
    toneMapText += ToneMapOperators[tmType];
    spriteRenderer.RenderText(font, toneMapText.c_str(), transform, XMFLOAT4(1, 1, 0 ,1));

    // Auto-exposure
    transform._42 += 25.0f;
    wstring autoExposureText(L"Auto Exposure(Press K to change): ");
    autoExposureText += AutoExposureTypes[autoExposureType];
    spriteRenderer.RenderText(font, autoExposureText.c_str(), transform, XMFLOAT4(1, 1, 0 ,1));

    for (UINT_PTR i = 0; i < guiObjects.size(); ++i)
        guiObjects[i]->Render(spriteRenderer);

    spriteRenderer.End();
}

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{	    			                                     
	ToneMapping app;
    app.Run();
}


