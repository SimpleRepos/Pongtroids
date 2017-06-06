cbuffer cb {
  float3 offset;
  float time;
};

struct InVert {
  float3 pos : POSITION;
  float invPeriod : PSIZE;
  float3 col : COLOR;
};

struct OutVert {
  float4 pos : SV_POSITION;
  float4 col : COLOR;
};

OutVert main(InVert vert) {
  OutVert v;
  v.pos = float4(vert.pos + offset, 1);
  v.col = float4(vert.col, vert.invPeriod);
  return v;
}
