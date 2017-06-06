Texture2D tex : register(t0);
SamplerState samp;

struct VOut {
  float4 pos : SV_POSITION;
  float2 uv : TEXCOORD;
};

float4 main(VOut input) : SV_TARGET {
  float4 col = tex.Sample(samp, input.uv);
  clip(col.a - 0.5);
	return col;
}
