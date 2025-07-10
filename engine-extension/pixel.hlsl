Texture2D mainTexture : register(t0);
SamplerState __sampler_mainTexture : register(s0);

struct PS_INPUT {
    float4 color : TEXCOORD1;
    float2 texCoord : TEXCOORD;
    float4 position : SV_POSITION;
};

float4 main(PS_INPUT input) : SV_Target{
    return mainTexture.Sample(__sampler_mainTexture, input.texCoord);

}