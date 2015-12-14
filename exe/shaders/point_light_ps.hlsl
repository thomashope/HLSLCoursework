// Light pixel shader
// Calculate ambient and diffuse lighting for a single light (also texturing)

Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

#define NUM_LIGHTS 4

cbuffer LightBuffer : register(cb0)
{
    float4 diffuseColour[NUM_LIGHTS];    
	float4 lightPosition[NUM_LIGHTS];
	float4 specularColour[NUM_LIGHTS];
	float4 ambientColour;

	// attenuation x, y, z, w == range, constant, linear, quadratic
	float4 attenuation[NUM_LIGHTS];
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
	float4 finalColour			// Final colour of the fragment: initialise as the ambient light value
		= ambientColour;		
	float4 totalSpecular		// Resulat of all specular highlights, then combined with final colour
		= { 0, 0, 0, 0 };	
	float4 textureColour		// Colour of the texture at this fragments texture coordinate
		= shaderTexture.Sample( SampleType, input.tex );
	float3 lightDirection;		// Vector from the fragment to the light
	float3 lightReflection;		// The result of reflection from the light to the fragment at the fragments normal
    float diffuseIntensity;		// Brightness of diffuse component of light
	float specularIntensity;	// Brightness of specular component of light
	float attenuationFactor;	// Result of the atenuation equation, affects how lights fade over distance
	float distance;				// Distance from the fragment to the light

	for (int i = 0; i < NUM_LIGHTS; i++)
	{
		distance = length( lightPosition[i].xyz - input.position3D );

		////* IGNORE LIGHTS NOT IN RANGE *////
		if( distance < attenuation[i].x )
		{
			// points from the light to the fragment
			lightDirection = normalize( input.position3D - lightPosition[i].xyz );

			// Calculate the amount of light on this pixel.
			diffuseIntensity = saturate( dot( input.normal, -lightDirection ) );

			// if the surface is facing away from the light, light intensity will be negative
			if( diffuseIntensity > 0.0f )
			{
				////* CALCULATE ATTENUATION *////
				attenuationFactor =	1 / (attenuation[i].y +
									attenuation[i].z * distance +
									attenuation[i].w * distance * distance);

				// Determine the final amount of diffuse color based on the diffuse color combined with the light intensity and attenuation.
				finalColour += diffuseColour[i] * diffuseIntensity * attenuationFactor;
				finalColour = saturate( finalColour );

				////* CALCULATE SPECULAR REFLECTION *////
				// Reflect the ray of light at the fragment according to it's normal
				lightReflection = reflect( lightDirection, input.normal );

				// determine the amount of specular light based on the lightReflection vector, viewing direcion and specular power
				//TODO: find out why / if its correct to have sharp edges on the specular lightReflection at extreme angles
				specularIntensity = pow( saturate( dot( lightReflection, input.viewDirection ) ), specularPower[i].w );

				// attenuate the specular highlight and add it to the other highlights
				totalSpecular += specularColour[i] * specularIntensity * attenuationFactor;
			}
		}
	}

	// Multiply the texture pixel and the final diffuse color to get the final pixel color result.
    finalColour = finalColour * textureColour;

	// blend with the specular finalColour
	finalColour = saturate( finalColour + totalSpecular );

	return finalColour;
}

