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

#include "ToneGraph.h"

ToneGraph::ToneGraph()
{
}

ToneGraph::~ToneGraph()
{
}

void ToneGraph::Initialize(ID3D11Device* device)
{
    font.Initialize(L"Microsoft Sans Serif", 8.5f, SpriteFont::Regular, true, device);

    // Create a 1x1 white texture    
    DXCall(D3DX11CreateShaderResourceViewFromFileW(device, L"Content\\White.png", NULL, NULL, &texture, NULL));    
}

static float ToneMapReinhard(float x, const TMPostProcessor::Constants& constants) {    
    return x / (x + 1);    
}

static float ToneMapReinhardModified(float x, const TMPostProcessor::Constants& constants) {    
    return x * (1.0f + x / (constants.WhiteLevel * constants.WhiteLevel)) / (1.0f + x);    
}

static float ToneMapFilmicALU(float x, const TMPostProcessor::Constants& constants)
{
    x = max(0, x - 0.004f);
    x = (x * (6.2f * x + 0.5f)) / (x * (6.2f * x + 1.7f)+ 0.06f);

    // result has 1/2.2 baked in
    return std::powf(x, 2.2f);
}

static float ToneMapLogarithmic(float x, const TMPostProcessor::Constants& constants)
{
    return std::log10f(1 + x) / std::log10f(1 + constants.WhiteLevel);
}

static float ToneMapExponential(float x, const TMPostProcessor::Constants& constants)
{
    return 1 - std::expf(-x / constants.WhiteLevel);
}

static float ToneMapDragoLogarithmic(float x, const TMPostProcessor::Constants& constants)
{
    float y = std::log10f(1 + x);
    y /= std::log10f(1 + constants.WhiteLevel);
    y /= std::log10f(2 + 8 * ((x / constants.WhiteLevel) * std::log10f(constants.Bias) / std::log10f(0.5f)));
    return y;
}

static float U2Func(float luminance, const TMPostProcessor::Constants& constants)
{
    float A = constants.ShoulderStrength;
    float B = constants.LinearStrength;
    float C = constants.LinearAngle;
    float D = constants.ToeStrength;
    float E = constants.ToeNumerator;
    float F = constants.ToeDenominator;
    float x = luminance;
    return ((x*(A*x+C*B)+D*E)/(x*(A*x+B)+D*F)) - E/F;
}

static float ToneMapFilmicU2(float luminance, const TMPostProcessor::Constants& constants)
{    
    float numerator = U2Func(luminance, constants);    

    float denominator = U2Func(constants.LinearWhite, constants);

    return numerator / denominator;
}

static float ToneMap(float x, const TMPostProcessor::Constants& constants)
{
    if (constants.ToneMapTechnique == 0)
        return x;
    else if (constants.ToneMapTechnique == 1)
        return ToneMapLogarithmic(x, constants);
    else if (constants.ToneMapTechnique == 2)
        return ToneMapExponential(x, constants);
    else if (constants.ToneMapTechnique == 3)
        return ToneMapDragoLogarithmic(x, constants);
    else if (constants.ToneMapTechnique == 4)
        return ToneMapReinhard(x, constants);
    else if (constants.ToneMapTechnique == 5)
        return ToneMapReinhardModified(x, constants);
    else if (constants.ToneMapTechnique == 6)
        return ToneMapFilmicALU(x, constants);
    else
        return ToneMapFilmicU2(x, constants);
}

void ToneGraph::Render(const TMPostProcessor::Constants& constants, SpriteRenderer& spriteRenderer, const XMFLOAT2& pos)
{
    PIXEvent pixEvent(L"Tone Graph");
    
    const UINT_PTR NumSamplePoints = 256;
    const float Width = static_cast<float>(NumSamplePoints);
    const float Height = 128.0f;
    const float MinLogX = -5.0f;
    const float MaxLogX = 5.0f;
    const float MaxY = 1.0f;

    // Get the tone map curve
    float samplePoints[NumSamplePoints];
    for (UINT_PTR i = 0; i < NumSamplePoints; ++i)
    {
        float t = i / Width;
        float logX = MinLogX + ((MaxLogX - MinLogX) * t);
        float x = std::powf(10.0f, logX);
        samplePoints[i] = ToneMap(x, constants) / MaxY;
    }

    // Render all sample points
    SpriteRenderer::SpriteDrawData drawData[NumSamplePoints - 1];
    for (UINT_PTR i = 0; i < NumSamplePoints - 1; ++i)
    {
        XMFLOAT2 p1 = XMFLOAT2(i + pos.x, pos.y + Height - (samplePoints[i] * Height));
        XMFLOAT2 p2 = XMFLOAT2(i + 1 + pos.x, pos.y + Height - (samplePoints[i + 1] * Height));
        
        XMVECTOR right = XMLoadFloat4(&XMFLOAT4(p2.x - p1.x, p2.y - p1.y, 0, 0));
        float length;
        XMStoreFloat(&length, XMVector3Length(right));
        right = XMVector3Normalize(right);

        XMVECTOR forward = XMLoadFloat4(&XMFLOAT4(0, 0, 1.0f, 0));
        XMVECTOR up = XMVector3Normalize(XMVector3Cross(right, forward));         

        XMMATRIX lineTransform = XMMatrixIdentity();
        XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&lineTransform._11), right);
        XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&lineTransform._21), up);
        XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&lineTransform._31), forward);
        lineTransform._41 = p1.x;
        lineTransform._42 = p1.y;
        lineTransform._43 = 0;
        lineTransform._44 = 1.0f;

        lineTransform = XMMatrixScaling(length, 1, 1) * lineTransform;

        drawData[i].Transform = lineTransform;
        drawData[i].DrawRect = XMFLOAT4(0, 0, 1, 1);
        drawData[i].Color = XMFLOAT4(1, 0, 0, 1);        
    }

    spriteRenderer.RenderBatch(texture, drawData, NumSamplePoints - 1);       

    // Vertical line
    XMMATRIX transform = XMMatrixScaling(1, Height, 1);
    transform *= XMMatrixTranslation(pos.x, pos.y, 0);
    spriteRenderer.Render(texture, transform, XMFLOAT4(1, 1, 1, 1));

    // Horizontal line
    transform = XMMatrixScaling(Width, 1, 1);
    transform *= XMMatrixTranslation(pos.x, pos.y + Height, 0);
    spriteRenderer.Render(texture, transform, XMFLOAT4(1, 1, 1, 1));

    // Text
    transform = XMMatrixTranslation(pos.x + 90.0f, pos.y + Height + 25.0f, 0);
    spriteRenderer.RenderText(font, L"Log Luminance/RGB", transform, XMFLOAT4(1, 1, 1, 1));

    transform = XMMatrixRotationZ(-3.14159f / 2);
    transform *= XMMatrixTranslation(pos.x - 35, pos.y + Height - 20, 0);
    spriteRenderer.RenderText(font, L"Output Luminance/RGB", transform);
    
    for (UINT i = 0; i < 11; ++i)
    {
        transform = XMMatrixTranslation(pos.x + i * 24.f, pos.y + Height + 5.0f, 0);
        float val = MinLogX + i;
        std::wstring numstring = ToString<float>(val);
        spriteRenderer.RenderText(font, numstring.c_str(), transform);
    }

    for (UINT i = 0; i < 6; ++i)
    {
        transform = XMMatrixTranslation(pos.x - 15, pos.y + Height - 10.0f - i * 25.0f, 0);
        float val = i * 0.2f;
        std::wstring numstring = ToString<float>(val);
        spriteRenderer.RenderText(font, numstring.c_str(), transform);
    }
}