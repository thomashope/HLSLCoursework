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
	float4 finalColour = input.colour;
	float3 lightDir = normalize(float3(1.0f, 1.0f, 0.0f));
	float lightIntensity;
	
	// Do lighting
	//lightIntensity = saturate( dot( lightDir, input.normal.xyz ) );
	//finalColour *= lightIntensity;
	
	// calculate the depth of the pixel
	float depthValue = input.depthPosition.z / input.depthPosition.w;
	finalColour = float4( input.colour.xyz, 1.0f);


	return finalColour;
}