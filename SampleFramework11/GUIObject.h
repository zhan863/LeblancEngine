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

namespace SampleFramework11
{

class GUIObject
{
   
public:

    virtual void Update(UINT mousePosX, UINT mousePosY, bool mouseLButtonDown) = 0;
    virtual void Render(SpriteRenderer& renderer) = 0;

    XMFLOAT2& Position() { return position; };
    XMFLOAT2 Position() const { return position; };

    bool& Enabled() { return enabled; };
    bool Enabled() const { return enabled; };

protected:

    XMFLOAT2 position;
    bool enabled;

};

}