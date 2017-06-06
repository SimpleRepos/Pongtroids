cbuffer cb {
  matrix transWVP;
};

struct VOut {
  float4 pos : SV_POSITION;
  float2 uv  : TEXCOORD;
};

struct VIn {
  float3 pos  : POSITION;
  float3 norm : NORMAL;
  float2 uv   : TEXCOORD;
};

VOut main(VIn input) {
  VOut output;
  output.pos = mul(float4(input.pos, 1), transWVP);
  output.uv = input.uv;
	return output;
}

