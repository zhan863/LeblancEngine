#include "Include/GlobalConstant/GlobalConstant.fxh"

// the basic structure of vs input and output.
struct VsInputStaticMesh
{
	float3 position : POSITION;
	float3 normal 	: NORMAL;
	float2 texcoord : TEXCOORD0;
};

struct VsOutput
{
	float2 texcoord : TEXCOORD0;
	float3 normal 	: NORMAL;
	float4 position : SV_POSITION;
};

// all the 
VsOutput VsStaticMesh(VsInputStaticMesh input)
{
	VsOutput output = (VsOutput)0;

	output.position = mul(per_object_world_matrix, float4(input.position, 1));
	output.position = mul(per_view_view_proj_matrix, float4(output.position.xyz, 1));
	output.normal 	= mul(per_object_world_matrix, float4(input.normal, 0)).xyz;
	output.texcoord = input.texcoord;

	return output;
}