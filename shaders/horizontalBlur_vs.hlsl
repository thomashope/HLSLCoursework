cbuffer MatrixBuffer : register(cb0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer ScreenSizeBuffer : register(cb1)
{
    float screenWidth;
    float3 padding;
};

struct InputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
};

struct OutputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float2 texCoord1 : TEXCOORD1;
    float2 texCoord2 : TEXCOORD2;
    float2 texCoord3 : TEXCOORD3;
    float2 texCoord4 : TEXCOORD4;
    float2 texCoord5 : TEXCOORD5;
};


OutputType main(InputType input)
{
    OutputType output;
    float texelSize;
	
    // Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    // Store the texture coordinates for the pixel shader.
    output.tex = input.tex;

	// Determine the floating point size of a texel for a screen with this specific width.
    texelSize = 1.0f / screenWidth;

	 // Create UV coordinates for the pixel and its four horizontal neighbors on either side.
    output.texCoord1 = input.tex + float2(texelSize * -2.0f, 0.0f);
    output.texCoord2 = input.tex + float2(texelSize * -1.0f, 0.0f);
    output.texCoord3 = input.tex + float2(texelSize *  0.0f, 0.0f);
    output.texCoord4 = input.tex + float2(texelSize *  1.0f, 0.0f);
    output.texCoord5 = input.tex + float2(texelSize *  2.0f, 0.0f);

    return output;
}