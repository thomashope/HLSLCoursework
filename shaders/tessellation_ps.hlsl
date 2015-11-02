// Tessellation pixel shader
// Output colour passed to stage.

struct InputType
{
    float4 position : SV_POSITION;
    float4 colour : COLOR;
};

float4 main(InputType input) : SV_TARGET
{
	//return float4(1.0, 0.0, 0.0, 1.0);
    return input.colour;
}