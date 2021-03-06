
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
    float4 position : POSITION; // TODO input is 3 instead of 4?
    float2 tex		: TEXCOORD0;
	float3 normal	: NORMAL;
	//float3 binormal : BINORMAL;
	//float3 tangent	: TANGENT;
};

struct OutputType
{
	float4 position				: SV_POSITION;
    float2 tex					: TEXCOORD0;
	float3 normal				: NORMAL0;
	float3 binormal				: BINORMAL;
	float3 tangent				: TANGENT;

	float3 position3D			: TEXCOORD1;
	float4 depthPosition		: TEXCOORD2;

	float4 lightViewPosition	[NUM_LIGHTS] : TEXCOORD3;
	float3 lightPos				[NUM_LIGHTS] : TEXCOORD5;
};

OutputType main(InputType input)
{
    OutputType output;
	float4 worldPosition;

	// Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

	// Calculate the depthPosition of the vertex against the world, view, and projection matrices.
	output.depthPosition = mul( input.position, worldMatrix );
	output.depthPosition = mul( output.depthPosition, viewMatrix );
	output.depthPosition = mul( output.depthPosition, projectionMatrix );

	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul( input.position, worldMatrix );
	output.position = mul( output.position, viewMatrix );
	output.position = mul( output.position, projectionMatrix );

	// Calculate the normal vector against the world matrix only and then normalize the final value.
	output.normal = mul( float3(0.0f, 1.0f, 0.0f), (float3x3)worldMatrix );
	output.normal = normalize( output.normal );

	// Calculate the tangent vector against the world matrix only and then normalize the final value.
	output.tangent = mul( float3(0.0f, 0.0f, 1.0f), (float3x3)worldMatrix);
	output.tangent = normalize( output.tangent );

	// Calculate the binormal vector against the world matrix only and then normalize the final value.
	output.binormal = mul( float3(1.0f, 0.0f, 0.0f), (float3x3)worldMatrix );
	output.binormal = normalize( output.binormal );

	// Store the texture coordinates for the pixel shader.
    output.tex = input.tex;

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