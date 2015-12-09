// Texture pixel/fragment shader
// Basic fragment shader for rendering textured geometry


Texture2D texture0 : register(t0);
Texture2D transparent : register(t1);
SamplerState Sampler0 : register(s0);

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};


float4 main(InputType input) : SV_TARGET
{
	float4 finalColour;
    float4 textureColour;

    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
	textureColour = texture0.Sample( Sampler0, input.tex );
	finalColour = textureColour;

	// blend with the transparent texture
	float4 transColour = transparent.Sample( Sampler0, input.tex );

	if( transColour.w < textureColour.w )
	{
		finalColour += transColour;
	}
	
	return finalColour;
}