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

	// Distance to the center of the screen
	float centerDist = scene.Sample( Sampler, float2(0.5f, 0.5f) ).w;

	sceneSample = scene.Sample( Sampler, input.tex );
	blurSample = blur.Sample( Sampler, input.tex );

	float depthDifference = abs( centerDist - sceneSample.w  );
	//if( sceneSample.w == 1.0f ) depthDifference = 0.0f;

	return lerp(sceneSample, blurSample, depthDifference);
}