// Texture pixel/fragment shader
// Basic fragment shader for rendering textured geometry


Texture2D scene : register(t0);
Texture2D transparent : register(t1);
Texture2D distortion : register(t2);

SamplerState Sampler0 : register(s0);

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

float4 main(InputType input) : SV_TARGET
{
	float4 distortionSample;
	float4 transparentSample;
	float4 sceneSample;
	float2 distortedTex;

	// Get the pixel from othe distortion texture
	distortionSample = distortion.Sample( Sampler0, input.tex );
	// if it's empty, return the normal scene
	if( distortionSample.w == 0 )
	{
		return scene.Sample( Sampler0, input.tex );
	}

	// Get the pixel from the transparent texture
	transparentSample = transparent.Sample( Sampler0, input.tex );
	// Get the pixel from the scene
	sceneSample = scene.Sample( Sampler0, input.tex );
	// do the depth test, if the scene is infront return it
	if( transparentSample.w > sceneSample.w )
	{
		return sceneSample;
	}

	// If we are here, distort the pixel
	float4 distortedPixel = scene.Sample( Sampler0, input.tex - distortionSample.xy * 0.1 );

	// blend with the transparent pixel
	distortedPixel += transparentSample;

	return distortedPixel;	
}