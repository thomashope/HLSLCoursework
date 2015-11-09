struct InputType
{
    float4 position : SV_POSITION;
    float4 depthPosition : TEXCOORD0;
};

float4 main(InputType input) : SV_TARGET
{
	float depthValue;
	float4 color;

	// Get the depth value of the pixel by dividing the Z pixel depth by the homogeneous W coordinate.
	depthValue = input.depthPosition.z / input.depthPosition.w;

	color = float4(depthValue, depthValue, depthValue, 1.0f);
	//color = float4(1.0f - depthValue, 1.0f - depthValue, 1.0f - depthValue, 1.0f);
	//color *= 4;
	saturate(color);

	return color;
}