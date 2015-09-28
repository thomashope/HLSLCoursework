// Light pixel shader
// Calculate ambient and diffuse lighting for a single light (also texturing)

Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

#define NUM_LIGHTS 2

cbuffer LightBuffer : register(cb0)
{
	float4 ambientColour;
    float4 diffuseColour[NUM_LIGHTS];    
	float4 lightPosition[NUM_LIGHTS];
    
	float4 specularColour[2];

	float specularPower[2];
	float padding[2];
};

struct InputType
{
    float4 position : SV_POSITION;

    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
	float3 viewDirection : TEXCOORD1;

	float3 position3D : TEXCOORD2;
};

float4 main(InputType input) : SV_TARGET
{
    float4 textureColour;
	float3 lightDir;
    float diffuseIntensity;
	float4 colour = { 0, 0, 0, 0 };
	float3 reflection;
	float4 specular;
	float4 finalSpec = { 0, 0, 0, 0 };

		// used for attenuation, constant for now
		float range = 25.0f;
		float constFactor = 0.5f;
		float linearFactor = 0.125f;
		float quadFactor = 0.0f;
		float attenuation = 1.0f;

		float distance;

    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
    textureColour = shaderTexture.Sample(SampleType, input.tex);

	// set the default output colour to the ambient light value
	colour = ambientColour;

	for (int i = 0; i < NUM_LIGHTS; i++)
	{
		distance = length( input.position3D - lightPosition[i].xyz );
		
		// skip lights that are not in range
		if (distance < range)
		{
			// points from the light to the fragment
			lightDir = normalize( input.position3D - lightPosition[i].xyz );

			// Calculate the amount of light on this pixel.
			diffuseIntensity = saturate( dot( input.normal, -lightDir ) );

			// if the surface is facing away from the light, light intensity will be negative
			if (diffuseIntensity > 0.0f)
			{
				// calculate attenuation value
				attenuation = 1 / (constFactor + linearFactor * distance + quadFactor * distance * distance);

				// Determine the final amount of diffuse color based on the diffuse color combined with the light intensity.
				colour += diffuseColour[i] * diffuseIntensity * attenuation;
				colour = saturate( colour );

				/* USED FOR SPECULAR REFLECTION */
				// calculate reflection vector based on the light intensity, normal vector and light direction
				//reflection = reflect( lightDir, input.normal );

				// determine the amount of specular light based on the reflection vector, viewing direcion and specular power
				//TODO: find out why / if its correct to have sharp edges on the specular reflection at extreme angles
				//specular = pow( saturate( dot( reflection, input.viewDirection) ), specularPower[i] );

				//float4 temp = { 1.0f, 1.0f, 1.0f, 1.0f };

				// sum up specular light
				//finalSpec = specularColour[0] * specular;

				//finalSpce = temp;
				//finalSpec = saturate( finalSpec + temp );
			}
		}
	}

	// Multiply the texture pixel and the final diffuse color to get the final pixel color result.
    colour = colour * textureColour;

	// blend with the specular colour
	//colour = saturate( colour + finalSpec );

	return colour;
}

