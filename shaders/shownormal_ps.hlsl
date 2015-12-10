struct InputType
{
	float4 position			: SV_POSITION;
	float4 depthPosition	: TEXCOORD0;
};

float4 main(InputType input) : SV_TARGET
{
	return float4(0.6f, 0.6f, 0.6f, input.depthPosition.z / input.depthPosition.w);
}