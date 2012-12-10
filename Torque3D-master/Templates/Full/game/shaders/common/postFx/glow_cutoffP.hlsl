// Samplers
sampler src_texture;


// Constants
float cutoff;


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
float4 main(in float2 uv : TEXCOORD0) : COLOR
{
  float4 sample = tex2D(src_texture, uv);
	float  lum = dot(sample.rgb, float3(0.33,0.33,0.33))+sample.a*0.05; 
	sample.rgb *= max(lum-cutoff, 0);

  return sample;
}
