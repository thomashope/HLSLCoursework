Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
	float2 texCoord1 : TEXCOORD1;
	float2 texCoord2 : TEXCOORD2;
	float2 texCoord3 : TEXCOORD3;
	float2 texCoord4 : TEXCOORD4;
	float2 texCoord5 : TEXCOORD5;
	float2 texCoord6 : TEXCOORD6;
	float2 texCoord7 : TEXCOORD7;
	float2 texCoord8 : TEXCOORD8;

};

float4 main(InputType input) : SV_TARGET
{
    float4 colour;

	// Initialize the color to black.
	colour = float4(0.0f, 0.0f, 0.0f, 0.0f);

    // Add the nine surrounding pixels to the colour.
	// Preserve the alpha channel of the center pixel
	colour += shaderTexture.Sample(SampleType, input.tex);
	colour.xyz += shaderTexture.Sample(SampleType, input.texCoord1).xyz;
	colour.xyz += shaderTexture.Sample(SampleType, input.texCoord2).xyz;
	colour.xyz += shaderTexture.Sample(SampleType, input.texCoord3).xyz;
	colour.xyz += shaderTexture.Sample(SampleType, input.texCoord4).xyz;
	colour.xyz += shaderTexture.Sample(SampleType, input.texCoord5).xyz;
	colour.xyz += shaderTexture.Sample(SampleType, input.texCoord6).xyz;
	colour.xyz += shaderTexture.Sample(SampleType, input.texCoord7).xyz;
	colour.xyz += shaderTexture.Sample(SampleType, input.texCoord8).xyz;

	colour /= 9.0f;

	// Set the alpha channel to one.
	//colour.a = 1.0f;

	//colour = float4(1.0f, 0.0f, 1.0f, 1.0f);

	return colour;
}
