// Light pixel shader
// Calculate ambient and diffuse lighting for a single light (also texturing)

Texture2D shaderTexture : register(t0);
Texture2D specularTexture : register(t1);
Texture2D normalTexture : register(t2);

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
	float3 normal : NORMAL;				// Multiplied with world matrix
	float3 tangent : TANGENT;			// Multiplied with world matrix
	float3 binormal : BINORMAL;			// Multiplied with world matrix
	float3 viewDirection : TEXCOORD1;
};

//TODO figure out why there are stange lines when using bumpNormal, possibly due to incorrectly sending data when creating shader?
// http://www.rastertek.com/dx11tut20.html

float4 main(InputType input) : SV_TARGET
{
	float4 finalColour = { 0, 0, 0, 0 };
	float4 finalSpec = { 0, 0, 0, 0 };
    float4 textureSample;
	float4 specularIntensity;
	float4 bumpSample;
	float3 bumpNormal;
	float4 specular;
	float3 lightDir;
	float3 reflection;
    float lightIntensity;

	// set the default output finalColour to the ambient light value
	finalColour = ambientColour;

    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
    textureSample = shaderTexture.Sample(SampleType, input.tex);

	// Sample the pixel in the bump map.
	bumpSample = normalTexture.Sample(SampleType, input.tex);
	// Expand the range of the normal value from (0, +1) to (-1, +1).
	bumpSample = (bumpSample * 2.0f) - 1.0f;
	// Calculate the normal from the data in the bump map.
	bumpNormal = (bumpSample.x * input.tangent) + (bumpSample.y * input.binormal) + (bumpSample.z * input.normal);
	// Normalize the resulting bump normal.
	bumpNormal = -normalize(bumpNormal);
	
	lightDir = lightDirection;

    // Calculate the amount of light on this pixel.
	lightIntensity = saturate(dot(bumpNormal, lightDir));
	//lightIntensity = saturate(dot(input.normal, lightDir));

	if( lightIntensity > 0.0f )
	{
		// Determine the final amount of diffuse color based on the diffuse color combined with the light intensity.
		finalColour += diffuseColour * lightIntensity;
		finalColour = saturate( finalColour );

		// Sample the specular texture
		specularIntensity = specularTexture.Sample(SampleType, input.tex);

		// calculate reflection vector based on the light intensity, normal vector and light direction
		//reflection = reflect( -lightDir, input.normal );
		reflection = reflect(-lightDir, bumpNormal);

		// determine the amount of specular light based on the reflection vector, viewing direcion and specular power
		// TODO: find out why / if its correct to have sharp edges on the specular reflection at extreme angles
		specular = pow( saturate( dot( reflection, input.viewDirection) + 0.5f), specularPower );

		// sum up specular light and adjust by specular colour
		finalSpec = specularColour * specular * specularIntensity;
		//finalSpec = specularColour * specular;
	}
    
	// Multiply the texture pixel and the final diffuse color to get the final pixel color result.
    finalColour = finalColour * textureSample;

	// Apply specular highlight
	finalColour = saturate( finalColour + finalSpec );

	return finalColour;
	//return float4(bumpNormal, 1.0f);
}

