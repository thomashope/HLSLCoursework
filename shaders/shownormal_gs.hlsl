
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
	float4 position			: SV_POSITION;
	float4 depthPosition	: TEXCOORD0;
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

[maxvertexcount(3)]
void main(triangle InputType input[3], inout LineStream<OutputType> Stream)
{
	OutputType output;
	float Explode = 0.2f;
    
    // Calculate the face normal
    float3 faceEdgeA = input[1].position.xyz - input[0].position.xyz;
    float3 faceEdgeB = input[2].position.xyz - input[0].position.xyz;
	float3 faceNormal = normalize( cross( faceEdgeB, faceEdgeA ) );
    float3 ExplodeAmt = faceNormal*Explode;
    
    // Calculate the face center
    float3 centerPos = (input[0].position.xyz + input[1].position.xyz + input[2].position.xyz)/3.0;
    float2 centerTex = (input[0].tex + input[1].tex + input[2].tex)/3.0;

	// center of triangle
	output.position = float4(centerPos, 1);
	output.position = mul( output.position, worldMatrix );
	output.position = mul( output.position, viewMatrix );
	output.position = mul( output.position, projectionMatrix );
	output.depthPosition = output.position;
	Stream.Append(output);
		
	// farther out
	output.position = float4(centerPos, 1) + float4(ExplodeAmt, 0);
	output.position = mul( output.position, worldMatrix );
	output.position = mul( output.position, viewMatrix );
	output.position = mul( output.position, projectionMatrix );
	output.depthPosition = output.position;
	Stream.Append( output );

	Stream.RestartStrip();   
}