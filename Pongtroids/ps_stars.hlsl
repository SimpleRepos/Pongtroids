cbuffer cb {
  float3 offset;
  float time;
};

struct Vert {
  float4 pos : SV_POSITION;
  float4 col : COLOR;
};


float4 main(Vert vert) : SV_TARGET {
  vert.col *= 1 + (cos(time * vert.col.a) * 0.5);
	return vert.col;
}
