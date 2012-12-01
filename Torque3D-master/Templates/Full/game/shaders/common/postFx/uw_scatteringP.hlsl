// Samplers
uniform sampler2D shafts_texture : register(S0);


// Constants
float3 HGg;
float3 beta_dash_Mie;
float3 lightDirection;
float3 intensity;



float compute_phase(float3 refr, float3 view_vector) {

  float dot_product = dot(refr, view_vector);

/*
  HGg.x = (1-HGg*HGg);  // (1-g^2);
  HGg.y = 1 + HGg*HGg;
  HGg.z = 2*HGg;
 */

// Mie
  float num = HGg.x;
  float den = (HGg.y - HGg.z*dot_product); 
  den = rsqrt(den); 
  float phase = num * (den*den*den);
  return phase;
}


float4 main(
			float3 world_pos      : TEXCOORD0,
			float2 uv             : TEXCOORD1,
			float3 out_scattering : TEXCOORD2
           ) : COLOR
{

  float3 view_vector = normalize(world_pos);  // view origin at 0,0,0
  float4 shafts = tex2D(shafts_texture, uv);
  float  phase = compute_phase( view_vector, -lightDirection);

// Compute Fresnel term, assuming a vertical normal (0,-1,0)
  float  dotVN = saturate( dot(-view_vector, float3(0,0,-1) ) );

  float3 color = (0.15 + intensity*shafts.xyz)*phase; // *   // view dep. scattering
				 //shafts.a;  // shadow

  return float4(color, 1);
}