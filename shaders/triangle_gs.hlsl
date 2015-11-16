
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
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
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

//[maxvertexcount(2)]
//'void main(triangle InputType input[3], inout TriangleStream<OutputType> triStream)
[maxvertexcount(4)]
void main(point InputType input[1], inout TriangleStream<OutputType> triStream)
{
	/*
	OutputType output;
	float Explode = 1.0f;
    
    // Calculate the face normal
    float3 faceEdgeA = input[1].position - input[0].position;
    float3 faceEdgeB = input[2].position - input[0].position;
    float3 faceNormal = normalize( cross(faceEdgeA, faceEdgeB) );
    float3 ExplodeAmt = faceNormal*Explode;
    
    // Calculate the face center
    float3 centerPos = (input[0].position.xyz + input[1].position.xyz + input[2].position.xyz)/3.0;
    float2 centerTex = (input[0].tex + input[1].tex + input[2].tex)/3.0;
    centerPos += faceNormal*Explode;

	// center of triangle
	output.position = float4(centerPos, 1);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
	output.normal = faceNormal;
	output.tex = centerTex;
	triStream.Append(output);
		
	// farther out
	output.position = float4(centerPos, 1) + float4(ExplodeAmt, 0);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
	output.normal = faceNormal;
	output.tex = centerTex;
	triStream.Append(output);

	triStream.RestartStrip();
    //*/

    // Output the pyramid
    /*
    for( int i=0; i<3; i++ )
    {
        output.position = input[i].position + float4(ExplodeAmt,0);
        output.position = mul( output.position, viewMatrix );
        output.position = mul( output.position, projectionMatrix );
        output.normal = input[i].normal;
        output.tex = input[i].tex;
        triStream.Append( output );
        
        int iNext = (i+1)%3;
        output.position = input[iNext].position + float4(ExplodeAmt,0);
        output.position = mul( output.position, viewMatrix );
        output.position = mul( output.position, projectionMatrix );
        output.normal = input[iNext].normal;
        output.tex = input[iNext].tex;
        triStream.Append( output );
        
        output.position = float4(centerPos,1) + float4(ExplodeAmt,0);
        output.position = mul( output.position, viewMatrix );
        output.position = mul( output.position, projectionMatrix );
        output.normal = faceNormal;
        output.tex = centerTex;
        triStream.Append( output );
        
        triStream.RestartStrip();
    }
    
    for( int i=2; i>=0; i-- )
    {
        output.position = input[i].position + float4(ExplodeAmt,0);
        output.position = mul( output.position, viewMatrix );
        output.position = mul( output.position, projectionMatrix );
        output.normal = -input[i].normal;
        output.tex = input[i].tex;
        triStream.Append( output );
    }
    triStream.RestartStrip();	
	*/

	//*
	OutputType output;
	
	for (int i = 0; i < 4; i++)
	{
		float3 vposition = g_positions[i];
		vposition = mul(vposition, (float3x3)worldMatrix) + input[0].position;
		output.position = mul(float4(vposition, 1.0), viewMatrix);
		output.position = mul(output.position, projectionMatrix);

		output.tex = input[0].tex;
		output.normal = input[0].normal;
		triStream.Append(output);
	}

	triStream.RestartStrip();
	//*/

	/*
	// Change the position vector to be 4 units for proper matrix calculations.
    input[0].position.w = 1.0f;
		
	// Move the vertex away from the point position
	output.position = input[0].position + float4(0.0, 1.0, 0.0, 0.0);
	
	output.position = mul(output.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

	output.tex = input[0].tex;

    output.normal = mul(input[0].normal, (float3x3)worldMatrix);
    output.normal = normalize(output.normal);

	triStream.Append(output);

	// Move the vertex away from the point position
	output.position = input[0].position + float4(1.0, 0.0, 0.0, 0.0);
	
	output.position = mul(output.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

	output.tex = input[0].tex;

    output.normal = mul(input[0].normal, (float3x3)worldMatrix);
    output.normal = normalize(output.normal);

	triStream.Append(output);

	// Move the vertex away from the point position
	output.position = input[0].position + float4(-1.0, 0.0, 0.0, 0.0);
	
	output.position = mul(output.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

	output.tex = input[0].tex;

    output.normal = mul(input[0].normal, (float3x3)worldMatrix);
    output.normal = normalize(output.normal);

	triStream.Append(output);

	triStream.RestartStrip();
	*/

}