// Tessellation Hull Shader
// Prepares control points for tessellation

cbuffer TessellationBuffer : register(cb)
{
    float tessellationFactor;
    float3 padding;
};

struct InputType
{
    float3 position : POSITION;
};

struct ConstantOutputType
{
    float edges[4] : SV_TessFactor;
    float inside[2] : SV_InsideTessFactor;
};

struct OutputType
{
    float3 position : POSITION;
};

ConstantOutputType PatchConstantFunction(InputPatch<InputType, 4> inputPatch, uint patchId : SV_PrimitiveID)
{    
    ConstantOutputType output;

    // Set the tessellation factors for the three edges of the triangle.
    output.edges[0] = tessellationFactor;
    output.edges[1] = tessellationFactor;
    output.edges[2] = tessellationFactor;
	output.edges[3] = tessellationFactor;

    // Set the tessellation factor for tessallating inside the triangle.
    output.inside[0] = tessellationFactor;
	output.inside[1] = tessellationFactor;

    return output;
}

[domain( "quad" )]
[partitioning( "fractional_odd" )]
[outputtopology( "triangle_ccw" )]
[outputcontrolpoints( 4 )]
[patchconstantfunc( "PatchConstantFunction" )]
[maxtessfactor( 31.0f )]
OutputType main(InputPatch<InputType, 4> patch, uint pointId : SV_OutputControlPointID, uint patchId : SV_PrimitiveID)
{
    OutputType output;

    // Set the position for this control point as the output position.
    output.position = patch[pointId].position;

    return output;
}