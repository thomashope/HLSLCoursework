// Tessellation pixel shader
// Output colour passed to stage.

Texture2D depthMapTexture : register(t0);

SamplerState SampleTypeClamp : register(s0);

struct InputType
{
	float4 position : SV_POSITION;
	float4 depthPosition : TEXCOORD0;
	float4 normal : NORMAL;
    float4 colour : COLOR;
};

float4 main(InputType input) : SV_TARGET
{	
	// calculate the depth of the pixel
	float depthValue = input.depthPosition.z / input.depthPosition.w;

	// Normalised screen coords
	float2 screenPos;
	//screenPos.x = (input.position.x / input.position.w) * 0.5f + 0.5f;
	//screenPos.y = (input.position.y / input.position.w) * 0.5f + 0.5f;
	screenPos.x = (input.position.x + 1.0f) * 0.5f;
	screenPos.y = (input.position.y + 1.0f) * -0.5f;

	// Get the depth already stored at this pixel in the depth map
	//float depthSample = depthMapTexture.Sample( SampleTypeClamp, screenPos ).w;
	float4 finalColour = float4(input.colour.xyz, 0.0f);
	//float4 finalColour = float4(depthSample, depthSample, depthSample, depthSample);
	
	return finalColour;
}