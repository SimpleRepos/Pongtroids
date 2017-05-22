cbuffer cb {
  matrix transWVP;
};

struct VOut {
  float4 pos : SV_POSITION;
  float2 uv  : TEXCOORD;
};

struct VIn {
  float4 pos : POSITION;
  float4 norm : NORMAL;
  float2 uv  : TEXCOORD;
};

VOut main(VIn input) {
  VOut output;
  output.pos = mul(input.pos, transWVP);
  output.uv = input.uv;
	return output;
}
