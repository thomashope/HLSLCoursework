Texture2D diffuseTex : register(t0);
Texture2D normalTex : register(t1);
Texture2D specularTex : register(t2);

SamplerState Sampler : register(s0);

struct InputType
{
	float4 position : SV_POSITION;
	//float2 tex		: TEXCOORD0;
	//float3 normal	: NORMAL;
	//float3 binormal : BINORMAL;
	//float3 tangent	: TANGENT;
};

float4 main(InputType input) : SV_TARGET
{
	float4 finalColour;
	float4 diffuseSamp;
	float4 normalSamp;
	float3 normal;
	float4 specularSamp;
	float lightIntensity;

	float3 lightDir = { 0.0f, 1.0f, 0.0f };

		// Sample from the textures
		//diffuseSamp = diffuseTex.Sample( Sampler, input.tex );
		//normalSamp = normalTex.Sample( Sampler, input.tex );
		//specularSamp = specularTex.Sample( Sampler, input.tex );

		// adjust the normal from [0,1] to [-1,1], then move to tangent space
		//normalSamp = (normalSamp * 2.0f) - 1.0f;
		//normal = (normalSamp.x * input.tangent) + (normalSamp.y * input.binormal) + (normalSamp.z * input.normal);
		//normal = normalize( normal );

		//lightIntensity = dot( lightDir.xyz, input.normal );

		//return diffuseSamp * lightIntensity;
		return float4(1.0f, 1.0f, 1.0f, 1.0f);
}