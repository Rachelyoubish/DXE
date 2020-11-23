struct VS_OUTPUT
{
    float3 color : COLOR;
    float4 pos : SV_Position;
};

VS_OUTPUT main(float2 pos : Position, float3 color : Color)
{
    VS_OUTPUT Output;
    Output.pos = float4(pos.x, pos.y, 0.0f, 1.0f);
    Output.color = color;
    return Output;
}