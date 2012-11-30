// Constants
float4x4 matScreenToWorld;
float3   attenuation_c;
float    water_altitude;


struct VS_INPUT {
  float4 pos         : POSITION;
  float4 uv          : TEXCOORD0;
};

struct VS_OUTPUT {
  float4 pos            : POSITION;
  float3 world_pos      : TEXCOORD0;
  float2 uv             : TEXCOORD1;
  float3 out_scattering : TEXCOORD2;
};


VS_OUTPUT main(const VS_INPUT input) {
  VS_OUTPUT output = (VS_OUTPUT)0;

  output.pos = input.pos;

  float4 world_pos = mul(matScreenToWorld, input.pos);
  output.world_pos = world_pos;
  output.uv = input.uv;
    
  float depth = water_altitude-world_pos.y;
  output.out_scattering = exp( -attenuation_c*max(0,depth) );

  return output;
}
