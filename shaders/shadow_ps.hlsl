
Texture2D shaderTexture : register(t0);
Texture2D depthMapTexture1 : register(t1);
Texture2D depthMapTexture2 : register(t2);

SamplerState SampleTypeWrap  : register(s0);
SamplerState SampleTypeClamp : register(s1);

#define NUM_LIGHTS 2

cbuffer LightBuffer : register(cb0)
{
	float4 ambientColor		[NUM_LIGHTS];
	float4 lightPosition	[NUM_LIGHTS];
	float4 diffuseColor		[NUM_LIGHTS];
	// attenuation x, y, z, w == range, constant, linear, quadratic
	float4 attenuation		[NUM_LIGHTS];
};

struct InputType
{
	float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
	float3 normal : NORMAL;

	float3 position3D			: TEXCOORD1;
	float4 depthPosition		: TEXCOORD2;

	float4 lightViewPosition	[NUM_LIGHTS] : TEXCOORD3;
	float3 lightPos				[NUM_LIGHTS] : TEXCOORD5;
};

float4 main(InputType input) : SV_TARGET
{
	float bias = 0.0001f;					// Set the bias value for fixing the floating point precision issues.
    float4 color;							// final fragment colour
	float2 projectTexCoord[NUM_LIGHTS];		//
	float depthValue;						//
	float distance;							// distance from the light to the fragment in world space
	float attenuationFactor;				// Result of the atenuation equation, affects how lights fade over distance
	float lightDepthValue;					//
    float lightIntensity;					//
	float3 projectionTint;					// Used when light is cast through a transparent object
	float4 textureColor;					//

	// Set the default output color to the ambient light value for all pixels.
    color = ambientColor[0];

	// Calculate the projected texture coordinates.
	for( int i = 0; i < NUM_LIGHTS; i++ )
	{
		projectTexCoord[i].x = input.lightViewPosition[i].x / input.lightViewPosition[i].w / 2.0f + 0.5f;
		projectTexCoord[i].y = -input.lightViewPosition[i].y / input.lightViewPosition[i].w / 2.0f + 0.5f;
	}

	// Calculate the world space distance from the light to the fragment
	distance = length(lightPosition[0].xyz - input.position3D);

	// Determine if the projected coordinates are in the 0 to 1 range.  If so then this pixel is in the view of the light.
	if(	(saturate(projectTexCoord[0].x) == projectTexCoord[0].x) &&
		(saturate(projectTexCoord[0].y) == projectTexCoord[0].y) )
	{
		// Sample the shadow map depth value from the depth texture using the sampler at the projected texture coordinate location.
		depthValue = depthMapTexture1.Sample( SampleTypeClamp, projectTexCoord[0] ).a;
		projectionTint = depthMapTexture1.Sample( SampleTypeClamp, projectTexCoord[0] ).rgb;

		// Calculate the depth of the light.
		lightDepthValue = input.lightViewPosition[0].z / input.lightViewPosition[0].w;

		// Subtract the bias from the lightDepthValue.
		lightDepthValue = lightDepthValue - bias;

		// Compare the depth of the shadow map value and the depth of the light to determine whether to shadow or to light this pixel.
		// If the light is in front of the object then light the pixel, if not then shadow this pixel since an object (occluder) is casting a shadow on it.
		if(lightDepthValue < depthValue && distance < attenuation[0].x )
		{
			// The fragment is receiving light

		    // Calculate the amount of light on this pixel.
			lightIntensity = saturate(dot(input.normal, input.lightPos[0]));

		    if(lightIntensity > 0.0f)
			{
				// Calculate Attenuation
				attenuationFactor = 1 / (attenuation[0].y +
					attenuation[0].z * distance +
					attenuation[0].w * distance * distance);

				// Determine the diffuse color based on the diffuse color and the amount of light intensity.
				color += (diffuseColor[0] * lightIntensity * attenuationFactor);

				// Tint by the projection colour
				color *= float4( projectionTint, 1.0f);

				// Saturate the final light color.
				color = saturate(color);
			}
		}
	}

	// Sample the pixel color from the texture using the sampler at this texture coordinate location.
	textureColor = shaderTexture.Sample(SampleTypeWrap, input.tex);

	// Combine the light and texture color.
	color = color * textureColor;

	// store the depth
	color.a = input.depthPosition.z / input.depthPosition.w;

    return color;
}