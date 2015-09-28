// Light vertex shader
// Standard issue vertex shader, apply matrices, pass info to pixel shader

cbuffer MatrixBuffer : register(cb0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer CameraBuffer : register(cb1)
{
	float3 cameraPosition;
	float padding;
}

struct InputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

struct OutputType
{
    float4 position : SV_POSITION;
    
	float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
	float3 viewDirection : TEXCOORD1;
	
	float3 position3D : TEXCOORD2;
	//float padding; //TODO: does struct need padding?
};

OutputType main(InputType input)
{
    OutputType output;
	float4 worldPosition;
    
    // Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    // Store the texture coordinates for the pixel shader.
    output.tex = input.tex;

	 // Calculate the normal vector against the world matrix only.
    output.normal = mul(input.normal, (float3x3)worldMatrix);
	
    // Normalize the normal vector.
    output.normal = normalize(output.normal);

	// Calculate the position of the vertex in the world
	worldPosition = mul( input.position, worldMatrix );

	output.position3D = worldPosition.xyz;

	// Determine the viewing direction based on the position of the camer and the position of the vertex in the world
	output.viewDirection = normalize( cameraPosition.xyz - worldPosition.xyz );

	// normalize the viewing direction vector
	//output.viewDirection = normalize( output.viewDirection );

    return output;
}