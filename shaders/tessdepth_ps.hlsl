// Tessellation pixel shader
// Output colour passed to stage.

struct InputType
{
	float4 position : SV_POSITION;
	float4 depthPosition : TEXCOORD0;
	float4 colour : COLOR;
};

float4 main(InputType input) : SV_TARGET
{
	// Here the colour of the fragment is written directly to the depth texture,
	// and the depth (alpha) is set to one. This could potentially cause problems as it assumes the
	// projected fragment will receive lighting.
	// To fix this the shadow map could be passed to the pixel shader, and the depth could be sampled and transferred
	// or, as I have done here, the blend operation for this pass could be set to preserve the depth (alpha) of the target fragment
	return float4(input.colour.xyz, 0.0f);
}