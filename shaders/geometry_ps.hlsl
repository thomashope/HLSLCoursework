struct InputType
{
	float4 position : SV_POSITION;
	float3 normal	: NORMAL;
};

float4 main(InputType input) : SV_TARGET
{
	float4 normalColour;

	// set che colour based on the angle of the normal
	normalColour.xyz = abs( input.normal );
	normalColour.w = 1.0f;

	return normalColour;
}