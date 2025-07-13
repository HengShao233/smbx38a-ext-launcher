struct VS_INPUT {
  float3 position : POSITION;
  float2 texCoord : TEXCOORD;
};

struct PS_INPUT {
  float4 color : TEXCOORD1;
  float2 texCoord : TEXCOORD;
  float4 position : SV_POSITION;
};

cbuffer ShapeBuffer : register(b0) {
  float4 shape;
  float4 color;
  float depth;
};

float2 trans(float2 pos)
{
  pos += float2(1.0, -1.0);
  pos *= float2(0.5, 0.5) * shape.zw;
  pos += float2(shape.x, -shape.y);
  pos *= float2(2.0, 2.0);
  pos -= float2(1.0, -1.0);
  return pos;
}

PS_INPUT main(VS_INPUT input) {
  PS_INPUT output;

  input.position.xy = trans(input.position.xy);
  input.position.z = depth;

  output.position = float4(input.position, 1.0);
  output.texCoord = input.texCoord;
  output.color = color;
  return output;
}
