Texture2D texture0 : register(t0);
SamplerState Sampler0 : register(s0);

struct InputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal	: NORMAL;
	float3 binormal : BINORMAL;
	float3 tangent	: TANGENT;
};

float4 main(InputType input) : SV_TARGET
{
	//return texture0.Sample( Sampler0, input.tex );
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}