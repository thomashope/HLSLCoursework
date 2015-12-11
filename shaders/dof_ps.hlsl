// Texture pixel/fragment shader
// Basic fragment shader for rendering textured geometry

Texture2D scene : register(t0);
Texture2D blur : register(t1);

SamplerState Sampler : register(s0);

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

float4 main(InputType input) : SV_TARGET
{
	float4 sceneSample;
	float4 blurSample;
	float4 fog;

	// Distance to the center of the screen
	float centerDist = scene.Sample( Sampler, float2(0.5f, 0.5f) ).w;

	sceneSample = scene.Sample( Sampler, input.tex );
	blurSample = blur.Sample( Sampler, input.tex );

	float depthDifference = abs( centerDist - sceneSample.w  ) * 0.9f;

	fog = (float4(0.6f, 0.5f, 0.6f, 1.0f) * pow(sceneSample.w, 60) );

	return saturate(lerp(sceneSample, blurSample, depthDifference) - fog);
}