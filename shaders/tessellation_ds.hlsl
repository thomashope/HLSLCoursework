// Tessellation domain shader
// After tessellation the domain shader processes the all the vertices

cbuffer MatrixBuffer : register(cb0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer VertexManipBuffer : register(cb1)
{
	float time;
	float3 padding;
}

struct ConstantOutputType
{
    float edges[4] : SV_TessFactor;
    float inside[2] : SV_InsideTessFactor;
};

struct InputType
{
    float3 position : POSITION;
    float4 colour : COLOR;
};

struct OutputType
{
    float4 position : SV_POSITION;
	float4 depthPosition : TEXCOORD0;
	float4 normal : NORMAL;
    float4 colour : COLOR;
};

[domain("quad")]
OutputType main(ConstantOutputType input, float2 uvwCoord : SV_DomainLocation, const OutputPatch<InputType, 4> patch)
{
    float3 position;
	float3 normal;
    OutputType output;
    
	// The new vertex position interpolated across the quad
	float3 v1 = lerp(patch[0].position, patch[1].position, 1 - uvwCoord.y);
	float3 v2 = lerp(patch[2].position, patch[3].position, 1 - uvwCoord.y);
	position = lerp( v1, v2, uvwCoord.x );

	// create the sphere
	position = normalize( position );
	normal = position + position;
	
	// distort the sphere
	float3 freq = float3(
		sin( time * 3.4f ),
		sin( time * 5.3f ),
		sin( time * 2.7f ) );
	float amp = 0.5f + sin( time * 2.0f ) * 0.3f;
	freq *= position * amp;
	position += position * freq;

	// aproximation of the normal, good enough
	output.normal = float4( normalize( normal - position ), 1.0f);

    // Calculate the position of the new vertex against the world, view, and projection matrices.
	output.position = mul( float4(position, 1.0f), worldMatrix );
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

	// Store the position value in a second input value for depth value calculations.
	output.depthPosition = output.position;
	
    // Send the input color into the pixel shader.
	output.colour = saturate( float4( abs(position), 1.0f) );

    return output;
}

