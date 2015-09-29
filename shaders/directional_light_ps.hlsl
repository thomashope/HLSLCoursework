// Light pixel shader
// Calculate ambient and diffuse lighting for a single light (also texturing)

Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

cbuffer LightBuffer : register(cb0)
{
    float4 diffuseColour;
	float4 ambientColour;
    float3 lightDirection;
    float specularPower;
	float4 specularColour;
};

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
	float3 viewDirection : TEXCOORD1;
};

float4 main(InputType input) : SV_TARGET
{
	float4 finalColour = { 0, 0, 0, 0 };
	float4 finalSpec = { 0, 0, 0, 0 };
    float4 textureColour;
	float4 specular;
	float3 lightDir;
	float3 reflection;
    float lightIntensity;

    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
    textureColour = shaderTexture.Sample(SampleType, input.tex);

	// set the default output finalColour to the ambient light value
	finalColour = ambientColour;

	lightDir = -lightDirection;

    // Calculate the amount of light on this pixel.
    lightIntensity = saturate(dot(input.normal, lightDir));

	if( lightIntensity > 0.0f )
	{
		// Determine the final amount of diffuse color based on the diffuse color combined with the light intensity.
		finalColour += diffuseColour * lightIntensity;
		finalColour = saturate( finalColour );

		// calculate reflection vector based on the light intensity, normal vector and light direction
		reflection = reflect( -lightDir, input.normal );

		// determine the amount of specular light based on the reflection vector, viewing direcion and specular power
		// TODO: find out why / if its correct to have sharp edges on the specular reflection at extreme angles
		specular = pow( saturate( dot( reflection, input.viewDirection) + 0.5f), specularPower );

		// sum up specular light
		finalSpec = specularColour * specular;
	}
    
	// Multiply the texture pixel and the final diffuse color to get the final pixel color result.
    finalColour = finalColour * textureColour;

	finalColour = saturate( finalColour + finalSpec );

	return finalColour;
}

