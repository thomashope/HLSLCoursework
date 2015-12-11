// Tessellation pixel shader
// Output colour passed to stage.

struct InputType
{
	float4 position : SV_POSITION;
	float4 depthPosition : TEXCOORD0;
	float4 normal : NORMAL;
    float4 colour : COLOR;
};

float4 main(InputType input) : SV_TARGET
{
	float4 finalColour;
	float depthValue;

	// the colour is modified based on position in the domain shader
	finalColour = input.colour;

	// write the depth value to the alpha channel for colour blending
	depthValue = input.depthPosition.z / input.depthPosition.w;
	finalColour.w = depthValue;

	return finalColour;
}