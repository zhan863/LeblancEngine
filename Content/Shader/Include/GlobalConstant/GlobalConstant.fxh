cbuffer PerViewBuffer : register(b0)
{
	float4x4 per_view_world_view_matrix;
	float4x4 per_view_view_proj_matrix;
	float4x4 per_view_proj_matrix;
}

cbuffer PerObjectBuffer : register(b1)
{
	float4x4 per_object_world_matrix;
}

