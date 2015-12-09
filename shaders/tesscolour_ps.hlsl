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
	// modify the colour based on the positon
	float4 finalColour;
	float3 lightDir;
	float lightIntensity;
	float depthValue;

	finalColour = input.colour;
	
	lightDir = normalize( float3(1.0f, 1.0f, 0.0f) );

	depthValue = input.depthPosition.z / input.depthPosition.w;
	
	// Do lighting
	lightIntensity = saturate( dot( lightDir, input.normal.xyz ) );
	finalColour *= lightIntensity;

	finalColour.w = depthValue;

	return finalColour;
}