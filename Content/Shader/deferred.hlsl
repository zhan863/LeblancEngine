float4x4 world_matrix;
float4x4 view_proj_matrix;

struct VSIn
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 texture_coord : TEXCOORD;
};

struct VSOut
{
	float4 proj_position : SV_POSITION;
	//float2 texture_coord : TEXCOORD1;
};

VSOut VS(VSIn input)
{
	VSOut result;

	result.proj_position = mul(world_matrix, input.position);
	result.proj_position = mul(view_proj_matrix, result.proj_position);

	//result.texture_coord = input.texture_coord;

	return result;
}

float4 PS(/*in float2 texcoord : TEXCOORD1*/) : SV_TARGET0
{
	return float4(1, 1, 1, 1);
}