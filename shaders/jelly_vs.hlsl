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
	float time;
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
};

OutputType main(InputType input)
{
    OutputType output;
	float4 worldPosition;
	float height = 0.2;
    
    // Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

	input.normal = normalize( input.normal );

	//input.position.xyz += input.normal * sin( time );

	//input.position.y = height * sin( input.position.x + time );
	//input.normal.x = 1 - cos( input.position.x + time );
	//input.normal.y = abs( cos( input.position.x + time ) );
	
	input.position.xyz += input.normal * sin( length(input.position.xz - float2(20.0f, 20.0f)) - time ) * height;

	// recalculate normals, TODO: find out how this should be
	input.normal.x = 1 - cos( length( input.position.xz - float2(20.0f, 20.0f) ) - time );
	input.normal.z = 1 - sin( length( input.position.xz - float2(20.0f, 20.0f) ) - time );
	input.normal.y = abs( cos( length( input.position.xz - float2(20.0f, 20.0f) ) - time ) );

	input.normal = normalize( input.normal );

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    // Store the texture coordinates for the pixel shader.
    output.tex = input.tex;

	 // Calculate the normal vector against the world matrix only.
    output.normal = normalize( mul(input.normal, (float3x3)worldMatrix) );
	
	// Calculate the position of the vertex in the world
	worldPosition = mul( input.position, worldMatrix );

	// convert the world position to a float3 and pass to output
	output.position3D = worldPosition.xyz;

	// Determine the viewing direction based on the position of the camer and the position of the vertex in the world
	output.viewDirection = cameraPosition.xyz - worldPosition.xyz;

	// normalize the viewing direction vector
	output.viewDirection = normalize( output.viewDirection );

    return output;
}