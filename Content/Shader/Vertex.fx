#include "GlobalConstant.fx"

// the basic structure of vs input and output.
struct VsInputStaticMesh
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 texcoord : TEXCOORD0;
};

struct VsOutputStaticMesh
{
	float4 position : POSITION;
	float3 normal : NORMAL;
	float2 texcoord : TEXCOORD0;
};

// all the 
VsOutputStaticMesh vsStaticMeshPNT(VsOutputStaticMesh input)
{
	VsOutputStaticMesh output = (VsOutputStaticMesh)0;

	output.position = mul(per_object_world_matrix, float4(input.position, 1));
	output.position = mul(per_frame_view_proj_matrix, float4(output.position, 1));
	output.normal = mul(per_object_world_matrix, input.normal);
	output.texcoord = input.texcoord;

	return output;
}