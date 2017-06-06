cbuffer cb {
  matrix transWVP;
};

struct Vertex {
  float4 pos : SV_POSITION;
  float2 uv  : TEXCOORD;
};

Vertex main(uint index : SV_VertexID) {
  Vertex v;
  v.uv = float2(index & 1, index >> 1);
  v.pos = mul(float4(v.uv, 0, 1), transWVP);
	return v;
}

