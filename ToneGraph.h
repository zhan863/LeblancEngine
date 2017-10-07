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
#include "SampleFramework11/SpriteRenderer.h"
#include "SampleFramework11/SpriteFont.h"

#include "TMPostProcessor.h"

using namespace SampleFramework11;

class ToneGraph
{

public:

    ToneGraph();
    ~ToneGraph();

    void Initialize(ID3D11Device* device);

    void SetConstants(const TMPostProcessor::Constants& constants);
    void Render(const TMPostProcessor::Constants& constants, SpriteRenderer& spriteRenderer, const XMFLOAT2& pos);

    SpriteFont& Font() { return font; };

protected:

    ID3D11ShaderResourceViewPtr texture;
    SpriteFont font;
};