// Light pixel shader
// Calculate ambient and diffuse lighting for a single light (also texturing)

Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

#define NUM_LIGHTS 2

cbuffer LightBuffer : register(cb0)
{
    float4 diffuseColour[NUM_LIGHTS];    
	float4 lightPosition[NUM_LIGHTS];
	float4 specularColour[NUM_LIGHTS];
	float4 ambientColour;

	// attenuation x, y, z, w == range, constant, linear, quadratic
	float4 attenuation[NUM_LIGHTS];
	//float4 range[NUM_LIGHTS];
	//float4 constantAttenuation[NUM_LIGHTS];
	//float4 linearAttenuation[NUM_LIGHTS];
	//float4 quadraticAttenuation[NUM_LIGHTS];

	float4 specularPower[NUM_LIGHTS];
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
	float4 colour = { 0, 0, 0, 0 }; // should be renamed final colour?
	float3 reflection;
	float4 specular;
	float4 finalSpec = { 0, 0, 0, 0 };

	float attenuationIntensity;
	float distance;

    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
    textureColour = shaderTexture.Sample(SampleType, input.tex);

	// set the default output colour to the ambient light value
	colour = ambientColour;

	for (int i = 0; i < NUM_LIGHTS; i++)
	{
		distance = length( lightPosition[i].xyz - input.position3D );

		// skip lights that are not in range
		if (distance < attenuation[i].x)
		{
			// points from the light to the fragment
			lightDir = normalize( input.position3D - lightPosition[i].xyz );

			// Calculate the amount of light on this pixel.
			diffuseIntensity = saturate( dot( input.normal, -lightDir ) );

			// if the surface is facing away from the light, light intensity will be negative
			if (diffuseIntensity > 0.0f)
			{
				// calculate attenuation value
				attenuationIntensity =	1 / (attenuation[i].y +
										attenuation[i].z * distance +
										attenuation[i].w * distance * distance);

				// Determine the final amount of diffuse color based on the diffuse color combined with the light intensity.
				colour += diffuseColour[i] * diffuseIntensity * attenuationIntensity;
				colour = saturate( colour );

				/* USED FOR SPECULAR REFLECTION */
				// calculate reflection vector based on the light intensity, normal vector and light direction
				reflection = reflect( lightDir, input.normal );

				// determine the amount of specular light based on the reflection vector, viewing direcion and specular power
				//TODO: find out why / if its correct to have sharp edges on the specular reflection at extreme angles
				specular = pow( saturate( dot( reflection, input.viewDirection) ), specularPower[i].w );

				// sum up specular light
				finalSpec += specularColour[i] * specular * attenuationIntensity;
			}
		}
	}

	// Multiply the texture pixel and the final diffuse color to get the final pixel color result.
    colour = colour * textureColour;

	// blend with the specular colour
	colour = saturate( colour + finalSpec );

	return colour;
}

