cbuffer Camera : register(b1)
{
    row_major matrix viewProjectionMatrix;
};

cbuffer Transform : register(b2)
{
    row_major matrix transform;
};

struct VSOut
{
    float4 color : Color;
    float4 tex : TexCoord;
    float4 pos : SV_Position;
};

VSOut main( float2 pos : Position, float4 color : Color, float2 tex : TexCoord ) 
{
    VSOut vso;
    vso.pos = mul(float4(pos.x, pos.y, 0.0f, 1.0f), transform);
    vso.pos = mul(vso.pos, viewProjectionMatrix);
    vso.color = color;
    vso.tex = float4(tex.x, tex.y, 0.0f, 1.0f);
    return vso;
}