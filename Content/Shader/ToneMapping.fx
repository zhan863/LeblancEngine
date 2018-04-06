float4 tone_mapping_color;

struct VS_INPUT
{
    float4 position:POSITION;
    float4 texcoord:TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 hPosition : SV_POSITION;
	float2 texcoord : TEXCOORD0;
};

VS_OUTPUT toneMappingVS(VS_INPUT input)
{
	VS_OUTPUT OUT;

	OUT.hPosition =  float4(input.position.xyz, 1.0);
	OUT.texcoord.xy = input.texcoord.xy;

	return OUT;
}

float4 toneMappingPS(VS_OUTPUT IN) : SV_TARGET0
{
	return tone_mapping_color;
}

technique11 toneMapping
{
	pass P0
	{
		SetVertexShader(CompileShader( vs_5_0, toneMappingVS()));
		SetGeometryShader(NULL);
		SetPixelShader (CompileShader( ps_5_0, toneMappingPS()));
	}
}