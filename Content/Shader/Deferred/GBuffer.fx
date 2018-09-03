#include "Vertex/Vertex.fxh"

struct PS_INPUT
{
	float2 texcoord : TEXCOORD0;
};

float4 GBufferPS(PS_INPUT input) : SV_TARGET0
{
	return float4(1, 0, 0, 1);
}

technique11 StaticMeshGBuffer
{
	pass P0
	{
		SetVertexShader (CompileShader( vs_5_0, VsStaticMesh()));
		SetPixelShader (CompileShader( ps_5_0, GBufferPS()));
	}
}