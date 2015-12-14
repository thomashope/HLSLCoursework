
#define NUM_LIGHTS 2

cbuffer MatrixBuffer : register(cb0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;

	matrix lightViewMatrix[NUM_LIGHTS];
	matrix lightProjectionMatrix[NUM_LIGHTS];
};

cbuffer LightBuffer : register(cb1)
{
	float4 lightPosition[NUM_LIGHTS];
};

struct InputType
{
    float4 position : POSITION;
    float2 tex		: TEXCOORD0;
	float3 normal	: NORMAL;
};

struct OutputType
{
	float4 position				: SV_POSITION;
    float2 tex					: TEXCOORD0;
	float3 normal				: NORMAL;

	float3 position3D			: TEXCOORD1;
	float4 depthPosition		: TEXCOORD2;

	float4 lightViewPosition	[NUM_LIGHTS] : TEXCOORD3;
	float3 lightPos				[NUM_LIGHTS] : TEXCOORD5;
};


OutputType main(InputType input)
{
    OutputType output;
	float4 worldPosition;

	// Calculate the depthPosition of the vertex against the world, view, and projection matrices.
	output.depthPosition = mul( input.position, worldMatrix );
	output.depthPosition = mul( output.depthPosition, viewMatrix );
	output.depthPosition = mul( output.depthPosition, projectionMatrix );

	// Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul( input.position, worldMatrix );
	output.position = mul( output.position, viewMatrix );
	output.position = mul( output.position, projectionMatrix );


	// Store the texture coordinates for the pixel shader.
    output.tex = input.tex;
    
	// Calculate the normal vector against the world matrix only.
    output.normal = mul(input.normal, (float3x3)worldMatrix);	
    output.normal = normalize(output.normal);

    // Calculate the position of the vertex in the world.
    worldPosition = mul(input.position, worldMatrix);

	// convert the world position to a float3 and pass to output
	output.position3D = worldPosition.xyz;

	for( int i = 0; i < NUM_LIGHTS; i++ )
	{
		// Calculate the position of the vertice as viewed by the light source.
		output.lightViewPosition[i] = mul( input.position, worldMatrix );
		output.lightViewPosition[i] = mul( output.lightViewPosition[i], lightViewMatrix[i] );
		output.lightViewPosition[i] = mul( output.lightViewPosition[i], lightProjectionMatrix[i] );

		// Determine the light position based on the position of the light and the position of the vertex in the world.
		output.lightPos[i] = lightPosition[i].xyz - worldPosition.xyz;
		output.lightPos[i] = normalize( output.lightPos[i] );
	}


	return output;
}