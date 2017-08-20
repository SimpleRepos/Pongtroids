Texture2D tex : register(t0);
SamplerState samp;

cbuffer cb : register(b0)  {
  float4x4 xform;
  float4 foreColor;
  float4 backColor;
};

struct VOut {
  float4 pos : SV_POSITION;
  float2 uv : TEXCOORD;
};

float4 main(VOut input) : SV_TARGET {
  float4 val = tex.Sample(samp, input.uv);

  float4 fg = foreColor * val.r;
  float4 bg = backColor * (1 - val.r);

  float4 col = fg + bg;
  col.a = val.r + val.g;

  return col;
}
