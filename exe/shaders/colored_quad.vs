struct VSInput
{
    float4 position : POSITION;
    float4 color : COLOR;
};

struct VSOutput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

VSOutput main(VSInput input)
{
    VSOutput output;
    input.position.z = 0;
    input.position.w = 1;
    output.position = input.position;//float4(input.position, 0.0, 1.0);
    output.color = input.color;
    return output;
}
