Texture2D shaderTexture;
SamplerState sampleType;

float4 main(float4 color : Color, float2 tex : TexCoord) : SV_Target
{
    return shaderTexture.Sample(sampleType, tex);
}