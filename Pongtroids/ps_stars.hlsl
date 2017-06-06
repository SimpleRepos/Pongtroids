cbuffer cb {
  float3 offset;
  float time;
};

float4 main(float4 col : COLOR) : SV_TARGET {
  col.a = (cos(col.a * time) + 1) * 0.5;
	return col;
}
