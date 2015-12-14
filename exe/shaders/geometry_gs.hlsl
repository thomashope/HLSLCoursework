
cbuffer MatrixBuffer : register(cb0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

struct InputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct OutputType
{
	float4 position : SV_POSITION;
	float3 normal	: NORMAL;
};

cbuffer PositionBuffer
{
	static float3 g_positions[4] =
	{
		float3(-0.5, 0.5, 0),
		float3(-0.5, -0.5, 0),
		float3(0.5, 0.5, 0),
		float3(0.5, -0.5, 0)
	};
};

[maxvertexcount(9)]
void main(triangle InputType input[3], inout LineStream<OutputType> Stream)
{
	OutputType output;

	// Calculate the normal and center of the face
	float3 normal = normalize( cross( (input[2].position - input[0].position).xyz, (input[1].position - input[0].position).xyz ) );
	float3 center = (input[0].position.xyz + input[1].position.xyz + input[2].position.xyz) / 3.0;

	output.normal = normal;

	float4 v0 = float4(lerp( input[0].position.xyz, center, 0.1f ) + normal * 0.1f, 1.0f);
	float4 v1 = float4(lerp( input[1].position.xyz, center, 0.1f ) + normal * 0.1f, 1.0f);
	float4 v2 = float4(lerp( input[2].position.xyz, center, 0.1f ) + normal * 0.1f, 1.0f);

	// multiply by world, view, projection
	v0 = mul( v0, worldMatrix );
	v0 = mul( v0, viewMatrix );
	v0 = mul( v0, projectionMatrix );
	v1 = mul( v1, worldMatrix );
	v1 = mul( v1, viewMatrix );
	v1 = mul( v1, projectionMatrix );
	v2 = mul( v2, worldMatrix );
	v2 = mul( v2, viewMatrix );
	v2 = mul( v2, projectionMatrix );

	// send each line to the pixel shader

	// v0 -> v1
	output.position = v0;
	Stream.Append( output );
	output.position = v1;
	Stream.Append( output );
	Stream.RestartStrip( );

	// v1 -> v2
	Stream.Append( output );
	output.position = v2;
	Stream.Append( output );
	Stream.RestartStrip( );

	// v2 -> v0
	Stream.Append( output );
	output.position = v0;
	Stream.Append( output );
	Stream.RestartStrip( );
}