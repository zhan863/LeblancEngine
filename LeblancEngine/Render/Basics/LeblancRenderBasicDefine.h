#ifndef LEBLANC_RENDER_BASIC_DEFINE_H
#define LEBLANC_RENDER_BASIC_DEFINE_H

enum class Pass : UINT
{
	GBuffer,
	DeferredShading,
	SingleForward,
	Transparent,
	DepthOnly
};

#endif